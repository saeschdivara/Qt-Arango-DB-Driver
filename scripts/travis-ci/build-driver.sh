#!/bin/bash

qmake -v

if [[ "$CXX" == "g++" ]]
then
  g++ --version
  sudo update-alternatives --install /usr/local/bin/gcc gcc /usr/bin/gcc-4.7 70 --slave /usr/local/bin/g++ g++ /usr/bin/g++-4.7
  g++ --version

  qmake ./src/arangodb-driver.pro -r -spec linux-g++ CONFIG+=debug CONFIG+=TRAVIS_CI_GCC
else
  whereis clang
  whereis clang++

  ls -lh /usr/local/bin/clang
  ls -lh /usr/local/bin/clang++

  clang --version
  sudo update-alternatives --install /usr/local/bin/clang clang /usr/bin/gcc-4.7 70 --slave /usr/local/bin/g++ g++ /usr/bin/g++-4.7
  clang --version

  qmake ./src/arangodb-driver.pro -r -spec linux-clang CONFIG+=debug
fi

make -j4
