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
#   TEST_BROWSER_DEBUG    true to log the browser pages connecting to Vitest, and which test files
#                         are sent to them and finish (default false), for diagnosing hangs.
#
# Usage, from the tests directory: run-browser-tests.sh <chromium|webkit|firefox>

set -uo pipefail

browser="$1"
timeout_minutes="${TEST_TIMEOUT_MINUTES:-10}"
browser_debug="${TEST_BROWSER_DEBUG:-false}"
status=0

if [ "${browser_debug,,}" = true ]; then
  # DEBUG turns on messages from the Node side of Vitest's browser mode.
  export DEBUG="vitest:browser:*"
  # VITEST_BROWSER_DEBUG turns on the browser side's messages but we don't use it currently (due to
  # an issue with browser pages sending messages before their connection to Vitest is open which
  # manifests as an InvalidStateError, especially in Firefox, seen with Vitest 5.1.0).
  # export VITEST_BROWSER_DEBUG=info
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
      echo "::error::The $projects tests in $browser didn't finish within the ${timeout_minutes}-minute time limit and were stopped. Set the BINDINGS_TEST_BROWSER_DEBUG variable to true to log which test files were sent to the browser and which finished."
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
