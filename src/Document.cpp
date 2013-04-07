#include "Document.h"
#include <QtDebug>
#include <QtCore/QJsonDocument>
#include <QtNetwork/QNetworkReply>

using namespace arangodb;

namespace internal {

class DocumentPrivate
{
    public:
        const QString ID  = QStringLiteral("_id");
        const QString KEY = QStringLiteral("_key");
        const QString REV = QStringLiteral("_rev");
};

}

Document::Document(QObject *parent) :
    QObject(parent),
    d(new internal::DocumentPrivate)
{
}

Document::~Document()
{
    delete d;
}

void Document::_ar_dataIsAvailable()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qDebug() << reply->readAll();
    emit ready();
}

void Document::save()
{
    emit saveData(this);
}
