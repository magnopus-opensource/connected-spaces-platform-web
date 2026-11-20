#!/usr/bin/env bash
# Installs the system packages Playwright's browsers need on Linux (mostly for WebKit and Firefox).
#
# They come from Ubuntu's package mirror, which is sometimes very slow. So the run-bindings-tests
# action caches the downloaded packages:
#   * If DEPS_DIR holds packages restored from the cache, they're installed from there without
#     contacting the mirror. If that fails, they're installed from the mirror as usual.
#   * Otherwise they're installed from the mirror, then the downloaded packages are copied to
#     DEPS_DIR and "cacheable=true" is written to the step's outputs, for the action to cache.
#
# apt is also set up to give up on stalled downloads sooner and retry failed ones.
#
# Settings, from environment variables (set by the run-bindings-tests action):
#   BROWSERS        Browsers to install the packages for, separated by spaces
#   DEPS_DIR        Folder the packages are cached in
#   DEPS_CACHE_HIT  true if DEPS_DIR was restored from the cache
#
# Usage, from the tests directory: install-playwright-deps.sh

set -euo pipefail

# Give up on a download that has stalled for 15 seconds and retry it (up to 5 times), rather than
# waiting on the mirror. Keep downloaded packages so they can be cached.
sudo tee /etc/apt/apt.conf.d/80-bindings-tests > /dev/null <<'EOF'
Acquire::Retries "5";
Acquire::http::Timeout "15";
Acquire::https::Timeout "15";
APT::Keep-Downloaded-Packages "true";
EOF

if [ "${DEPS_CACHE_HIT:-}" = true ] && compgen -G "$DEPS_DIR/*.deb" > /dev/null; then
  echo "::group::Install system packages from the cache"
  # --no-download makes this fail rather than fall back to the mirror if anything else is needed.
  if sudo apt-get install -y --no-install-recommends --no-download "$DEPS_DIR"/*.deb; then
    echo "::endgroup::"
    exit 0
  fi
  echo "::endgroup::"
  echo "::warning::Couldn't install the cached system packages for Playwright, installing them from the package mirror instead"
  # Finish configuring anything the failed install left half done, so it doesn't block the next one.
  sudo dpkg --configure -a || true
fi

echo "::group::Install system packages from the package mirror"
# Start from an empty package cache, so only the packages downloaded now are kept for caching.
sudo apt-get clean
npx playwright install-deps $BROWSERS
echo "::endgroup::"

# Where apt keeps downloaded packages (normally /var/cache/apt/archives/)
eval "$(apt-config shell apt_archives Dir::Cache::archives/d)"

mkdir -p "$DEPS_DIR"
rm -f "$DEPS_DIR"/*.deb
if compgen -G "${apt_archives%/}/*.deb" > /dev/null; then
  cp "${apt_archives%/}"/*.deb "$DEPS_DIR"/
  echo "Kept $(ls "$DEPS_DIR"/*.deb | wc -l) downloaded packages for caching"
  echo "cacheable=true" >> "$GITHUB_OUTPUT"
else
  echo "No packages were downloaded, so there's nothing to cache"
fi
