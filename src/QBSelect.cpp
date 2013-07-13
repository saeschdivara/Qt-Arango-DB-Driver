#include "QBSelect.h"

#include <QtCore/QDebug>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

namespace arangodb
{

class QBSelectPrivate
{
    public:
        QString collection;
        int batchSize;
        int limit;
        bool isCounting;

        QString where;
        QString whereField;
        QStringList bindVars;
};

QBSelect::QBSelect(const QString & collection, int batchSize) :
    d_ptr(new QBSelectPrivate)
{
    Q_D(QBSelect);
    d->collection = collection;
    d->batchSize = batchSize;
    d->isCounting = false;
    d->limit = -1;
}

QBSelect::~QBSelect()
{
    delete d_ptr;
}

QString QBSelect::collection() const
{
    Q_D(const QBSelect);
    return d->collection;
}

int QBSelect::batchSize() const
{
    Q_D(const QBSelect);
    return d->batchSize;
}

void QBSelect::setCounting(bool c)
{
    Q_D(QBSelect);
    d->isCounting = c;
}

bool QBSelect::isCounting() const
{
    Q_D(const QBSelect);
    return d->isCounting;
}

void QBSelect::setWhere(const QString & field, const QString & op)
{
    Q_D(QBSelect);
    d->where = QStringLiteral("FILTER u.%1 == \"%2\"").arg(field, op);
}

void QBSelect::setWhere(const QString & field, const QStringList & op)
{
    Q_D(QBSelect);
    d->bindVars = op;
    d->where = QStringLiteral("FILTER u.%1 == %2").arg(field, QChar('@') + field);
    d->whereField = field;
}

QByteArray QBSelect::toJson() const
{
    Q_D(const QBSelect);
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;
    QString query("FOR u IN %1 %2 %3 RETURN u");

    if ( d->limit < 1 ) {
        query = query.arg(d->collection, QStringLiteral(""), d->where);
    }
    else {
        query = query.arg(d->collection, QStringLiteral("LIMIT %1").arg(QString::number(d->limit)), d->where);
    }

    jsonObj.insert(QStringLiteral("query"), query);
    jsonObj.insert(QStringLiteral("count"), d->isCounting);
    jsonObj.insert(QStringLiteral("batchSize"), d->batchSize);

    if (d->bindVars.size() > 0) {
        QJsonObject bindVarsObj;
        bindVarsObj.insert(d->whereField, QJsonArray::fromStringList(d->bindVars));

        jsonObj.insert(QStringLiteral("bindVars"), bindVarsObj);
    }

    jsonDoc.setObject(jsonObj);
    return jsonDoc.toJson();
}

}
