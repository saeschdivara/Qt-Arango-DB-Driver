#-------------------------------------------------
#
# Project created by QtCreator 2013-07-25T14:49:10
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_CollectionTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += tst_CollectionTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

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
