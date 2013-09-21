#!/bin/bash
qmake -v

if [[ "$CXX" == "g++" ]]
then
  qmake ./tests/Collection/Collection.pro -r -spec linux-g++ CONFIG+=debug
else
  qmake ./tests/Collection/Collection.pro -r -spec linux-clang CONFIG+=debug
fi

make -j4
