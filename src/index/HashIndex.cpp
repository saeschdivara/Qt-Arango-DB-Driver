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

#include "HashIndex.h"

#include <QtCore/QEventLoop>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkReply>

namespace arangodb
{
namespace index
{

class HashIndexPrivate
{
    public:
        QString id;
        Collection * collection;

        QStringList fields;
        bool isUnique;

        bool isNewlyCreated = false;

        // Error related
        bool hasError;
        int errorNumber;
        QString errorMessage;

        inline void clearError() {
            hasError = false;
            errorNumber = 0;
            errorMessage.clear();
        }
};

const QString HASH_INDEX_NAME = QStringLiteral("hash");

HashIndex::HashIndex(Collection * collection, QObject *parent) :
    QObject(parent),
    d_ptr(new HashIndexPrivate)
{
    d_ptr->clearError();
    d_ptr->collection = collection;
}

QString HashIndex::id() const
{
    Q_D(const HashIndex);
    return d->id;
}

QString HashIndex::name() const
{
    return HASH_INDEX_NAME;
}

Collection *HashIndex::collection() const
{
    Q_D(const HashIndex);
    return d->collection;
}

QByteArray HashIndex::toJson() const
{
    QJsonDocument doc;
    QJsonObject obj;

    obj.insert(QLatin1String("type"), name());
    //obj.insert(QLatin1String("size"), QJsonValue(size()));

    doc.setObject(obj);
    return doc.toJson();
}

void HashIndex::waitUntilReady()
{
    QEventLoop loop;
    QObject::connect( this, &HashIndex::ready, &loop, &QEventLoop::quit );
    QObject::connect( this, &HashIndex::error, &loop, &QEventLoop::quit );
    loop.exec();
}

void HashIndex::waitUntilDeleted()
{
    QEventLoop loop;
    QObject::connect( this, &HashIndex::deleted, &loop, &QEventLoop::quit );
    QObject::connect( this, &HashIndex::error, &loop, &QEventLoop::quit );
    loop.exec();
}

bool HashIndex::hasErrorOccurred() const
{
    Q_D(const HashIndex);
    return d->hasError;
}

int HashIndex::errorCode() const
{
    Q_D(const HashIndex);
    return d->errorNumber;
}

QString HashIndex::errorMessage() const
{
    Q_D(const HashIndex);
    return d->errorMessage;
}

bool HashIndex::isNewlyCreated() const
{
    Q_D(const HashIndex);
    return d->isNewlyCreated;
}

void HashIndex::save()
{
    Q_EMIT saveSignal(this);
}

void HashIndex::deleteInDatabase()
{
    Q_EMIT deleteSignal(this);
}

void HashIndex::_ar_saveRequestFinished()
{
    Q_D(HashIndex);

    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
    if ( !reply ) return;

    QByteArray replyContent = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(replyContent);
    QJsonObject obj = doc.object();

    if ( obj.isEmpty() ) return;

    bool hasError = obj.value("error").toBool(true);
    if ( hasError ) {
        d->hasError = true;
        d->errorNumber = obj.value("errorNum").toDouble(-1);
        d->errorMessage = obj.value("errorMessage").toString();

        Q_EMIT error();
    }
    else {
        d->id = obj.value("id").toString();
        d->isNewlyCreated = obj.value("isNewlyCreated").toBool();

        d->clearError();

        Q_EMIT ready();
    }
}

}
}
