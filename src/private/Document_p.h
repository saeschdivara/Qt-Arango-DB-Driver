#ifndef DOCUMENT_P_H
#define DOCUMENT_P_H

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtNetwork/QNetworkReply>

namespace internal {

class DocumentPrivate
{
    public:
        bool isReady = false;
        bool isDirty = false;
        bool isCreated = false;
        bool isCurrent = true;
        QJsonObject data;
        QString collectionName;

        QString errorMessage;
        quint32 errorCode = 0;
        quint32 errorNumber = 0;

        QStringList dirtyAttributes;

        inline void resetError() {
            errorMessage.clear();
            errorCode = 0;
            errorNumber = 0;
        }
};

const QString ID  = QStringLiteral("_id");
const QString KEY = QStringLiteral("_key");
const QString REV = QStringLiteral("_rev");

}

#endif // DOCUMENT_P_H
