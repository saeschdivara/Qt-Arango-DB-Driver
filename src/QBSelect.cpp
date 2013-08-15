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
        QStringList collections;
        int batchSize;
        int limit;
        bool isCounting;

        QString where;
        QString whereField;
        QStringList bindVars;

        /**
         * @brief Since there can be multiple collections
         * we need to have a way to generate their identfiers
         *
         * @param collectionName
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        inline QString getCollectionIdentifier(const QString & collectionName) const {
            QString identifier;

            if ( collectionName.length() > 4 )
                identifier = collectionName.left(4).toLower();
            else
                identifier = collectionName + QChar('_');

            return identifier;
        }

        /**
         * @brief Returns the name of the first collection,
         * otherwise an empty string
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        inline QString getCollectionName() const {
            return (collections.isEmpty()) ? QStringLiteral("") : collections.first();
        }
};

QBSelect::QBSelect(const QString & collection, int batchSize) :
    d_ptr(new QBSelectPrivate)
{
    Q_D(QBSelect);
    d->collections.append(collection);
    d->batchSize = batchSize;
    d->isCounting = false;
    d->limit = -1;
}

QBSelect::QBSelect(const QStringList & collections, int batchSize):
    d_ptr(new QBSelectPrivate)
{
    Q_D(QBSelect);
    d->collections = collections;
    d->batchSize = batchSize;
    d->isCounting = false;
    d->limit = -1;
}

QBSelect::~QBSelect()
{
    delete d_ptr;
}

void QBSelect::addNewCollection(const QString & collection)
{
    Q_D(QBSelect);
    if ( !d->collections.contains(collection) )
        d->collections.append(collection);
}

QStringList QBSelect::collections() const
{
    Q_D(const QBSelect);
    return d->collections;
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
    QString collectionIdentifier = d->getCollectionIdentifier(d->getCollectionName());
    d->where = QStringLiteral("FILTER %1.%2 == \"%3\"").arg(collectionIdentifier, field, op);
}

void QBSelect::setWhere(const QString & field, const QStringList & op)
{
    Q_D(QBSelect);
    QString collectionIdentifier = d->getCollectionIdentifier(d->getCollectionName());
    d->bindVars = op;
    d->where = QStringLiteral("FILTER %1.%2 IN %3").arg(collectionIdentifier, field, QChar('@') + field);
    d->whereField = field;
}

QByteArray QBSelect::toJson() const
{
    Q_D(const QBSelect);
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;
    const QString forCollectionTemplate("FOR %1 IN %2");
    QString query("FOR %1 IN %2 %3 %4 RETURN %1");

    const int totalCollections = d->collections.size();
    for (int i = 0; i < totalCollections-1; ++i) {
        QString collection = forCollectionTemplate;
        QString collectionName = d->collections.at(i);
        collection.arg(d->getCollectionIdentifier(collectionName), collectionName);
        query = collection + query;
    }

    QString lastCollection = d->collections.last();
    QString collectionIdentifier = d->getCollectionIdentifier(lastCollection);
    if ( d->limit < 1 ) {
        query = query.arg(collectionIdentifier, lastCollection, QStringLiteral(""), d->where);
    }
    else {
        QString limit = QStringLiteral("LIMIT %1").arg(QString::number(d->limit));
        query = query.arg(collectionIdentifier, lastCollection, limit, d->where);
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
