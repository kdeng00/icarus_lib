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


## Getting started

Cloning repository

```BASH
git clone --recursive https://github.com/kdeng00/icarus_lib
cd icarus_lib
```

Bootstrapping vcpkg

```BASH
3rdparty/vcpkg/bootstrap.sh
```

Installing dependencies

```BASH
cd 3rdparty/vcpkg
3rdparty/vcpkg/vcpkg install nlohmann-json curl openssl cpr taglib jwt-cpp boost libmariadb catch2
```

Building library

```BASH
mkdir build
cd build
cmake ..
cmake --build .
```
