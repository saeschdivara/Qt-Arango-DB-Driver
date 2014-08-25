#-------------------------------------------------
#
# Project created by QtCreator 2013-04-06T13:55:09
#
#-------------------------------------------------

QT       += network

QT       -= gui

contains(CONFIG, TRAVIS_CI_GCC) {
    QMAKE_CXXFLAGS += -std=c++0x
}
else {
    QMAKE_CXXFLAGS += -std=c++11
}

macx {
    QMAKE_CXXFLAGS += -stdlib=libc++
    LIBS += -stdlib=libc++
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
}

TARGET = arangodb-driver
TEMPLATE = lib

CONFIG(release, debug|release):DEFINES += AR_RELEASE_BUILD
CONFIG(debug, debug|release):DEFINES += AR_DEBUG_BUILD

DEFINES += ARANGODBDRIVER_LIBRARY

SOURCES += Arangodbdriver.cpp \
    Document.cpp \
    Collection.cpp \
    Edge.cpp \
    QueryBuilder.cpp \
    QBSelect.cpp \
    QBCursor.cpp

HEADERS += Arangodbdriver.h\
        arangodb-driver_global.h \
    Document.h \
    Collection.h \
    Edge.h \
    private/Document_p.h \
    QueryBuilder.h \
    QBSelect.h \
    QBCursor.h
