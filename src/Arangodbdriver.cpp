#include "Arangodbdriver.h"
#include <QtCore/QBuffer>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

using namespace arangodb;

namespace internal {

class ArangodbdriverPrivate
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

Arangodbdriver::Arangodbdriver(QString protocol, QString host, qint32 port) :
    d(new internal::ArangodbdriverPrivate)
{
    d->protocol = protocol;
    d->host = host;
    d->port = port;

    d->createStandardUrl();
}

Arangodbdriver::~Arangodbdriver()
{
    delete d;
}

Collection *Arangodbdriver::getCollection(QString name)
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

Collection *Arangodbdriver::createCollection(const QString & name, bool waitForSync, int journalSize, bool isSystem, bool isVolatile, Collection::KeyOption * keyOption, Collection::Type type)
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

void Arangodbdriver::connectCollection(Collection * collection)
{
    Q_UNUSED(collection);
}

Document *Arangodbdriver::getDocument(QString id)
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

Document *Arangodbdriver::createDocument(QString collection)
{
    Document *doc = new Document(collection, this);
    connectDocument(doc);

    return doc;
}

Document *Arangodbdriver::createDocument(QString collection, QString key)
{
    Document *doc = new Document(collection, key, this);
    connectDocument(doc);

    return doc;
}

void Arangodbdriver::connectDocument(Document * doc)
{
    connect(doc, &Document::saveData,
            this, &Arangodbdriver::_ar_document_save
            );

    connect(doc, &Document::deleteData,
            this, &Arangodbdriver::_ar_document_delete
            );

    connect(doc, &Document::updateDataStatus,
            this, &Arangodbdriver::_ar_document_updateStatus
            );

    connect(doc, &Document::syncData,
            this, &Arangodbdriver::_ar_document_sync
            );
}

Edge *Arangodbdriver::getEdge(QString id)
{
    Edge *e = new Edge(this);

    QUrl url(d->standardUrl + QString("/edge/") + id);
    QNetworkReply *reply = d->networkManager.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished,
            e, &Document::_ar_dataIsAvailable
            );

    connect(e, &Edge::saveData,
            this, &Arangodbdriver::_ar_edge_save
            );

    connect(e, &Edge::deleteData,
            this, &Arangodbdriver::_ar_edge_delete
            );

    connect(e, &Edge::updateDataStatus,
            this, &Arangodbdriver::_ar_document_updateStatus
            );

    connect(e, &Edge::syncData,
            this, &Arangodbdriver::_ar_document_sync
            );

    return e;
}

Edge *Arangodbdriver::createEdge(QString collection, Document *fromDoc, Document *toDoc)
{
    Edge *e = new Edge(collection, fromDoc, toDoc, this);

    connect(e, &Edge::saveData,
            this, &Arangodbdriver::_ar_edge_save
            );

    connect(e, &Edge::deleteData,
            this, &Arangodbdriver::_ar_edge_delete
            );

    connect(e, &Edge::updateDataStatus,
            this, &Arangodbdriver::_ar_document_updateStatus
            );

    connect(e, &Edge::syncData,
            this, &Arangodbdriver::_ar_document_sync
            );

    return e;
}

QSharedPointer<QBCursor> Arangodbdriver::executeSelect(QSharedPointer<QBSelect> select)
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

void Arangodbdriver::loadMoreResults(QBCursor * cursor)
{
    QUrl url(d->standardUrl + QString("/cursor/") + cursor->id());
    QNetworkRequest request(url);
    request.setRawHeader("Content-Type", "application/json");

    QNetworkReply *reply = d->networkManager.put(request, QByteArrayLiteral(""));

    connect(reply, &QNetworkReply::finished,
            cursor, &QBCursor::_ar_cursor_result_loaded
            );
}

void Arangodbdriver::_ar_document_save(Document *doc)
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

void Arangodbdriver::_ar_document_delete(Document *doc)
{
    QUrl url(d->standardUrl + QString("/document/") + doc->docID());
    QNetworkRequest request(url);
    QNetworkReply *reply = d->networkManager.deleteResource(request);

    connect(reply, &QNetworkReply::finished,
            doc, &Document::_ar_dataDeleted
            );
}

void Arangodbdriver::_ar_document_updateStatus(Document *doc)
{
    QUrl url(d->standardUrl + QString("/document/") + doc->docID());
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("etag"), doc->rev().toUtf8());
    QNetworkReply *reply = d->networkManager.head(request);

    connect(reply, &QNetworkReply::finished,
            doc, &Document::_ar_dataUpdated
            );
}

void Arangodbdriver::_ar_document_sync(Document *doc)
{
    QUrl url(d->standardUrl + QString("/document/") + doc->docID());
    QNetworkReply *reply = d->networkManager.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished,
            doc, &Document::_ar_dataIsAvailable
            );
}

void Arangodbdriver::_ar_edge_save(Document *doc)
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

void Arangodbdriver::_ar_edge_delete(Document *doc)
{
    QUrl url(d->standardUrl + QString("/edge/") + doc->docID());
    QNetworkRequest request(url);
    QNetworkReply *reply = d->networkManager.deleteResource(request);

    connect(reply, &QNetworkReply::finished,
            doc, &Document::_ar_dataDeleted
            );
}
