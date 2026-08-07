import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';

/*
 * Demonstrates that the OpenUSD integration works inside the browser module.
 *
 * The documents are written into the module's virtual filesystem at runtime
 * rather than embedded at build time, because that is the shape real usage will
 * take: documents arrive over the network, get handed to USD, and USD resolves
 * the references between them itself.
 *
 * The three documents compose like this:
 *
 *   scene.usda ......... root layer
 *     |- subLayers ..... overrides.usda   (opinion on a prim inside the reference)
 *     |- /World/Reference -> references @./model.usda@</Model>
 *     |- /World/Payload   -> payload    @./model.usda@</Model>
 *
 * which is enough to exercise sublayers, references, payloads, and the relative
 * asset paths that Ar has to resolve between them.
 */

const DOCUMENT_DIR = '/documents';
const MODEL = `${DOCUMENT_DIR}/model.usda`;
const OVERRIDES = `${DOCUMENT_DIR}/overrides.usda`;
const SCENE = `${DOCUMENT_DIR}/scene.usda`;

// Referenced by scene.usda, and knows nothing about it. That is the point of a reference.
const MODEL_USDA = `#usda 1.0
(
    defaultPrim = "Model"
)

def Xform "Model"
{
    def Sphere "Ball"
    {
        double radius = 3
    }
}
`;

// A sublayer of scene.usda. Reaches inside the referenced subtree to restate the radius.
const OVERRIDES_USDA = `#usda 1.0

over "World"
{
    over "Reference"
    {
        over "Ball"
        {
            double radius = 7
        }
    }
}
`;

const SCENE_USDA = `#usda 1.0
(
    defaultPrim = "World"
    subLayers = [
        @./overrides.usda@
    ]
)

def Xform "World"
{
    def "Reference" (
        prepend references = @./model.usda@</Model>
    )
    {
    }

    def "Payload" (
        prepend payload = @./model.usda@</Model>
    )
    {
    }
}
`;

describe('OpenUSD', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();

    // Fail here rather than in a test, so a filesystem problem is not reported
    // as a composition problem.
    expect(csp.WriteUsdDocument(MODEL, MODEL_USDA)).toBe(true);
    expect(csp.WriteUsdDocument(OVERRIDES, OVERRIDES_USDA)).toBe(true);
    expect(csp.WriteUsdDocument(SCENE, SCENE_USDA)).toBe(true);
  });

  it('consumes a USD document', () => {
    expect(csp.UsdDefaultPrimPath(MODEL)).toBe('/Model');

    const prims = csp.UsdPrimPaths(MODEL, true).split('\n');
    expect(prims).toContain('/Model');
    expect(prims).toContain('/Model/Ball');

    expect(csp.UsdSphereRadius(MODEL, '/Model/Ball')).toBe(3);
  });

  it('composes a document that references other documents', () => {
    const prims = csp.UsdPrimPaths(SCENE, true).split('\n');

    // The subtree at /World/Reference exists only because the reference to
    // model.usda was resolved and composed in.
    expect(prims).toContain('/World/Reference/Ball');

    // overrides.usda is a sublayer of the root layer, and USD's strength
    // ordering puts the root layer stack above references - so its opinion wins
    // over the value model.usda states for the same prim.
    expect(csp.UsdSphereRadius(SCENE, '/World/Reference/Ball')).toBe(7);

    // Nothing overrides the payload's copy, so it still reads model.usda's value.
    // Same source document, two composition arcs, two different results.
    expect(csp.UsdSphereRadius(SCENE, '/World/Payload/Ball')).toBe(3);

    // Direct evidence that all three documents contributed, rather than the
    // numbers above happening to look right.
    const contributing = csp.UsdContributingDocuments(SCENE);
    expect(contributing).toContain('scene.usda');
    expect(contributing).toContain('overrides.usda');
    expect(contributing).toContain('model.usda');
  });

  it('defers payloads until they are loaded', () => {
    const unloaded = csp.UsdPrimPaths(SCENE, false).split('\n');
    const loaded = csp.UsdPrimPaths(SCENE, true).split('\n');

    // Both arcs point at the same document, but a reference composes eagerly...
    expect(unloaded).toContain('/World/Reference/Ball');

    // ...whereas a payload is deliberately left out until asked for. This is the
    // mechanism a web client would lean on to avoid pulling a whole scene.
    expect(unloaded).not.toContain('/World/Payload/Ball');
    expect(loaded).toContain('/World/Payload/Ball');
  });

  it('resolves an asset through Ar', () => {
    // Ar 2.0 splits resolution in two. First: what does this asset path mean,
    // written as it is inside scene.usda?
    const identifier = csp.ArCreateIdentifier('./model.usda', SCENE);
    expect(identifier).toContain('model.usda');

    // Second: where does that identifier actually live?
    const resolved = csp.ArResolve(identifier);
    expect(resolved).not.toBe('');

    // And Ar can hand back the bytes, which is how Sdf's file format plugins
    // read content. Getting a filename back would not have proved that.
    expect(csp.ArReadAssetHead(resolved, 5)).toBe('#usda');

    // An asset path with nothing behind it resolves to nothing, rather than
    // throwing across the wasm boundary.
    expect(csp.ArResolve(`${DOCUMENT_DIR}/missing.usda`)).toBe('');
  });
});
