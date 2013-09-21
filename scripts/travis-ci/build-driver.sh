#!/bin/bash

qmake -v

cd ..
mkdir arangodb-driver-dist/
mkdir arangodb-driver-dist/release/
cd ./Qt-Arango-DB-Driver/src

if [[ "$CXX" == "g++" ]]
then
  sudo update-alternatives --install /usr/local/bin/gcc gcc /usr/bin/gcc-4.7 70 --slave /usr/local/bin/g++ g++ /usr/bin/g++-4.7

  qmake arangodb-driver.pro -r -spec linux-g++
else
  sudo update-alternatives --install /usr/local/bin/clang clang /usr/bin/clang 70 --slave /usr/local/bin/clang++ clang++ /usr/bin/clang++

  qmake arangodb-driver.pro -r -spec linux-clang
fi

make -j6

ls .
ls ..
ls ./../../arangodb-driver-dist/release/

cd ..
