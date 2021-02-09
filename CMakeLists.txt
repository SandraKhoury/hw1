cmake_minimum_required(VERSION 3.10.0)
project (FA-Checker LANGUAGES CXX)
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_BUILD_TYPE "Release")
#after cmake ..
#cmake --build . --config Release 
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
 SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/Boost/boost_1_74_0/boost_1_74_0/")
 # for linux pass the path to Boost 
 # on the command line -DBoost_INCLUDE_DIR=path
# ELSE()

# SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/mnt/c/Boost/boost_1_74_0/boost_1_74_0/")
 ENDIF()
find_package(Boost 1.72.0) 

add_executable(FaChecker FaChecker.cpp readFA.cpp utils.cpp)
target_include_directories(FaChecker PUBLIC "${PROJECT_SOURCE_DIR}")
include_directories(${Boost_INCLUDE_DIRS}) 
