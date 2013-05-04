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

Document *Arangodbdriver::getDocument(QString id)
{
    Document *doc = new Document(this);

    QUrl url(d->standardUrl + QString("/document/") + id);
    QNetworkReply *reply = d->networkManager.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished,
            doc, &Document::_ar_dataIsAvailable
            );

    connect(doc, &Document::saveData,
            this, &Arangodbdriver::_ar_save
            );

    connect(doc, &Document::deleteData,
            this, &Arangodbdriver::_ar_delete
            );

    return doc;
}

Document *Arangodbdriver::createDocument(QString collection)
{
    Document *doc = new Document(collection, this);

    connect(doc, &Document::saveData,
            this, &Arangodbdriver::_ar_save
            );

    connect(doc, &Document::deleteData,
            this, &Arangodbdriver::_ar_delete
            );

    return doc;
}

void Arangodbdriver::_ar_save(Document *doc)
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

void Arangodbdriver::_ar_delete(Document *doc)
{
    QUrl url(d->standardUrl + QString("/document/") + doc->docID());
    QNetworkRequest request(url);
    QNetworkReply *reply = d->networkManager.deleteResource(request);

    connect(reply, &QNetworkReply::finished,
            doc, &Document::_ar_dataDeleted
            );
}
