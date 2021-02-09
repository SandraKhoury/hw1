#!/bin/bash
rm -rf build
mkdir build
cd build
cmake -DBoost_INCLUDE_DIR=/mnt/c/Boost/boost_1_74_0/boost_1_74_0 -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
./FaChecker ../problems.json
