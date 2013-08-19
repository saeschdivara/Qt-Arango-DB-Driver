qmake -v
qmake ./src/arangodb-driver.pro -r -spec linux-g++ CONFIG+=debug
make -j4
