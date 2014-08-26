Qt-Arango-DB-Driver

Qt-Arango-DB-Driver is as its name already says a driver for the ArangoDB
which uses the Qt framework and is written in C++. Since it uses new features
of the C++ language, you are required to have a C++11 compatible compiler.
In my case this is gcc 4.7.3 (64bit). Also in my testing I used only
Qt 5 and 5.1.

Now what is driver already able to do:

* Creating, getting and deleting Databases
* Creating, getting, modifing, deleting and updating of Document's
* Creating, getting, modifing, deleting and updating of Edge's
* Creating, getting and deleting Colllection's
* Creating from Collection's index's is possible
* The following index's can be created and deleted: cap, fulltext, geo, hash, skiplist
* Transactions are starting to get support
