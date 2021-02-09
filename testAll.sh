#!/bin/bash
rm -rf build
mkdir build
cd build
cmake -DBoost_INCLUDE_DIR=$BOOST_ROOT_1_72_0/include -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
./FaChecker ../problems.json
