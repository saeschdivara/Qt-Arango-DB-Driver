#!/bin/bash
qmake -v

if [[ "$CXX" == "g++" ]]
then
  qmake ./tests/Collection/Collection.pro -r -spec linux-g++
else
  qmake ./tests/Collection/Collection.pro -r -spec linux-clang
fi

make -j4
