#include "Collection.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtNetwork/QNetworkReply>

namespace arangodb
{

class CollectionPrivate
{
    public:
        bool isReady = false;
        bool isDirty = false;
        bool isCreated = false;
        bool isCurrent = true;

        QString name;
        bool waitForSync;
        int journalSize;
        bool isSystem;
        bool isVolatile;
        Collection::KeyOption * keyOption;
        Collection::Type type;

        QString errorMessage;
        quint32 errorCode = 0;
        quint32 errorNumber = 0;

        inline void resetError() {
            errorMessage.clear();
            errorCode = 0;
            errorNumber = 0;
        }
};

Collection::Collection(const QString & name, QObject * parent) :
    QObject(parent),
    d_ptr(new CollectionPrivate)
{
    Q_D(Collection);
    d->name = name;
}

Collection::Collection(const QString & name, bool waitForSync, int journalSize, bool isSystem, bool isVolatile, KeyOption * keyOption, Type type, QObject *parent) :
    QObject(parent),
    d_ptr(new CollectionPrivate)
{
    Q_D(Collection);
    d->name = name;
    d->waitForSync = waitForSync;
    d->journalSize = journalSize;
    d->isSystem = isSystem;
    d->isVolatile = isVolatile;
    d->keyOption = keyOption;
    d->type = type;
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

bool Collection::isWaitingForSync()
{
    Q_D(Collection);
    return d->waitForSync;
}

bool Collection::isSystem()
{
    Q_D(Collection);
    return d->isSystem;
}

bool Collection::isVolatile()
{
    Q_D(Collection);
    return d->isVolatile;
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

void Collection::save()
{
    //Q_UNIMPLEMENTED;
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
