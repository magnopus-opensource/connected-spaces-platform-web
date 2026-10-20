#!/usr/bin/env bash
# Runs the bindings tests for one browser as two separate Vitest runs for the type and unit tests,
# then the integration tests. Each run starts a fresh browser, which avoids running out of memory
# for WebKit and Firefox.
#
# Both runs happen even if the first fails, and the script fails if either does.
#
# Usage, from the tests directory: run-browser-tests.sh <chromium|webkit|firefox>

set -uo pipefail

browser="$1"
status=0

for projects in "types unit" "integration"; do
  args=()
  for project in $projects; do
    args+=(--project "$project")
  done

  # Create a separate section for each run in the GitHub Actions log
  echo "::group::Test - $projects ($browser)"
  npm run "test:$browser" -- "${args[@]}" || status=$?
  echo "::endgroup::"
done

exit $status
