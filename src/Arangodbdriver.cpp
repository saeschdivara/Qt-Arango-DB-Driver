#include "Arangodbdriver.h"
#include <QtCore/QDebug>

Arangodbdriver::Arangodbdriver()
{
#ifdef AR_DEBUG_BUILD
    qDebug() << "debug";
#elif AR_RELEASE_BUILD
    qDebug() << "release";
#endif
}
