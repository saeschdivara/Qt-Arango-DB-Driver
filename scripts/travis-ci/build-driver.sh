#!/bin/bash

which clang++
clang --version

qmake -v

if [[ "$CXX" == "g++" ]]
then
  qmake ./src/arangodb-driver.pro -r -spec linux-g++ CONFIG+=debug CONFIG+=TRAVIS_CI_GCC
else
  qmake ./src/arangodb-driver.pro -r -spec linux-clang CONFIG+=debug
fi

make -j4
