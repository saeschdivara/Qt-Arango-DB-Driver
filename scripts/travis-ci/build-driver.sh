#!/bin/bash

qmake -v

mkdir ../arangodb-driver-dist
mkdir ../arangodb-driver-dist/debug
mkdir ../arangodb-driver-dist/release

if [[ "$CXX" == "g++" ]]
then
  sudo update-alternatives --install /usr/local/bin/gcc gcc /usr/bin/gcc-4.7 70 --slave /usr/local/bin/g++ g++ /usr/bin/g++-4.7

  qmake ./src/arangodb-driver.pro -r -spec linux-g++ CONFIG=DEBUG
else
  sudo update-alternatives --install /usr/local/bin/clang clang /usr/bin/clang 70 --slave /usr/local/bin/clang++ clang++ /usr/bin/clang++

  qmake ./src/arangodb-driver.pro -r -spec linux-clang CONFIG=DEBUG
fi

make -j6
