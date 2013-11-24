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

#include "ArangoDBDriver.h"

#include <transaction/TransactionController.h>

#include <QtCore/QBuffer>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QMetaMethod>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <memory>

namespace internal {

class ArangoDBDriverPrivate
{
    public:
        QString protocol;
        QString host;
        qint32 port;
        QNetworkAccessManager networkManager;

        QString standardUrl;

        QByteArray jsonData;
        QBuffer data;

        void createStandardUrl() {
            standardUrl = protocol + QString("://") + host + QString(":") + QString::number(port) + QString("/_api");
        }
};

}

namespace arangodb
{

ArangoDBDriver::ArangoDBDriver(QString protocol, QString host, qint32 port) :
    d(new internal::ArangoDBDriverPrivate)
{
    d->protocol = protocol;
    d->host = host;
    d->port = port;

    d->createStandardUrl();
}

ArangoDBDriver::~ArangoDBDriver()
{
    delete d;
}

bool ArangoDBDriver::isColllectionExisting(const QString & collectionName)
{
    QUrl url(d->standardUrl + QString("/collection/") + collectionName);
    QNetworkReply *reply = d->networkManager.get(QNetworkRequest(url));

    bool isWaiting = true;

    QMetaObject::Connection connection = connect(reply, &QNetworkReply::finished,
                                                 [&isWaiting] {
        isWaiting = false;
    });

    while (isWaiting) {
        qApp->processEvents();
    }

    disconnect(connection);

    return reply->error() == QNetworkReply::NoError;
}

Collection *ArangoDBDriver::getCollection(QString name)
{
    Collection *collection = new Collection(name, this);

    QUrl url(d->standardUrl + QString("/collection/") + name);
    QNetworkReply *reply = d->networkManager.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished,
            collection, &Collection::_ar_dataIsAvailable
            );

    connectCollection(collection);

    return collection;
}

Collection *ArangoDBDriver::createCollection(const QString & name, Collection::Type type, bool waitForSync, int journalSize, bool isSystem, bool isVolatile, Collection::KeyOption * keyOption)
{
    Collection *collection = new Collection(name,
                                            waitForSync,
                                            journalSize,
                                            isSystem,
                                            isVolatile,
                                            keyOption,
                                            type,
                                            this);

    connectCollection(collection);
    return collection;
}

Collection *ArangoDBDriver::createCollection(const QString &name, Collection::CreateOptions options)
{
    Collection * collection = new Collection(name, options, this);
    connectCollection(collection);
    return collection;
}

Collection::CreateOptions ArangoDBDriver::defaultCollectionCreationOptions()
{
    Collection::CreateOptions options;
    // Default values
    options.waitForSync = false;
    options.isSystem = false;
    options.isVolatile = false;
    options.journalSize = -1;
    options.type = Collection::Type::DocumentType;
    options.keyOption = Q_NULLPTR;

    return options;
}

void ArangoDBDriver::connectCollection(Collection * collection)
{
    connect( collection, &Collection::saveData,
             this, &ArangoDBDriver::_ar_collection_save
             );
    connect( collection, &Collection::loadIntoMemory,
             this, &ArangoDBDriver::_ar_collection_save
             );
    connect( collection, &Collection::deleteData,
             this, &ArangoDBDriver::_ar_collection_delete
             );
}

Document *ArangoDBDriver::getDocument(QString id)
{
    Document *doc = new Document(this);

    QUrl url(d->standardUrl + QString("/document/") + id);
    QNetworkReply *reply = d->networkManager.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished,
            doc, &Document::_ar_dataIsAvailable
            );

    connectDocument(doc);

    return doc;
}

Document *ArangoDBDriver::createDocument(QString collection)
{
    Document *doc = new Document(collection, this);
    connectDocument(doc);

    return doc;
}

Document *ArangoDBDriver::createDocument(QString collection, QString key)
{
    Document *doc = new Document(collection, key, this);
    connectDocument(doc);

    return doc;
}

void ArangoDBDriver::connectDocument(Document * doc)
{
    connect(doc, &Document::saveData,
            this, &ArangoDBDriver::_ar_document_save
            );

    connect(doc, &Document::deleteData,
            this, &ArangoDBDriver::_ar_document_delete
            );

    connect(doc, &Document::updateDataStatus,
            this, &ArangoDBDriver::_ar_document_updateStatus
            );

    connect(doc, &Document::updateData,
            this, &ArangoDBDriver::_ar_document_update
            );

    connect(doc, &Document::syncData,
            this, &ArangoDBDriver::_ar_document_sync
            );
}

Edge *ArangoDBDriver::getEdge(QString id)
{
    Edge *e = new Edge(this);

    QUrl url(d->standardUrl + QString("/edge/") + id);
    QNetworkReply *reply = d->networkManager.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished,
            e, &Document::_ar_dataIsAvailable
            );

    connect(e, &Edge::saveData,
            this, &ArangoDBDriver::_ar_edge_save
            );

    connect(e, &Edge::deleteData,
            this, &ArangoDBDriver::_ar_edge_delete
            );

    connect(e, &Edge::updateDataStatus,
            this, &ArangoDBDriver::_ar_document_updateStatus
            );

    connect(e, &Edge::syncData,
            this, &ArangoDBDriver::_ar_document_sync
            );

    return e;
}

Edge *ArangoDBDriver::createEdge(QString collection, Document *fromDoc, Document *toDoc)
{
    Edge *e = new Edge(collection, fromDoc, toDoc, this);

    connect(e, &Edge::saveData,
            this, &ArangoDBDriver::_ar_edge_save
            );

    connect(e, &Edge::deleteData,
            this, &ArangoDBDriver::_ar_edge_delete
            );

    connect(e, &Edge::updateDataStatus,
            this, &ArangoDBDriver::_ar_document_updateStatus
            );

    connect(e, &Edge::syncData,
            this, &ArangoDBDriver::_ar_document_sync
            );

    return e;
}

void ArangoDBDriver::connectIndex(index::AbstractIndex * index)
{

    QObject * obj = dynamic_cast<QObject *>(index);

    connect( obj, SIGNAL(saveSignal(AbstractIndex*)),
             this,  SLOT(_ar_index_save(AbstractIndex*))
             );
}

QSharedPointer<QBCursor> ArangoDBDriver::executeSelect(QSharedPointer<QBSelect> select)
{
    QSharedPointer<QBCursor> cursor(new QBCursor(this));

    QByteArray jsonSelect = select->toJson();
    QUrl url(d->standardUrl + QString("/cursor"));
    QNetworkRequest request(url);
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(jsonSelect.size()));

    QNetworkReply *reply = d->networkManager.post(request, jsonSelect);

    connect(reply, &QNetworkReply::finished,
            cursor.data(), &QBCursor::_ar_cursor_result_loaded
            );

    return cursor;
}

QSharedPointer<QBCursor> ArangoDBDriver::executeSelect(QSharedPointer<QBSimpleSelect> select)
{
    QSharedPointer<QBCursor> cursor(new QBCursor(this));

    QByteArray jsonSelect = select->toJson();
    QUrl url(d->standardUrl + select->url());
    QNetworkRequest request(url);
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(jsonSelect.size()));

    QNetworkReply *reply = d->networkManager.put(request, jsonSelect);

    connect(reply, &QNetworkReply::finished,
            cursor.data(), &QBCursor::_ar_cursor_result_loaded
            );

    return cursor;
}

void ArangoDBDriver::loadMoreResults(QBCursor * cursor)
{
    QUrl url(d->standardUrl + QString("/cursor/") + cursor->id());
    QNetworkRequest request(url);
    request.setRawHeader("Content-Type", "application/json");

    QNetworkReply *reply = d->networkManager.put(request, QByteArrayLiteral(""));

    connect(reply, &QNetworkReply::finished,
            cursor, &QBCursor::_ar_cursor_result_loaded
            );
}

void ArangoDBDriver::connectTransactionController(TransactionController *ctrl)
{
    connect( ctrl, &TransactionController::transactionCommittedSignal,
             this, &ArangoDBDriver::_ar_transaction_commit
             );
}

void ArangoDBDriver::_ar_document_save(Document *doc)
{
    d->jsonData = doc->toJsonString();
    QByteArray jsonDataSize = QByteArray::number(d->jsonData.size());

    if ( doc->isCreated() ) {
        QUrl url(d->standardUrl + QString("/document/") + doc->docID());
        QNetworkRequest request(url);
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("Content-Length", jsonDataSize);

        QNetworkReply *reply = Q_NULLPTR;

        if ( doc->isEveryAttributeDirty() ) {
            reply = d->networkManager.put(request, d->jsonData);
        }
        else {
            d->data.setBuffer(&d->jsonData);
            reply = d->networkManager.sendCustomRequest(request, QByteArrayLiteral("PATCH"), &d->data);
        }

        connect(reply, &QNetworkReply::finished,
                doc, &Document::_ar_dataIsAvailable
                );
    }
    else {
        QUrl url(d->standardUrl + QString("/document?collection=") + doc->collection());
        QNetworkRequest request(url);
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("Content-Length", jsonDataSize);

        QNetworkReply *reply = d->networkManager.post(request, d->jsonData);

        connect(reply, &QNetworkReply::finished,
                doc, &Document::_ar_dataIsAvailable
                );
    }
}

void ArangoDBDriver::_ar_document_update(Document * doc)
{
    d->jsonData = doc->toJsonString();
    QByteArray jsonDataSize = QByteArray::number(d->jsonData.size());

    if ( doc->isCreated() ) {
        QUrl url(d->standardUrl + QString("/document/") + doc->docID());
        QNetworkRequest request(url);
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("Content-Length", jsonDataSize);

        QNetworkReply *reply = Q_NULLPTR;
        d->data.setBuffer(&d->jsonData);
        reply = d->networkManager.sendCustomRequest(request, QByteArrayLiteral("PATCH"), &d->data);

        connect(reply, &QNetworkReply::finished,
                doc, &Document::_ar_dataIsAvailable
                );
    }
}

void ArangoDBDriver::_ar_document_delete(Document *doc)
{
    QUrl url(d->standardUrl + QString("/document/") + doc->docID());
    QNetworkRequest request(url);
    QNetworkReply *reply = d->networkManager.deleteResource(request);

    connect(reply, &QNetworkReply::finished,
            doc, &Document::_ar_dataDeleted
            );
}

void ArangoDBDriver::_ar_document_updateStatus(Document *doc)
{
    QUrl url(d->standardUrl + QString("/document/") + doc->docID());
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("etag"), doc->rev().toUtf8());
    QNetworkReply *reply = d->networkManager.head(request);

    connect(reply, &QNetworkReply::finished,
            doc, &Document::_ar_dataUpdated
            );
}

void ArangoDBDriver::_ar_document_sync(Document *doc)
{
    QUrl url(d->standardUrl + QString("/document/") + doc->docID());
    QNetworkReply *reply = d->networkManager.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished,
            doc, &Document::_ar_dataIsAvailable
            );
}

void ArangoDBDriver::_ar_edge_save(Document *doc)
{
    Edge *e = qobject_cast<Edge *>(doc);
    d->jsonData = e->toJsonString();
    QByteArray jsonDataSize = QByteArray::number(d->jsonData.size());

    if ( e->isCreated() ) {
        QUrl url(d->standardUrl + QString("/edge/") + e->docID());
        QNetworkRequest request(url);
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("Content-Length", jsonDataSize);

        QNetworkReply *reply = Q_NULLPTR;

        if ( e->isEveryAttributeDirty() ) {
            reply = d->networkManager.put(request, d->jsonData);
        }
        else {
            d->data.setBuffer(&d->jsonData);
            reply = d->networkManager.sendCustomRequest(request, QByteArrayLiteral("PATCH"), &d->data);
        }

        connect(reply, &QNetworkReply::finished,
                doc, &Edge::_ar_dataIsAvailable
                );
    }
    else {
        QString edgeUrl = d->standardUrl +
                QString("/edge?collection=%1&from=%2&to=%3")
                .arg(e->collection())
                .arg(e->from())
                .arg(e->to());

        QUrl url(edgeUrl);
        QNetworkRequest request(url);
        request.setRawHeader("Content-Type", "application/json");
        request.setRawHeader("Content-Length", jsonDataSize);

        QNetworkReply *reply = d->networkManager.post(request, d->jsonData);

        connect(reply, &QNetworkReply::finished,
                doc, &Edge::_ar_dataIsAvailable
                );
    }
}

void ArangoDBDriver::_ar_edge_delete(Document *doc)
{
    QUrl url(d->standardUrl + QString("/edge/") + doc->docID());
    QNetworkRequest request(url);
    QNetworkReply *reply = d->networkManager.deleteResource(request);

    connect(reply, &QNetworkReply::finished,
            doc, &Document::_ar_dataDeleted
            );
}

void ArangoDBDriver::_ar_collection_save(Collection * collection)
{
    d->jsonData = collection->toJsonString();
    QByteArray jsonDataSize = QByteArray::number(d->jsonData.size());

    QUrl url(d->standardUrl + QString("/collection"));
    QNetworkRequest request(url);
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", jsonDataSize);

    QNetworkReply *reply = d->networkManager.post(request, d->jsonData);

    connect(reply, &QNetworkReply::finished,
            collection, &Collection::_ar_dataIsAvailable
            );
}

void ArangoDBDriver::_ar_collection_load(Collection * collection)
{
    d->jsonData = QByteArrayLiteral("{}");
    QByteArray jsonDataSize = QByteArray::number(d->jsonData.size());
    QUrl url(d->standardUrl + QString("/collection/%1/load").arg(collection->name()));
    QNetworkRequest request(url);
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", jsonDataSize);

    QNetworkReply *reply = d->networkManager.put(request, d->jsonData);

    connect(reply, &QNetworkReply::finished,
            collection, &Collection::_ar_loaded
            );
}

void ArangoDBDriver::_ar_collection_delete(Collection * collection)
{
    QUrl url(d->standardUrl + QString("/collection/") + collection->name());
    QNetworkRequest request(url);
    QNetworkReply *reply = d->networkManager.deleteResource(request);

    connect(reply, &QNetworkReply::finished,
            collection, &Collection::_ar_isDeleted
            );
}

void ArangoDBDriver::_ar_index_save(AbstractIndex * index)
{
    d->jsonData = index->toJson();
    QByteArray jsonDataSize = QByteArray::number(d->jsonData.size());
    QUrl url(d->standardUrl + QString("/index?collection=") + index->collection()->name());
    QNetworkRequest request(url);
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", jsonDataSize);

    QNetworkReply *reply = d->networkManager.post(request, d->jsonData);

    connect(reply, &QNetworkReply::finished,
            index, &AbstractIndex::_ar_saveRequestFinished
            );
}

void ArangoDBDriver::_ar_index_delete(AbstractIndex * index)
{
    QUrl url(d->standardUrl + QString("/index/") + index->id());
    QNetworkRequest request(url);
    QNetworkReply *reply = d->networkManager.deleteResource(request);

    connect(reply, &QNetworkReply::finished,
            index, &AbstractIndex::_ar_deleteRequestFinished
            );
}

void ArangoDBDriver::_ar_transaction_commit(Transaction *transaction)
{
    qDebug() << transaction;
}

}
