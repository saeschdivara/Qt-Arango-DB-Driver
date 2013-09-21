#!/bin/bash

#ls -lh /usr/local/bin/gcc
#ls -lh /usr/local/bin/g++
#ls -lh /usr/local/bin/clang
#ls -lh /usr/local/bin/clang++

qmake -v

if [[ "$CXX" == "g++" ]]
then
  whereis gcc
  whereis g++

  gcc --version
  qmake ./src/arangodb-driver.pro -r -spec linux-g++ CONFIG+=debug CONFIG+=TRAVIS_CI_GCC
else
  whereis clang
  whereis clang++

  clang --version
  qmake ./src/arangodb-driver.pro -r -spec linux-clang CONFIG+=debug
fi

make -j4
