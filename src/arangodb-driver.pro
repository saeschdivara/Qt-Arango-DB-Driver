#-------------------------------------------------
#
# Project created by QtCreator 2013-04-06T13:55:09
#
#-------------------------------------------------

QT       += network

QT       -= gui

#QMAKE_CXXFLAGS += -std=c++11
CONFIG+=c++11

TARGET = arangodb-driver
TEMPLATE = lib

CONFIG(release, debug|release){
    TARGET += $$PWD/../../arangodb-driver-dist/release/arangodb-driver
    DEFINES += AR_RELEASE_BUILD
}
CONFIG(debug, debug|release){
    TARGET += $$PWD/../../arangodb-driver-dist/debug/arangodb-driver
    DEFINES += AR_DEBUG_BUILD
}

DEFINES += ARANGODBDRIVER_LIBRARY

SOURCES += \
    Document.cpp \
    Collection.cpp \
    Edge.cpp \
    QueryBuilder.cpp \
    QBSelect.cpp \
    QBCursor.cpp \
    index/CapIndex.cpp \
    QBSimpleSelect.cpp \
    index/HashIndex.cpp \
    index/SkipListIndex.cpp \
    index/AbstractIndex.cpp \
    index/GeoIndex.cpp \
    index/FulltextIndex.cpp \
    ArangoDBDriver.cpp \
    transaction/TransactionController.cpp \
    transaction/Transaction.cpp \
    transaction/TransactionOperation.cpp \
    Database.cpp \
    AbstractDbObject.cpp

HEADERS +=\
        arangodb-driver_global.h \
    Document.h \
    Collection.h \
    Edge.h \
    private/Document_p.h \
    QueryBuilder.h \
    QBSelect.h \
    QBCursor.h \
    index/CapIndex.h \
    QBSimpleSelect.h \
    index/HashIndex.h \
    index/SkipListIndex.h \
    index/AbstractIndex.h \
    index/private/AbstractIndex_p.h \
    index/private/HashIndex_p.h \
    index/GeoIndex.h \
    index/FulltextIndex.h \
    ArangoDBDriver.h \
    transaction/TransactionController.h \
    transaction/Transaction.h \
    transaction/TransactionOperation.h \
    Database.h \
    AbstractDbObject.h \
    private/AbstractDbObject_p.h
