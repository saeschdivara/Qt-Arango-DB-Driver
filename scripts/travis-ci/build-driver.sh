qmake -v
qmake ./src/arangodb-driver.pro -r -spec linux-$CXX CONFIG+=debug
make -j4
