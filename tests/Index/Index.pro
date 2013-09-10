#-------------------------------------------------
#
# Project created by QtCreator 2013-09-10T10:35:02
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_IndexTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += tst_IndexTest.cpp
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
INCLUDEPATH += $$PWD/../../include
