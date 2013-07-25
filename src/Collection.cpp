#include "Collection.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtNetwork/QNetworkReply>

namespace arangodb
{

class CollectionPrivate
{
    public:
        QString name;

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

Collection::Collection(QString name, QObject *parent) :
    QObject(parent),
    d_ptr(new CollectionPrivate)
{
    Q_D(Collection);
    d->name = name;
}

Collection::~Collection()
{
    delete d_ptr;
}

bool Collection::isReady()
{
    Q_D(Collection);
    return d->isReady;
}

bool Collection::isCreated()
{
    Q_D(Collection);
    return d->isCreated;
}

QString Collection::name() const
{
    Q_D(const Collection);
    return d->name;
}

QString Collection::errorMessage() const
{
    Q_D(const Collection);
    return d->errorMessage;
}

quint32 Collection::errorCode()
{
    Q_D(Collection);
    return d->errorCode;
}

quint32 Collection::errorNumber()
{
    Q_D(Collection);
    return d->errorNumber;
}

bool Collection::hasErrorOccurred()
{
    Q_D(Collection);
    return d->errorNumber != 0;
}

void Collection::_ar_dataIsAvailable()
{
    Q_D(Collection);
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();

    reply->disconnect(this, SLOT(_ar_dataIsAvailable()));
    bool hasError = obj.value("error").toBool();
    if ( hasError ) {
        d->errorMessage = obj.value("errorMessage").toString();
        d->errorNumber  = obj.value("errorNum").toVariant().toInt();
        d->errorCode    = obj.value("code").toVariant().toInt();
        emit error();
    }
    else {
        d->isReady = true;
        d->isCreated = true;
        d->isCurrent = true;

        d->resetError();

        emit ready();
    }
}

}
