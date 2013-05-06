#include "Document.h"
#include "private/Document_p.h"
#include <QtDebug>

using namespace arangodb;

Document::Document(QObject *parent) :
    QObject(parent),
    d(new internal::DocumentPrivate)
{
}

Document::Document(internal::DocumentPrivate *privatePointer, QObject *parent) :
    QObject(parent),
    d(privatePointer)
{
}

Document::Document(internal::DocumentPrivate *privatePointer, QString collection, QObject *parent) :
    Document(privatePointer, parent)
{
    d->collectionName = collection;
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

bool Document::isCurrent()
{
    return d->isCurrent;
}

QByteArray Document::toJsonString() const
{
    QJsonDocument doc;

    if ( !isEveryAttributeDirty() ) {
            QJsonObject obj;

            obj.insert(internal::ID, obj.value(internal::ID));
            obj.insert(internal::KEY, obj.value(internal::KEY));
            obj.insert(internal::REV, obj.value(internal::REV));

            for( QString attribute : d->dirtyAttributes ) {
                    obj.insert(attribute, d->data[attribute]);
                }

            doc.setObject(obj);
        }
    else {
            doc.setObject(d->data);
        }

    return doc.toJson();
}

QString Document::docID() const
{
    return d->data.value(internal::ID).toString();
}

QString Document::key() const
{
    return d->data.value(internal::KEY).toString();
}

QString Document::rev() const
{
    return d->data.value(internal::REV).toString();
}

QString Document::collection() const
{
    return d->collectionName;
}

QString Document::errorMessage() const
{
    return d->errorMessage;
}

quint32 Document::errorCode()
{
    return d->errorCode;
}

quint32 Document::errorNumber()
{
    return d->errorNumber;
}

bool Document::hasErrorOccurred()
{
    return d->errorCode != 0;
}

void Document::set(const QString &key, QVariant data)
{
    d->dirtyAttributes.append(key);
    d->data.insert(key, QJsonValue::fromVariant(data));
    d->isDirty = true;
}

QVariant Document::get(const QString &key) const
{
    return d->data.value(key).toVariant();
}

QStringList Document::dirtyAttributes() const
{
    return d->dirtyAttributes;
}

bool Document::isEveryAttributeDirty() const
{
    int attributes = d->data.keys().size()-3;

    if ( d->data.contains("error") ) {
            attributes--;
        }

    return d->dirtyAttributes.size() >= attributes;
}

void Document::_ar_dataIsAvailable()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
    d->dirtyAttributes.clear();

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

            for ( auto key : obj.keys() ) {
                    d->data.insert(key, obj[key]);
                }

            emit ready();
        }
}

void Document::_ar_dataDeleted()
{
    emit dataDeleted();
}

void Document::_ar_dataUpdated()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    d->isReady = true;

    if (reply->hasRawHeader("etag") ) {
            QByteArray etag = reply->rawHeader("etag");
            etag.replace('"', ""); // comes from the rawHeader
            d->isCreated = true;
            d->isCurrent = ( etag == rev() );
        }
    else {
            d->isCreated = false;
            d->isCurrent = false;
        }

    emit ready();
}

void Document::save()
{
    if ( !d->isCreated || d->isDirty ) {
            d->isDirty = false;

            emit saveData(this);
        }
}

void Document::drop()
{
    if ( d->isCreated ) {
            d->isDirty = false;
            d->isCreated = false;

            emit deleteData(this);
        }
}

void Document::updateStatus()
{
    emit updateDataStatus(this);
}
