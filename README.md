# Open Space Simulator

Open Space Simulator is a 3D simulator developed in the Computer Graphics course at Uppsala University.

## Installation

We use [Conan](https://conan.io/) to manage dependencies.

*Note: If you prefer to use CLion, please use the Conan [plugin](https://plugins.jetbrains.com/plugin/11956-conan).*

Generate build files:

```bash
mkdir build && cd build
conan install ..
```

### Windows

Build release version:

```bash
cmake .. -G "Visual Studio 16"
cmake --build . --config Release
```

### Linux / Mac

Build release version:

```bash
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Usage

Make sure you execute the binary from the project root directory:

```bash
./build/bin/cg1_project
```

