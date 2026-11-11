#!/usr/bin/env bash
# Runs the bindings tests for one browser as two separate Vitest runs for the type and unit tests,
# then the integration tests. Each run starts a fresh browser, which avoids running out of memory
# for WebKit and Firefox.
#
# Both runs happen even if the first fails, and the script fails if either does.
#
# Settings, from environment variables (set by the run-bindings-tests action):
#   TEST_TIMEOUT_MINUTES  How long each Vitest run may take before it's stopped (default 10). If the
#                         browser page freezes, Vitest's own timeouts can't fire, so without this a
#                         run that hangs would only end when the job times out.
#   TEST_BROWSER_DEBUG    true to log Vitest's progress through each test file (default false), for
#                         diagnosing hangs.
#
# Usage, from the tests directory: run-browser-tests.sh <chromium|webkit|firefox>

set -uo pipefail

browser="$1"
timeout_minutes="${TEST_TIMEOUT_MINUTES:-10}"
browser_debug="${TEST_BROWSER_DEBUG:-false}"
status=0

if [ "${browser_debug,,}" = true ]; then
  # VITEST_BROWSER_DEBUG turns on progress messages from the browser side of Vitest. Vitest also
  # uses it as the log level of the Vite server for the browser tests, so it must be a valid Vite
  # log level: "info" is the default, whereas "true" would hide that server's errors.
  # DEBUG turns on the Node side's messages about sending test files to the browser.
  export VITEST_BROWSER_DEBUG=info
  export DEBUG="vitest:browser:*"
fi

for projects in "types unit" "integration"; do
  args=()
  for project in $projects; do
    args+=(--project "$project")
  done

  # Create a separate section for each run in the GitHub Actions log
  echo "::group::Test - $projects ($browser)"
  # Stop the run if it exceeds the time limit, and kill it if it's still running a minute later.
  timeout --kill-after=1m "${timeout_minutes}m" npm run "test:$browser" -- "${args[@]}"
  run_status=$?
  echo "::endgroup::"

  case $run_status in
    124)
      echo "::error::The $projects tests in $browser didn't finish within the ${timeout_minutes}-minute time limit and were stopped. Set the BINDINGS_TEST_BROWSER_DEBUG variable to true to log Vitest's progress through each test file."
      ;;
    137)
      echo "::error::The $projects tests in $browser were killed. Either they didn't stop after timing out, or the runner ran out of memory."
      ;;
  esac
  if [ "$run_status" -ne 0 ]; then
    status=$run_status
  fi
done

exit $status
