# icarus_lib


## Built with

* C++ - With some C++ 17 features
* nlohmann json
* taglib
* boost
* cpr
* curl
* openssl
* jwt-cpp
* bcrypt
* libmariadb
* catch2
* soci


## Getting started

This library uses `vcpkg` for managing packages. There is a submodule within this repository that
references a vcpkg repository. If you don't specify your own `vcpkg` root path, toolchain file, and 
triplet then it will resort to the defaults.

Cloning repository

```BASH
git clone --recursive https://github.com/kdeng00/icarus_lib
cd icarus_lib
```

Bootstrapping vcpkg

```BASH
include/3rdparty/vcpkg/bootstrap.sh
```

Installing dependencies

```BASH
cd include/3rdparty/vcpkg
./vcpkg install nlohmann-json curl openssl cpr taglib jwt-cpp boost libmariadb catch2 soci
```

Building library

```BASH
mkdir build
cd build
cmake ..
cmake --build .
```
