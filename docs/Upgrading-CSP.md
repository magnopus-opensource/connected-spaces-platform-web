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
> `csp-development` may include commits relating to CSP changes merged after the release has been published. It's important to be attentive to any commits that do not related to the release and avoid merging them into to the upgrade branch.
>
> To avoid the need for this, the CSP upgrade should be carried out promptly after a CSP release.
>
> For example, if the latest commit `csp-development` corresponds to a change in CSP made after the latest release, we can merge in `csp-development` excluding the latest commit by doing:
>
> ```bash
> git merge origin/csp-development~1
> ```

### Update the pinned version of CSP

This repository pins to a specific version of CSP which is specified in [cmake/GetCSP.cmake](../cmake/GetCSP.cmake). We must the set version pin to the CSP version we're upgrading to:

```cmake
...
# Specify the CSP version we want to use.
set(CSP_TARGET_VERSION "X.X.X" CACHE STRING "The CSP release version to download")
...
```

### Create a PR to `main`

Finally, we can create a PR to merge the upgrade branch into `main`. Once that is merged, we can create a new release of the web bindings and publish the NPM package.
