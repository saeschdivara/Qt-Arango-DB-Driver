#ifndef ABSTRACTDBOBJECT_P_H
#define ABSTRACTDBOBJECT_P_H

#include <QtCore/QString>

namespace arangodb
{

class AbstractDbObjectPrivate
{
    public:
        bool isReady = false;
        bool isDirty = false;
        bool isCreated = false;
        bool isCurrent = true;

        QString errorMessage;
        quint32 errorCode = 0;
        quint32 errorNumber = 0;

        inline void resetError() {
            errorMessage.clear();
            errorCode = 0;
            errorNumber = 0;
        }
};

}

#endif // ABSTRACTDBOBJECT_P_H
