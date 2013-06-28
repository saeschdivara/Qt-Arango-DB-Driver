#ifndef QCURSOR_H
#define QCURSOR_H

#include "arangodb-driver_global.h"

namespace arangodb
{

class ARANGODBDRIVERSHARED_EXPORT QBCursor : public QObject
{
        Q_OBJECT
    public:
        QBCursor(QObject *parent = 0);
};

}

#endif // QCURSOR_H
