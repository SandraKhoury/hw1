#!/bin/bash
rm -rf build
mkdir build
cd build
echo "S${BOOST_ROOT_1_72_0}E"
cmake .. -DBoost_INCLUDE_DIR=$BOOST_ROOT_1_72_0/include
cmake --build . --config Release
