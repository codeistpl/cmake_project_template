### This is a template project c++ project using cmake gtest and gmock, cmake, and conan

### build instructions
```
$ conan install . --output-folder=build --build=missing
$ cd build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```