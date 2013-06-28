#-------------------------------------------------
#
# Project created by QtCreator 2013-05-31T17:22:05
#
#-------------------------------------------------

QT       += testlib network

QT       -= gui

TARGET = tst_QueriesTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += tst_QueriesTest.cpp
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

INCLUDEPATH += $$PWD/../../src
