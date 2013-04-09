#include "Document.h"
#include <QtDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtNetwork/QNetworkReply>

using namespace arangodb;

namespace internal {

class DocumentPrivate
{
    public:
        const QString ID  = QStringLiteral("_id");
        const QString KEY = QStringLiteral("_key");
        const QString REV = QStringLiteral("_rev");

        bool isReady = false;
        bool isDirty = false;
        bool isCreated = false;
        QJsonObject data;
        QString collectionName;
};

}

Document::Document(QObject *parent) :
    QObject(parent),
    d(new internal::DocumentPrivate)
{
}

Document::Document(QString collection, QObject *parent) :
    Document(parent)
{
    d->collectionName = collection;
}

Document::~Document()
{
    delete d;
}

bool Document::isReady()
{
    return d->isReady;
}

bool Document::isCreated()
{
    return d->isCreated;
}

QByteArray Document::toJsonString() const
{
    QJsonDocument doc;
    doc.setObject(d->data);
    return doc.toJson();
}

QString Document::docID() const
{
    return d->data.value(d->ID).toString();
}

QString Document::key() const
{
    return d->data.value(d->KEY).toString();
}

QString Document::rev() const
{
    return d->data.value(d->REV).toString();
}

QString Document::collection() const
{
    return d->collectionName;
}

void Document::set(const QString &key, QVariant data)
{
    d->data.insert(key, QJsonValue::fromVariant(data));
    d->isDirty = true;
}

QVariant Document::get(const QString &key) const
{
    return d->data.value(key).toVariant();
}

void Document::_ar_dataIsAvailable()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    d->data = QJsonDocument::fromJson(reply->readAll()).object();
    d->isReady = true;
    emit ready();
}

void Document::save()
{
    d->isDirty = false;
    emit saveData(this);
}
