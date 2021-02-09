#!/bin/bash
echo "BOOTS ${BOOST_ROOT_1_72_0} END"
rm -rf build
mkdir build
cd build
cmake .. -DBoost_INCLUDE_DIR=$BOOST_ROOT_1_72_0/include
cmake --build . --config Release
./FaChecker ../problems.json
