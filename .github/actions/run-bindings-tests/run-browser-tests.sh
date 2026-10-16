#!/usr/bin/env bash
# Runs the bindings tests in one browser as two separate Vitest runs: the type and unit tests, then
# the integration tests. Each run starts a fresh browser.
#
# WebKit and Firefox keep the memory of every test file they've run until the browser closes, so
# running all the tests in one browser ran the runner out of memory with the debug build. Splitting
# the run releases what the unit tests built up before the integration tests start.
#
# Both runs happen even if the first fails, and the script fails if either does. Memory use is
# logged throughout (see with-memory-log.sh).
#
# Usage, from the tests directory: run-browser-tests.sh <chromium|webkit|firefox>

set -uo pipefail

browser="$1"
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
status=0

for projects in "types unit" "integration"; do
  args=()
  for project in $projects; do
    args+=(--project "$project")
  done

  echo "=== Running the $projects tests in $browser"
  bash "$script_dir/with-memory-log.sh" npm run "test:$browser" -- "${args[@]}" || status=$?
done

exit $status
