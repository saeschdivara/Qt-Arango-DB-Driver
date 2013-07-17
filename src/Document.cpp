#include "Document.h"
#include "private/Document_p.h"
#include <QtCore/QCoreApplication>
#include <QtDebug>

using namespace arangodb;

Document::Document(QObject *parent) :
    QObject(parent),
    d_ptr(new internal::DocumentPrivate)
{
}

Document::Document(internal::DocumentPrivate *privatePointer, QObject *parent) :
    QObject(parent),
    d_ptr(privatePointer)
{
}

Document::Document(internal::DocumentPrivate *privatePointer, QString collection, QObject *parent) :
    Document(privatePointer, parent)
{
    d_func()->collectionName = collection;
}

Document::Document(QString collection, QObject *parent) :
    Document(new internal::DocumentPrivate, collection, parent)
{
}

Document::Document(QString collection, QString key, QObject *parent) :
    Document(collection, parent)
{
    set(internal::KEY, key);
}

Document::Document(QJsonObject obj, QObject * parent) :
    Document(new internal::DocumentPrivate, parent)
{
    obj.remove(QStringLiteral("n"));
    d_func()->collectionName = obj.value(internal::ID).toString().split('/').at(0);
    d_func()->data.insert(internal::ID, obj.value(internal::ID));
    d_func()->data.insert(internal::KEY, obj.value(internal::KEY));
    d_func()->data.insert(internal::REV, obj.value(internal::REV));
    d_func()->isCurrent = false;
}

Document::~Document()
{
    delete d_ptr;
}

bool Document::isReady()
{
    return d_func()->isReady;
}

bool Document::isCreated()
{
    return d_func()->isCreated;
}

bool Document::isCurrent()
{
    return d_func()->isCurrent;
}

QByteArray Document::toJsonString() const
{
    QJsonDocument doc;

    if ( !isEveryAttributeDirty() ) {
        QJsonObject obj;

        obj.insert(internal::ID, obj.value(internal::ID));
        obj.insert(internal::KEY, obj.value(internal::KEY));
        obj.insert(internal::REV, obj.value(internal::REV));

        for( QString attribute : d_func()->dirtyAttributes ) {
            obj.insert(attribute, d_func()->data[attribute]);
        }

        doc.setObject(obj);
    }
    else {
        doc.setObject(d_func()->data);
    }

    return doc.toJson();
}

QString Document::docID() const
{
    return d_func()->data.value(internal::ID).toString();
}

QString Document::key() const
{
    return d_func()->data.value(internal::KEY).toString();
}

QString Document::rev() const
{
    return d_func()->data.value(internal::REV).toString();
}

QString Document::collection() const
{
    return d_func()->collectionName;
}

QString Document::errorMessage() const
{
    return d_func()->errorMessage;
}

quint32 Document::errorCode()
{
    return d_func()->errorCode;
}

quint32 Document::errorNumber()
{
    return d_func()->errorNumber;
}

bool Document::hasErrorOccurred()
{
    return d_func()->errorCode != 0;
}

void Document::set(const QString &key, QVariant data)
{
    d_func()->dirtyAttributes.append(key);
    d_func()->data.insert(key, QJsonValue::fromVariant(data));
    d_func()->isDirty = true;
}

QVariant Document::get(const QString &key) const
{
    return d_func()->data.value(key).toVariant();
}

bool Document::contains(const QString & key) const
{
    return d_func()->data.contains(key);
}

QStringList Document::dirtyAttributes() const
{
    return d_func()->dirtyAttributes;
}

bool Document::isEveryAttributeDirty() const
{
    int attributes = d_func()->data.keys().size()-3;

    if ( d_func()->data.contains("error") ) {
        attributes--;
    }

    return d_func()->dirtyAttributes.size() >= attributes;
}

void Document::waitForResult()
{
    bool b = true;
    QMetaObject::Connection conn1 = QObject::connect(this, &Document::ready, [&] {
        b = false;
    });
    QMetaObject::Connection conn2 = QObject::connect(this, &Document::error, [&] {
        b = false;
    });
    while (b) {
        qApp->processEvents();
    }
    QObject::disconnect(conn1);
    QObject::disconnect(conn2);

}

void Document::_ar_dataIsAvailable()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
    d_func()->dirtyAttributes.clear();

    reply->disconnect(this, SLOT(_ar_dataIsAvailable()));

    bool hasError = obj.value("error").toBool();
    if ( hasError ) {
        d_func()->errorMessage = obj.value("errorMessage").toString();
        d_func()->errorNumber  = obj.value("errorNum").toVariant().toInt();
        d_func()->errorCode    = obj.value("code").toVariant().toInt();
        emit error();
    }
    else {
        d_func()->isReady = true;
        d_func()->isCreated = true;
        d_func()->isCurrent = true;

        for ( auto key : obj.keys() ) {
            d_func()->data.insert(key, obj[key]);
        }

        d_func()->resetError();

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
    d_func()->isReady = true;

    if (reply->hasRawHeader("etag") ) {
        QByteArray etag = reply->rawHeader("etag");
        etag.replace('"', ""); // comes from the rawHeader
        d_func()->isCreated = true;
        d_func()->isCurrent = ( etag == rev() );
    }
    else {
        d_func()->isCreated = false;
        d_func()->isCurrent = false;
    }

    emit ready();
}

void Document::save()
{
    if ( !d_func()->isCreated || d_func()->isDirty ) {
        d_func()->isDirty = false;

        emit saveData(this);
    }
}

void Document::sync()
{
    if ( !d_func()->isCurrent ) {
        emit syncData(this);
    }
}

void Document::drop()
{
    if ( d_func()->isCreated ) {
        d_func()->isDirty = false;
        d_func()->isCreated = false;

        emit deleteData(this);
    }
}

void Document::updateStatus()
{
    emit updateDataStatus(this);
}
