#!/bin/bash

qmake -v

cd ./tests/Collection

if [[ "$CXX" == "g++" ]]
then
  qmake ./Collection.pro -r -spec linux-g++
else
  qmake ./Collection.pro -r -spec linux-clang
fi

make -j6
