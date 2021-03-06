#ifndef ARANGODBDRIVER_GLOBAL_H
#define ARANGODBDRIVER_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QtCore/QObject>

#ifdef AR_DEBUG_BUILD
    #include <QtCore/QDebug>
#endif

#if defined(ARANGODBDRIVER_LIBRARY)
#  define ARANGODBDRIVERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ARANGODBDRIVERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ARANGODBDRIVER_GLOBAL_H
