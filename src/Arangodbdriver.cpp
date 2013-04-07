#include "Arangodbdriver.h"
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
};

}

Arangodbdriver::Arangodbdriver(QString protocol, QString host, qint32 port) :
    d(new internal::ArangodbdriverPrivate)
{
    d->protocol = protocol;
    d->host = host;
    d->port = port;
}

Arangodbdriver::~Arangodbdriver()
{
    delete d;
}

Document *Arangodbdriver::getDocument(QString id)
{
    Document *doc = new Document;

    QUrl url(d->protocol + QString("://") + d->host + QString(":") + QString::number(d->port) + QString("/_api/document/") + id);
    QNetworkReply *reply = d->networkManager.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished,
            doc, &Document::_ar_dataIsAvailable
            );

    connect(doc, &Document::saveData,
            this, &Arangodbdriver::_ar_save
            );

    return doc;
}

void Arangodbdriver::_ar_save(Document *doc)
{
    Q_UNUSED(doc)
}
