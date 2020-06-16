#!/bin/bash

alias cmake='/sw/packages/cmake3/bin/cmake'
CC=/opt/gcc-4.8.5/bin/gcc
CXX=/opt/gcc-4.8.5/bin/g++
#CXX=/opt/gcc-4.8.5/bin/c++

if [ -d build ] ; then
  rm -r build
fi
mkdir build
cd build

#cmake ../source
#cmake -C ../mysettings.cmake ../source
cmake -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX ../source
make

cd ../
