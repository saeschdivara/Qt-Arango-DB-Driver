#include "QBCursor.h"
#include "Arangodbdriver.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtNetwork/QNetworkReply>

namespace arangodb
{

class QBCursorPrivate
{
    public:
        bool hasMore;
        QString id;
        QList<Document *> docs;

        QString errorMessage;
        quint32 errorCode = 0;
        quint32 errorNumber = 0;

        inline void resetError() {
            errorMessage.clear();
            errorCode = 0;
            errorNumber = 0;
        }
};

QBCursor::QBCursor(QObject *parent) :
    QObject(parent),
    d_ptr(new QBCursorPrivate)
{
    Q_D(QBCursor);
    d->hasMore = false;
}

bool QBCursor::hasMore() const
{
    Q_D(const QBCursor);
    return d->hasMore;
}

QString QBCursor::id() const
{
    Q_D(const QBCursor);
    return d->id;
}

QString QBCursor::errorMessage() const
{
    Q_D(const QBCursor);
    return d->errorMessage;
}

quint32 QBCursor::errorCode() const
{
    Q_D(const QBCursor);
    return d->errorCode;
}

quint32 QBCursor::errorNumber() const
{
    Q_D(const QBCursor);
    return d->errorNumber;
}

bool QBCursor::hasErrorOccurred() const
{
    Q_D(const QBCursor);
    return d->errorCode != 0;
}

QList<Document *> QBCursor::data()
{
    Q_D(QBCursor);
    return d->docs;
}

int QBCursor::count() const
{
    Q_D(const QBCursor);
    return d->docs.count();
}

void QBCursor::getMoreData()
{
    Arangodbdriver * driver = qobject_cast<Arangodbdriver *>(parent());
    if (driver) {
        driver->loadMoreResults(this);
    }
    else {
        qWarning() << Q_FUNC_INFO;
        qWarning() << "Parent is not Arangodbdriver";
    }
}

void QBCursor::waitForResult()
{
    bool b = true;
    QMetaObject::Connection conn1 = QObject::connect(this, &QBCursor::ready, [&] {
        b = false;
    });
    QMetaObject::Connection conn2 = QObject::connect(this, &QBCursor::error, [&] {
        b = false;
    });
    while (b) {
        qApp->processEvents();
    }
    QObject::disconnect(conn1);
    QObject::disconnect(conn2);
}

void QBCursor::_ar_cursor_result_loaded()
{
    Q_D(QBCursor);

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    disconnect(reply, &QNetworkReply::finished,
               this, &QBCursor::_ar_cursor_result_loaded);

    QByteArray data = reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    d->resetError();

    if ( obj.value(QStringLiteral("error")).toBool() ) {
        d->errorMessage = obj.value("errorMessage").toString();
        d->errorNumber  = obj.value("errorNum").toVariant().toInt();
        d->errorCode    = obj.value("code").toVariant().toInt();

        emit error();
        return;
    }

    d->hasMore = obj.value(QStringLiteral("hasMore")).toBool();
    d->id      = obj.value(QStringLiteral("id")).toString();

    QJsonArray dataArr = obj.value(QStringLiteral("result")).toArray();
    const int total = dataArr.size();
    for (int i = 0; i < total; ++i) {
        Document * doc = new Document(dataArr.at(i).toObject());
        d->docs.append(doc);
    }

    emit ready();
}

}
