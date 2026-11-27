# Upgrading CSP

This guide describes how to upgrade the version of CSP used by the bindings.

### Create an upgrade branch

Firstly, create a new branch from `main` for the CSP upgrade:

```bash
git checkout -b chore/csp-{X.X.X}
```

### Merge in `csp-development`

Any breaking changes in CSP itself will have been addressed by the CSP team in the `csp-development` branch. To include these changes we must merge `csp-development` into the upgrade branch.

```bash
git merge origin/csp-development
```

> [!NOTE]
> `csp-development` may include commits relating to CSP changes merged after the release has been published. It's important to be attentive to any commits that came in after the targeted CSP release and avoid merging them into the upgrade branch. Otherwise this can result in build errors and other issues.
>
> To avoid the need for this, the CSP upgrade should be carried out promptly after a CSP release.
>
> For example, if the latest commit on `csp-development` corresponds to a change in CSP made after the latest release, we can merge in `csp-development`, excluding the latest commit by doing the following:
>
> ```bash
> git merge origin/csp-development~1
> ```

### Update the pinned version of CSP

This repository pins to a specific version of CSP which is specified in [cmake/GetCSP.cmake](../cmake/GetCSP.cmake). We must set the version pin to the CSP version we're upgrading to:

```cmake
...
# Specify the CSP version we want to use.
set(CSP_TARGET_VERSION "X.X.X" CACHE STRING "The CSP release version to download")
...
```

This pin is also the CSP version the package reports to consumers (`cspVersion` from the `./version` entry point and in `package.json`). It automatically appears in the release notes too, so there is no need to update the version anywhere else.

> [!NOTE]
> `CSP_TARGET_VERSION` is a cache variable, so an existing build directory keeps its old value. Reconfigure with a fresh build directory (or `-DCSP_TARGET_VERSION=X.X.X`) to pick up the new pin locally.

### Make any additional bindings updates (optional)

While the breaking changes in the CSP release will have been accounted for by the CSP team in the `csp-development` branch, there may be other changes in the release that we want to update the bindings for.

> [!NOTE]
> This part is optional. In particular, we don't necessarily need to bind all the new CSP APIs that are added in a release if there is no immediate plan to use them.

### Create a PR to `main`

Next, create a PR to merge the upgrade branch into `main`.

### Release and publish

Once the PR is merged, we can create a new release of the web bindings and publish the NPM package.

To do this run the [Make Release](https://github.com/magnopus-opensource/connected-spaces-platform-web/actions/workflows/make-release.yml) GitHub actions workflow. Follow the same pattern as previous releases for the release tag and name.
