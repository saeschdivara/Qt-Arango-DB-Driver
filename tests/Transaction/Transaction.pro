#-------------------------------------------------
#
# Project created by QtCreator 2013-11-23T16:48:49
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

QMAKE_CXXFLAGS += -std=c++11

TARGET = tst_TransactionTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_TransactionTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

# Import libraries
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../arangodb-driver-dist/release/ -larangodb-driver
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../arangodb-driver-dist/debug/ -larangodb-driver

else:unix:CONFIG(debug, debug|release): {
LIBS += -L$$PWD/../../../arangodb-driver-dist/debug/ -larangodb-driver
DEPENDPATH += $$PWD/../../../arangodb-driver-dist/debug
}
else:unix:CONFIG(release, debug|release): {
LIBS += -L$$PWD/../../../arangodb-driver-dist/release/ -larangodb-driver
DEPENDPATH += $$PWD/../../../arangodb-driver-dist/release
}

# Add include paths
INCLUDEPATH += $$PWD/../../src
INCLUDEPATH += $$PWD/../../include
