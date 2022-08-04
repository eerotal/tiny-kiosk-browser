# tiny-kiosk-browser - A very tiny kiosk browser

As it's name implies, *tiny-kiosk-browser* is a very tiny browser intended for
kiosk usage. It's implemented using GTK+ 3.0 and WebKitGTK 2. The feature list
of the browser is short, but long enough for many kiosk or digital signage type
use cases.

## Features

- Display web pages in full screen without borders, loading bars, etc.
- Authenticate using HTTP Basic Authentication.
- Pass HTTP Basic Authentication credentials from the command line.
- Built with modern GTK+ 3.0 and WebKitGTK libraries.
- Runs even on a Raspberry Pi 1B, although rather slowly.

## Build time dependencies (on Debian)

The following dependencies are required for building

```
libgtk-3-dev
libwebkit2gtk-4.0-dev
```

In addition, the build system requires the usual things: *cmake* and *gcc*.

## How to build

Run the following commands:

```
cd ../tiny-kiosk-browser
mkdir Build
cd Build/
cmake ..
make
```

The binary is placed in `Build/tiny-kiosk-browser`.

## Examples

Show a help message.

```
./tiny-kiosk-browser --help
```

Load *https://example.com*.

```
./tiny-kiosk-browser --username admin --password admin --host example.com https://example.com
```

Load *https://example.com* and login using HTTP Basic Authentication.

```
./tiny-kiosk-browser --username admin --password admin --host example.com https://example.com
```

## License

This software is licensed under the BSD 3-clause license. See LICENSE.txt for
more details.
