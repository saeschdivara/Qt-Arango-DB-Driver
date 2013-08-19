#!/bin/bash
qmake -v

if [[ "$CXX" == "g++" ]]
then
  qmake ./src/arangodb-driver.pro -r -spec linux-g++ CONFIG+=debug
else
  qmake ./src/arangodb-driver.pro -r -spec linux-clang CONFIG+=debug
fi

make -j4
