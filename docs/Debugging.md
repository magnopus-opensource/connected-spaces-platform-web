# Debugging

Beleive it or not, this project does support native debugging from the browser, into the bindings, and all the way through to the underlying C++ library.

Browser debugging can be tricky, but we can get through it together.

## Setup

### Building

To begin with, you are going to need a debug version of CSP compiled for emscripten. If you are reading this at a time when the legacy build system still exists, make sure you are using the CMake build system.

> [!IMPORTANT]
>
> You must build the CSP library yourself. When building in debug mode, DWARF symbol paths will be injected into the binary. These paths are specific to _your local machine_.

Once you have that, go ahead and build the bindings in debug mode against the debug CSP lib. You'll know you've been succesful because the compiled WASM will be large. When I do this, it comes out at about 90MB. That's all the debug symbols.

### Browser Setup

Debugging is performed through the test suite. This project used a vitest test suite, using a headless puppeteer chromium browser to run the tests in the browser context. What we're going to do when debugging is launch a _headed_ browser, such that we can use the developer tools.

To get WASM debugging support, you're going to need to install the [C/C++ Devtools Support (DWARF) extension](https://chromewebstore.google.com/detail/cc++-devtools-support-dwa/pdcpmagijalfljmkmjngeonclgbbannb)

Once that is installed, the next step is to tell our test stack where to find it. You do this by setting the `CSP_DWARF_EXT` environment variable to the path of your extension. This path will look something like mine does : `"C:\Users\Elliot.Morris\AppData\Local\Google\Chrome\User Data\Profile 2\Extensions\pdcpmagijalfljmkmjngeonclgbbannb\0.2.5854.1_0\"`

> [!TIP]
>
> `Profile` here is the profile that you login to chrome with when you open the thing, in this case I installed the extension under the second profile
>
> The long character string is the extension ID. You can see it's the same as the ID of the extension on the chrome web store URL. I think this should be standard across installs, but do double check.
>
> The number at the end is just the version number, take whatever version you happen to have installed.

There's no harm setting this environment variable permenantly. On windows, I set it in `Documents/WindowsPowerShell/Microsoft.PowerShell_profile.ps1` with

```ps
$env:CSP_DWARF_EXT="C:\Users\Elliot.Morris\AppData\Local\Google\Chrome\User Data\Profile 2\Extensions\pdcpmagijalfljmkmjngeonclgbbannb\0.2.5854.1_0\"
```

On other platforms, you'll have your `.bashrc` files or equivilent.

> [!NOTE]
>
> The reason we do this is because the browser we're running isn't actually _our_ browser,
> it's the bundled chromium browser the comes with playwright. We're injecting the extension into the playwright browser.
>
> Arguably this is a bit hacky, reusing a chrome extension install like this.
> We might want to consider vendoring the extension (if licensing allows), or writing some sort of script to fetch the extension ad-hoc.

## Launching

With that setup, you should be good to go. Navigate to the `tests` directory and run

```
npm install # <-- if not done already
npm run test:debug
```

You should see a vitest window like below appear, and the tests should be executed.

### Debugging

A convenient way to trigger a breakpoint is to use the `debugger;` command inside one of the typescript tests, which avoids you scrambling to open the developer tools and set a breakpoint in time.

For example :

```ts
it("exports GetVersion", () => {
  debugger; // <-- Add this to break here.
  expect(typeof csp.GetVersion).toBe("function");
});
```

With this in place, launch the `test:debug` profile again,and open the developer tools with `F12`.

Navigate to the `sources` tab. Once execution has stopped at your breakpoint, you will be able to use `Ctrl-P` to search for sources. In this instance, search for `CSPFoundation.cpp`. If you can find it, then things are working out for you! Navigate into it, and set a breakpoint via clicking the gutter next to the static string construction. Click to continue execution, and if things have gone well, the debugger will break inside the C++, and you have a debugger setup, huzah!
