/**
 * Merges the Embind-generated .d.ts and the hand-written declarations into a single declaration
 * file, with the enriched signatures sitting in place of the loose generated ones.
 *
 * There are two kinds of hand-written input, distinguished by provenance:
 *
 * - Type overlays (src/type-overlays) refine members Embind *did* generate but typed badly, because
 *   `emscripten::val` parameters collapse to `any`. Each member must match a generated one.
 * - JS declarations (src/js-declarations) declare members Embind cannot see at all, because they
 *   are attached to the module from JavaScript. Each member must *not* match a generated one.
 *
 * The script will fail if either of the rules above are violated.
 *
 * The generated file's structure is preserved exactly. Only the targeted members of `EmbindModule`
 * are rewritten, plus any standalone declarations the inputs add.
 *
 * Steps:
 *
 * 1. Parse the arguments, and refuse to run if --out would overwrite --generated.
 * 2. Read each folder's index.d.ts manifest (`TypeOverrides`, `JsDeclarations`) to find the
 *    interfaces it contributes and the file each one lives in.
 * 3. Load the generated file and note its line endings, which everything injected will match.
 * 4. Check every member of those interfaces against `EmbindModule`, and plan a text edit for each:
 *    - a member claimed by more than one file, or matching an overload set, is a problem;
 *    - an overlay member must match a generated one, which it replaces - the generated JSDoc is
 *      kept unless the overlay brings its own;
 *    - a declared member must not match one, and is queued to be appended to the interface.
 * 5. Collect the standalone declarations from the input files - anything that is neither an import
 *    nor a manifest interface, such as `declare class CspRequestError`.
 * 6. If any member was a problem, fail with every one listed, before anything is written.
 * 7. Apply the edits last-first so no edit shifts another's offsets, append the standalone
 *    declarations, and write the result.
 * 8. Typecheck the written file on its own, under strict, with only the libs a browser consumer
 *    has and no ambient @types.
 * 9. If --snapshot was given, write the reviewable record of every replaced, declared and
 *    standalone declaration.
 *
 * Usage:
 *   node merge-dts.ts --generated <path> --overlays <dir> --declarations <dir>
 *                     --out <path> [--snapshot <path>]
 *
 * e.g.:
 *   node tools/merge-dts/merge-dts.ts --generated CSP-WASM-Bindings/connected-spaces-platform-bindings.d.ts --overlays src/type-overlays --declarations src/js-declarations --out CSP-WASM-Bindings/merged.d.ts
 */

import { writeFileSync, mkdirSync } from 'node:fs';
import { dirname, resolve, basename } from 'node:path';
import { parseArgs as parseCliArgs } from 'node:util';
import { Node, Project } from 'ts-morph';
import type { SourceFile, TypeElementTypes } from 'ts-morph';
// Deliberately the standalone compiler, not ts-morph's re-export: that one is bundled inside
// @ts-morph/common and its getDefaultLibFilePath points at a directory with no lib.*.d.ts files,
// so every lib global would silently resolve to "Cannot find name".
import ts from 'typescript';

// The interface in the generated file that holds the module's functions and class handles.
// Every hand-written member targets a member of this interface.
const TARGET_INTERFACE = 'EmbindModule';

// Indentation for declared members when the target interface has no members to copy it from.
// Injected lines otherwise take their indentation from the generated file, so this is only a
// fallback.
const FALLBACK_INDENT = '  ';

class MergeError extends Error {}

//==================================================================================================

interface Args {
  generated: string;
  overlays: string;
  declarations: string;
  out: string;
  snapshot?: string | undefined;
}

/**
 * Which way round the membership check runs for a set of hand-written inputs.
 *
 * 'overlay'     - the member must already exist in the generated declarations; we replace it.
 * 'declaration' - the member must not exist; we append it.
 */
type SourceKind = 'overlay' | 'declaration';

/** One interface named in a folder's manifest, and the file it is declared in. */
interface SourceEntry {
  name: string;
  filePath: string;
  sourceFile: SourceFile;
}

/** A folder of hand-written inputs, resolved to the interfaces it contributes. */
interface ResolvedSource {
  kind: SourceKind;
  dir: string;
  entries: SourceEntry[];
}

/** An absolute text span in the generated file, and what to put there. */
interface Edit {
  start: number;
  end: number;
  text: string;
}

/** A generated member whose signature an overlay replaced. */
interface Replacement {
  memberName: string;
  before: string;
  after: string;
}

/** A JS-provided member, appended because Embind emits no counterpart. */
interface Declared {
  memberName: string;
  from: string;
  /** The member as written in its source file, JSDoc included. */
  source: string;
}

/** A standalone declaration lifted wholesale out of a hand-written file, as written there. */
interface Carried {
  from: string;
  text: string;
}

interface Plan {
  edits: Edit[];
  replacements: Replacement[];
  declared: Declared[];
  carried: Carried[];
}

//==================================================================================================

function parseArgs(argv: string[]): Args {
  const { generated, overlays, declarations, out, snapshot } = parseFlags(argv);

  if (!generated) {
    throw new MergeError('Missing required argument --generated.');
  }
  if (!overlays) {
    throw new MergeError('Missing required argument --overlays.');
  }
  if (!declarations) {
    throw new MergeError('Missing required argument --declarations.');
  }
  if (!out) {
    throw new MergeError('Missing required argument --out.');
  }

  return { generated, overlays, declarations, out, snapshot };
}

function parseFlags(argv: string[]) {
  try {
    const { values } = parseCliArgs({
      args: argv,
      strict: true,
      options: {
        generated: { type: 'string' },
        overlays: { type: 'string' },
        declarations: { type: 'string' },
        out: { type: 'string' },
        snapshot: { type: 'string' }
      }
    });
    return values;
  } catch (error) {
    throw new MergeError(error instanceof Error ? error.message : String(error));
  }
}

/**
 * Moves source text lifted from one file to a new indentation depth. The first line is emitted bare
 * - the caller positions it - and later lines have their shared base indentation swapped for
 * `targetIndent`, so indentation *within* the fragment (JSDoc asterisks, wrapped parameters)
 * survives. Line endings are normalised to `eol` along the way.
 */
function reindent(text: string, targetIndent: string, eol: string): string {
  const lines = text.split(/\r?\n/);
  const body = lines.slice(1).filter((line) => line.trim().length > 0);
  const baseIndent = body.length ? Math.min(...body.map((line) => line.length - line.trimStart().length)) : 0;

  return lines
    .map((line, index) => (index === 0 || line.trim().length === 0 ? line : targetIndent + line.slice(baseIndent)))
    .join(eol);
}

function withEol(text: string, eol: string): string {
  return text.split(/\r?\n/).join(eol);
}

/**
 * The indentation actually present in front of a node.
 *
 * ts-morph's getIndentationText() reports the indentation its manipulation settings *would* use
 * (four spaces by default), not what the file contains. Embind's output is not uniformly indented -
 * EmbindModule mixes two and four spaces - so anything we inject has to read the real thing.
 */
function indentationOf(node: Node): string {
  const text = node.getSourceFile().getFullText();
  const start = node.getStart(true);
  const lineStart = text.lastIndexOf('\n', start - 1) + 1;
  const prefix = text.slice(lineStart, start);

  return /^[ \t]*$/.test(prefix) ? prefix : '';
}

/** Interface members have no common `getName()` - index and call signatures are unnamed. */
function nameOf(member: TypeElementTypes): string | undefined {
  return member.getSymbol()?.getName();
}

//==================================================================================================

/**
 * Reads a folder of hand-written inputs. Its index.d.ts exports a manifest alias - an intersection
 * of the folder's interfaces - and each operand is matched back to the `import type` that brought it
 * in, to find the file it lives in.
 */
function readSource(project: Project, kind: SourceKind, dir: string, aliasName: string): ResolvedSource {
  const indexPath = resolve(dir, 'index.d.ts');
  const indexFile = project.addSourceFileAtPath(indexPath);

  const alias = indexFile.getTypeAlias(aliasName);
  if (!alias) {
    throw new MergeError(`Could not find "type ${aliasName} = ..." in ${indexPath}.`);
  }

  const typeNode = alias.getTypeNodeOrThrow();
  const operands = Node.isIntersectionTypeNode(typeNode) ? typeNode.getTypeNodes() : [typeNode];

  const entries = operands.map((operand) => {
    const name = operand.getText().trim();

    const importDecl = indexFile
      .getImportDeclarations()
      .find((decl) => decl.getNamedImports().some((named) => named.getName() === name));

    if (!importDecl) {
      throw new MergeError(
        `"${name}" appears in ${aliasName} but is not imported in ${indexPath}.\n` +
          `Add an "import type { ${name} } from './<file>.d.ts';" alongside the others.`
      );
    }

    const filePath = resolve(dir, importDecl.getModuleSpecifierValue());
    return { name, filePath, sourceFile: project.addSourceFileAtPath(filePath) };
  });

  return { kind, dir, entries };
}

//==================================================================================================

/**
 * Builds the list of text edits that turn the generated file into the merged one.
 * Edits are recorded as absolute {start, end, text} spans and applied in reverse, so no edit
 * disturbs the offsets of any other and the AST is never mutated mid-walk.
 */
function planEdits(generatedFile: SourceFile, sources: ResolvedSource[], eol: string): Plan {
  const target = generatedFile.getInterface(TARGET_INTERFACE);

  if (!target) {
    throw new MergeError(
      `The generated declarations contain no "interface ${TARGET_INTERFACE}".\n` +
        `Embind's output shape has changed; the merge script needs updating to match.`
    );
  }

  const edits: Edit[] = [];
  const replacements: Replacement[] = [];
  const declared: Declared[] = [];
  const carried: Carried[] = [];
  const problems: string[] = [];
  // Which file claimed each member. Two inputs targeting the same one would produce overlapping
  // edits that splice into each other, so the second is reported instead.
  const claimedBy = new Map<string, string>();

  // Declared members follow whatever the member they are appended after uses; replacements use
  // each generated member's own indentation.
  const lastMember = target.getMembers().at(-1);
  const appendIndent = lastMember ? indentationOf(lastMember) : FALLBACK_INDENT;

  for (const { kind, dir, entries } of sources) {
    for (const { name, filePath, sourceFile } of entries) {
      const location = `${basename(dir)}/${basename(filePath)}`;
      const sourceInterface = sourceFile.getInterface(name);

      if (!sourceInterface) {
        throw new MergeError(`${location} does not export an interface named "${name}".`);
      }

      for (const member of sourceInterface.getMembers()) {
        const memberName = nameOf(member);
        if (!memberName) {
          throw new MergeError(`Could not determine a name for a member of ${name} in ${location}.`);
        }

        const earlier = claimedBy.get(memberName);
        if (earlier) {
          problems.push(
            `  ${location} -> ${memberName}: also declared in ${earlier}.\n` +
              `    Each member may appear in only one hand-written file.`
          );

          continue;
        }
        claimedBy.set(memberName, location);

        const matches = target.getMembers().filter((candidate) => nameOf(candidate) === memberName);

        if (matches.length > 1) {
          problems.push(
            `  ${location} -> ${memberName}: "${TARGET_INTERFACE}" declares this member ` +
              `${matches.length} times (an overload set). The merge script replaces single declarations only.`
          );

          continue;
        }

        const generatedMember = matches[0];

        // The drift gates. Each kind asserts the opposite thing about the generated declarations,
        // so a member that changes category - a JS symbol that later gets bound through Embind, or
        // a binding renamed upstream - fails here instead of silently shipping the wrong types.
        if (kind === 'overlay' && !generatedMember) {
          problems.push(
            `  ${location} -> ${memberName}: no member of this name in "${TARGET_INTERFACE}".\n` +
              `    If it was renamed or removed upstream, update the overlay.\n` +
              `    If it is provided by JavaScript rather than Embind, move it to src/js-declarations.`
          );

          continue;
        }

        if (kind === 'declaration' && generatedMember) {
          problems.push(
            `  ${location} -> ${memberName}: "${TARGET_INTERFACE}" already declares this member.\n` +
              `    Embind now generates it, so the hand-written declaration is redundant.\n` +
              `    Delete it, or move it to src/type-overlays to refine the generated signature.`
          );

          continue;
        }

        if (!generatedMember) {
          declared.push({ memberName, from: basename(filePath), source: member.getText(true).trimEnd() });

          continue;
        }

        // The span covers the generated JSDoc only when the overlay brings its own. Otherwise it
        // starts at the signature and whatever Embind documented stays put, so documentation is
        // never lost to a signature fix.
        edits.push({
          start: generatedMember.getStart(member.getJsDocs().length > 0),
          end: generatedMember.getEnd(),
          text: reindent(member.getText(true).trimEnd(), indentationOf(generatedMember), eol)
        });

        replacements.push({
          memberName,
          before: generatedMember.getText().trimEnd(),
          after: member.getText().trimEnd()
        });
      }
    }
  }

  // Standalone declarations the inputs contribute, e.g. `declare class CspRequestError extends Error`
  // plus its `export type { CspRequestError }`. Anything that is not an import and not one of the
  // manifest interfaces themselves is part of the public surface and comes along.
  const allEntries = sources.flatMap((source) => source.entries);
  const manifestNames = new Set(allEntries.map((entry) => entry.name));

  for (const { filePath, sourceFile } of allEntries) {
    for (const statement of sourceFile.getStatements()) {
      if (Node.isImportDeclaration(statement)) {
        continue;
      }
      if (Node.isInterfaceDeclaration(statement) && manifestNames.has(statement.getName())) {
        continue;
      }

      carried.push({ from: basename(filePath), text: statement.getText(true).trimEnd() });
    }
  }

  if (problems.length > 0) {
    throw new MergeError(
      `Hand-written declarations are out of step with the generated ones:\n\n${problems.join('\n\n')}`
    );
  }

  // Append declared members just inside the target interface's closing brace.
  if (declared.length > 0) {
    const insertAt = lastMember ? lastMember.getEnd() : target.getEnd() - 1;
    const text = declared.map(({ source }) => `${eol}${appendIndent}${reindent(source, appendIndent, eol)}`).join('');

    edits.push({ start: insertAt, end: insertAt, text });
  }

  return { edits, replacements, declared, carried };
}

function applyEdits(text: string, edits: Edit[]): string {
  return [...edits]
    .sort((a, b) => b.start - a.start)
    .reduce((acc, edit) => acc.slice(0, edit.start) + edit.text + acc.slice(edit.end), text);
}

//==================================================================================================

/**
 * Typechecks the emitted file on its own, against the environment a consumer will have: the
 * standard and DOM libs, and nothing else. The hand-written inputs cannot do this - their
 * cross-directory imports only resolve in the installed layout, which is why they carry
 * @ts-expect-error - so the merged file compiling standalone under strict is itself the evidence
 * the merge is sound.
 */
function typecheckEmitted(outPath: string): void {
  const program = ts.createProgram([outPath], {
    strict: true,
    noEmit: true,
    target: ts.ScriptTarget.ES2022,
    module: ts.ModuleKind.ESNext,
    moduleResolution: ts.ModuleResolutionKind.Bundler,
    lib: ['lib.es2022.d.ts', 'lib.dom.d.ts', 'lib.esnext.disposable.d.ts'],
    // Check our file but not TypeScript's own lib files. skipLibCheck would be wrong here: it skips
    // every .d.ts, including the one being checked.
    skipDefaultLibCheck: true,
    // No ambient @types. Otherwise whatever is installed alongside this script leaks in - @types/node
    // at least - and a node-only global like Buffer would pass here yet fail for a browser consumer.
    types: []
  });

  const diagnostics = ts.getPreEmitDiagnostics(program);

  if (diagnostics.length > 0) {
    const formatted = ts.formatDiagnosticsWithColorAndContext(diagnostics, {
      getCanonicalFileName: (f) => f,
      getCurrentDirectory: () => process.cwd(),
      getNewLine: () => '\n'
    });
    throw new MergeError(`The merged declarations do not typecheck:\n\n${formatted}`);
  }
}

function writeSnapshot(snapshotPath: string, plan: Plan): void {
  const { replacements, declared, carried } = plan;

  const lines: string[] = [
    '# Hand-written type surface',
    '',
    'Generated by `tools/merge-dts/merge-dts.ts`. Commit changes to this file alongside the edit that',
    'caused them - it is the reviewable record of how the published types differ from what Embind',
    'emits. Scoped to the hand-written surface only, so it stays stable across unrelated upstream CSP',
    'changes.',
    '',
    `## Replaced members (${replacements.length})`,
    '',
    'Signatures from `src/type-overlays`, refining declarations Embind generated too loosely.',
    ''
  ];

  for (const { memberName, before, after } of replacements) {
    lines.push(`### ${memberName}`, '', '```ts', `- ${before}`, `+ ${after}`, '```', '');
  }

  lines.push(
    `## Declared members (${declared.length})`,
    '',
    'Members from `src/js-declarations`, provided by JavaScript so Embind never sees them.',
    ''
  );
  for (const { memberName, from, source } of declared) {
    lines.push(`### ${memberName}`, '', `From \`${from}\`.`, '', '```ts', reindent(source, '', '\n'), '```', '');
  }

  lines.push(`## Standalone declarations (${carried.length})`, '');
  for (const { from, text } of carried) {
    lines.push(`### from ${from}`, '', '```ts', text, '```', '');
  }

  // Fragments arrive with whatever line endings their source files had; the snapshot uses one.
  mkdirSync(dirname(snapshotPath), { recursive: true });
  writeFileSync(snapshotPath, withEol(lines.join('\n'), '\n'), 'utf8');
}

//==================================================================================================

function main(): void {
  const args = parseArgs(process.argv.slice(2));
  const generatedPath = resolve(args.generated);
  const outPath = resolve(args.out);

  // Writing over the input would mean a re-run without an intervening rebuild merges into an
  // already-merged file, duplicating every carried declaration.
  if (outPath === generatedPath) {
    throw new MergeError(`--out must differ from --generated; both resolve to ${outPath}.`);
  }

  const project = new Project({ skipAddingFilesFromTsConfig: true, skipFileDependencyResolution: true });

  const sources = [
    readSource(project, 'overlay', resolve(args.overlays), 'TypeOverrides'),
    readSource(project, 'declaration', resolve(args.declarations), 'JsDeclarations')
  ];

  const generatedFile = project.addSourceFileAtPath(generatedPath);
  const generatedText = generatedFile.getFullText();
  // Match whatever Embind emitted rather than imposing our own, so the merged file stays
  // diffable against the generated one line for line.
  const eol = generatedText.includes('\r\n') ? '\r\n' : '\n';

  const plan = planEdits(generatedFile, sources, eol);

  const body = applyEdits(generatedText, plan.edits).trimEnd();
  const tail = plan.carried.map((entry) => `${eol}${withEol(entry.text, eol)}${eol}`).join('');
  const output = `${body}${eol}${tail}`;

  mkdirSync(dirname(outPath), { recursive: true });
  writeFileSync(outPath, output, 'utf8');

  typecheckEmitted(outPath);

  if (args.snapshot) {
    writeSnapshot(resolve(args.snapshot), plan);
  }

  console.log(
    `merge-dts: ${plan.replacements.length} replaced, ${plan.declared.length} declared, ` +
      `${plan.carried.length} standalone declaration(s) carried over.`
  );
  console.log(`merge-dts: wrote ${outPath}`);
}

try {
  main();
} catch (error) {
  if (error instanceof MergeError) {
    console.error(`\nmerge-dts: ${error.message}\n`);
    process.exit(1);
  }
  throw error;
}
