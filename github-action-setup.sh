#!/bin/bash
rm -rf tests
mkdir tests
cd tests
cmake .. -DBoost_INCLUDE_DIR=$BOOST_ROOT_1_72_0/include
make FA-Checker
