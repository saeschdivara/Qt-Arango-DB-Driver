/********************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2013 Sascha Ludwig Häusler
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy of
 ** this software and associated documentation files (the "Software"), to deal in
 ** the Software without restriction, including without limitation the rights to
 ** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 ** the Software, and to permit persons to whom the Software is furnished to do so,
 ** subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in all
 ** copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 ** FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 ** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 ** IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 ** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *********************************************************************************/

#include "Collection.h"

#include "Arangodbdriver.h"
#include "Document.h"

#include <QtCore/QCoreApplication>
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

        QString id;
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

QString Collection::id() const
{
    Q_D(const Collection);
    return d->id;
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

Collection::KeyOption *Collection::keyOption() const
{
    Q_D(const Collection);
    return d->keyOption;
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

Document *Collection::createDocument()
{
    Q_D(Collection);

    Arangodbdriver * driver = Q_NULLPTR;
    if ( (driver = qobject_cast<Arangodbdriver *>(parent())) ) {
        return driver->createDocument(d->name);
    }
    else {
        return new Document(d->name);
    }
}

Document *Collection::createDocument(const QString & key)
{
    Q_D(Collection);

    Arangodbdriver * driver = Q_NULLPTR;
    if ( (driver = qobject_cast<Arangodbdriver *>(parent())) ) {
        return driver->createDocument(d->name, key);
    }
    else {
        return new Document(d->name, key);
    }
}

void Collection::save()
{
    Q_EMIT saveData(this);
}

void Collection::deleteAll()
{
    Q_EMIT deleteData(this);
}

void Collection::waitUntilReady()
{
    bool b = true;
    QMetaObject::Connection conn1 = QObject::connect(this, &Collection::ready, [&] {
        b = false;
    });
    QMetaObject::Connection conn2 = QObject::connect(this, &Collection::error, [&] {
        b = false;
    });
    while (b) {
        qApp->processEvents();
    }
    QObject::disconnect(conn1);
    QObject::disconnect(conn2);
}

void Collection::waitUntilDeleted()
{
    bool b = true;
    QMetaObject::Connection conn1 = QObject::connect(this, &Collection::deleted, [&] {
        b = false;
    });
    QMetaObject::Connection conn2 = QObject::connect(this, &Collection::error, [&] {
        b = false;
    });
    while (b) {
        qApp->processEvents();
    }
    QObject::disconnect(conn1);
    QObject::disconnect(conn2);
}

QByteArray Collection::toJsonString()
{
    Q_D(Collection);

    QJsonObject obj;
    obj.insert(QStringLiteral("name"), d->name);
    obj.insert(QStringLiteral("waitForSync"), d->waitForSync);
    if (d->journalSize > 0 ) obj.insert(QStringLiteral("journalSize"), d->journalSize);
    obj.insert(QStringLiteral("isSystem"), d->isSystem);
    obj.insert(QStringLiteral("isVolatile"), d->isVolatile);
    if ( d->keyOption != Q_NULLPTR ) obj.insert(QStringLiteral("keyOption"), d->keyOption->object());
    obj.insert(QStringLiteral("type"), int(d->type));

    QJsonDocument doc;
    doc.setObject(obj);
    return doc.toJson();
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

        d->id           = obj.value(QStringLiteral("id")).toBool();
        d->waitForSync  = obj.value(QStringLiteral("waitForSync")).toBool();
        d->journalSize  = obj.value(QStringLiteral("journalSize")).toDouble();
        d->isSystem     = obj.value(QStringLiteral("isSystem")).toBool();
        d->isVolatile   = obj.value(QStringLiteral("isVolatile")).toBool();

        emit ready();
    }
}

void Collection::_ar_isDeleted()
{
    Q_D(Collection);
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();

    bool hasError = obj.value("error").toBool();
    if ( hasError ) {
        d->errorMessage = obj.value("errorMessage").toString();
        d->errorNumber  = obj.value("errorNum").toVariant().toInt();
        d->errorCode    = obj.value("code").toVariant().toInt();
        Q_EMIT error();
    }
    else {
        Q_EMIT deleted();
    }
}

}
