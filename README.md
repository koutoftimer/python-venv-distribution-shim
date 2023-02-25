# Why?

Lets suppose you have python package with an entry point you wish to wrap into
an installer for windows. Lets suppose you have tried `pyinstaller`, `py2exe`
and whatever else you have found, but nothing helps because of one or another
reason. Lets suppose that you are actually able to `pip install` a package with
an entry point into virtual environment. This means that you can ship `venv`
folder as an application itself.

Well, not yet. If you want to provide "nice" user experience for console
application you don't want to pollute `PATH` with random executables your
package needs, as well as `python`, `pip` and whatever goes into `Scripts`
folder. But you want your package's entry point to be visible in the `PATH`.

# What is it?

This is just a tiny little shim written in `C` that produces an executable that
"redirect" calls to underlying executable inside `Scripts` folder.

# Usage

Build `shim.exe` (if you don't know how, install Code Blocks, open `shim.c` and
just press <kbd>Ctrl</kbd>+<kbd>F9</kbd>)

Name of exact executable is determined by the name of the shim itself. So you
have to copy `shim.exe` into `venv` folder with the name of an executable
inside `Scripts` folder which you would like to target.

Than you can add `venv` into the `PATH` environment variable and use shim like
regular console application:

```console
$ <app-name> arg1 arg2 [...]
```

Will figure out its location as `path\to\venv`, name as `app-name` and execute

```console
$ <path\to\venv\Scripts\app-name.exe> arg1 arg2 [...]
```

`[...]` is just whatever arguments you will provide in the command line.
