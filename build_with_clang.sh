#/bin/bash
if [ ! -d ".build_clang" ]; then
    mkdir -p .build_clang
fi

cd .build_clang
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cmake --build .  --parallel
