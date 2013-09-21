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

#include "AbstractIndex.h"
#include "private/AbstractIndex_p.h"

#include <QtCore/QEventLoop>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtNetwork/QNetworkReply>

namespace arangodb
{
namespace index
{

AbstractIndex::~AbstractIndex()
{
    Q_D(AbstractIndex);
    delete d;
}

QString AbstractIndex::id() const
{
    Q_D(const AbstractIndex);
    return d->id;
}

Collection *AbstractIndex::collection() const
{
    Q_D(const AbstractIndex);
    return d->collection;
}

void AbstractIndex::waitUntilReady()
{
    QEventLoop loop;
    QObject::connect( this, &AbstractIndex::ready, &loop, &QEventLoop::quit );
    QObject::connect( this, &AbstractIndex::error, &loop, &QEventLoop::quit );
    loop.exec();
}

void AbstractIndex::waitUntilDeleted()
{
    QEventLoop loop;
    QObject::connect( this, &AbstractIndex::deleted, &loop, &QEventLoop::quit );
    QObject::connect( this, &AbstractIndex::error, &loop, &QEventLoop::quit );
    loop.exec();
}

bool AbstractIndex::hasErrorOccurred() const
{
    Q_D(const AbstractIndex);
    return d->hasError;
}

int AbstractIndex::errorCode() const
{
    Q_D(const AbstractIndex);
    return d->errorNumber;
}

QString AbstractIndex::errorMessage() const
{
    Q_D(const AbstractIndex);
    return d->errorMessage;
}

bool AbstractIndex::isNewlyCreated() const
{
    Q_D(const AbstractIndex);
    return d->isNewlyCreated;
}

void AbstractIndex::save()
{
    Q_EMIT saveSignal(this);
}

void AbstractIndex::deleteInDatabase()
{
    Q_EMIT deleteSignal(this);
}

void AbstractIndex::_ar_saveRequestFinished()
{
    Q_D(AbstractIndex);

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

void AbstractIndex::_ar_deleteRequestFinished()
{
    Q_D(AbstractIndex);
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();

    bool hasError = obj.value("error").toBool();
    if ( hasError ) {
        d->errorMessage = obj.value("errorMessage").toString();
        d->errorNumber  = obj.value("errorNum").toVariant().toInt();
        d->errorNumber  = obj.value("code").toVariant().toInt();
        Q_EMIT error();
    }
    else {
        Q_EMIT deleted();
    }
}

AbstractIndex::AbstractIndex(Collection * collection, AbstractIndexPrivate * d, QObject * parent) :
    QObject(parent),
    d_ptr(d)
{
    d->collection = collection;
    d->clearError();
}

}
}
