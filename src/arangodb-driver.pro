#-------------------------------------------------
#
# Project created by QtCreator 2013-04-06T13:55:09
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = arangodb-driver
TEMPLATE = lib

CONFIG(release, debug|release):DEFINES += AR_RELEASE_BUILD
CONFIG(debug, debug|release):DEFINES += AR_DEBUG_BUILD

DEFINES += ARANGODBDRIVER_LIBRARY

SOURCES += Arangodbdriver.cpp

HEADERS += Arangodbdriver.h\
        arangodb-driver_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
