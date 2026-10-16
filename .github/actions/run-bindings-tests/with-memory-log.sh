#!/usr/bin/env bash
# Runs the given command while logging the runner's memory use every few seconds, so a test run
# that exhausts memory (e.g. the runner being shut down, or processes being killed) can be diagnosed
# from its log. Logs to the step's output as it goes, so it survives the runner going away.
#
# Each line shows overall memory and swap use, then the processes using the most memory, with
# processes of the same name added together (browsers run as several processes).
#
# Usage: with-memory-log.sh <command> [args...]
# Exits with the command's exit code. Linux only, elsewhere it just runs the command.

set -uo pipefail

if [ "${RUNNER_OS:-}" != "Linux" ]; then
  exec "$@"
fi

interval="${MEMORY_LOG_INTERVAL:-5}"

(
  while sleep "$interval"; do
    memory=$(free -m | awk '
      /^Mem:/  { printf "used %d MB, available %d MB", $3, $7 }
      /^Swap:/ { printf ", swap used %d MB", $3 }')
    top=$(ps -eo rss=,comm= | awk '
      { rss = $1; $1 = ""; sub(/^ /, ""); total[$0] += rss }
      END { for (name in total) printf "%d\t%s\n", total[name] / 1024, name }' |
      sort -rn | head -5 | awk -F'\t' '{ printf "%s%s %d MB", (NR > 1 ? ", " : ""), $2, $1 }')
    echo "[memory $(date +%H:%M:%S)] $memory | $top"
  done
) &
monitor=$!

# Stop the logging loop and whatever it's running (normally its sleep), so nothing is left behind
# holding the step's output open.
stop_monitor() {
  local children
  children=$(pgrep -P "$monitor" || true)
  kill "$monitor" 2>/dev/null
  if [ -n "$children" ]; then
    kill $children 2>/dev/null
  fi
  wait "$monitor" 2>/dev/null
}
trap stop_monitor EXIT

"$@"
