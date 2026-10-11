# INDI Gersbach Roof Driver

`indi_gersbach_roof` is an INDI dome driver for the roof at Sternwarte Gersbach.
It exposes the roof as the INDI device `Dach Sternwarte Gersbach` in the
`Domes` group.

The driver connects to the roof controller through Snap7 and sends commands to a
Siemens S7-compatible PLC. It implements the INDI dome operations needed by this
installation:

- park and unpark the roof
- abort roof motion
- open and close the shutter
- perform a PLC handshake during connection

## Requirements

Build requirements:

- CMake 3.5 or newer
- a C++17-capable compiler
- INDI development headers and libraries
- libnova development headers and library
- zlib development headers and library
- GSL development headers and library
- Snap7 development header and library (`snap7.h` and `libsnap7`)

Runtime requirements:

- INDI server/client environment
- network access to the roof PLC
- Snap7-compatible PLC endpoint configured with the correct host address, local
  TSAP, and remote TSAP values in the INDI connection settings

If Snap7 is installed in a non-standard location, pass its root directory to
CMake:

```sh
cmake -S . -B build -DSNAP7_ROOT=/path/to/snap7
```

`LibSnap7_ROOT` is also supported.

## Building

```sh
cmake -S . -B build
cmake --build build
```

The build produces the `indi_gersbach_roof` executable and generates the INDI
driver XML from `indi_gersbach_roof.xml.cmake`.

## Installing

```sh
cmake --install build
```

Installation places the driver executable in `bin` and installs the generated
driver XML into the INDI data directory detected by CMake.
