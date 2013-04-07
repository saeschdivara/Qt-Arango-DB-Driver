#-------------------------------------------------
#
# Project created by QtCreator 2013-04-06T14:32:04
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_StartTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_StartTest.cpp
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
