#/bin/bash
if [ ! -d ".build" ]; then
    mkdir -p .build
fi

cd .build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake   
cmake --build .
