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

        QVariant result;

        enum class ResultType : quint8 {
            NoResult            = 0,
            StringResult        = 1,
            StringListResult    = 2,
            HashResult          = 3
        };

        ResultType resultType = ResultType::NoResult;

        inline QString getResult() const {
            QString realResult;
            ResultType resultType = this->resultType;
            // If no result is set, we assume everything is wanted
            if ( resultType == ResultType::NoResult ) {
                resultType = (collections.size() == 1) ? ResultType::StringResult
                                                       : ResultType::StringListResult;
            }

            switch (resultType)
            {
                case ResultType::StringResult: {
                        realResult = QString("%1").arg(getCollectionIdentifier(getCollectionName()));
                    }
                    break;

                case ResultType::StringListResult: {
                        QString collectionsResult = getCollectionIdentifier(collections.first());
                        const int total = collections.size();
                        for (int i = 1; i < total; ++i) {
                            collectionsResult += QString(",") + getCollectionIdentifier(collections.at(i));
                        }

                        realResult = QString("MERGE(%1)").arg(collectionsResult);
                    }
                    break;

                case ResultType::HashResult: {
                    }
                    break;

                default:
                    break;
            }

            return realResult;
        }

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
                identifier = collectionName.right(2).toUpper() + collectionName.left(2).toLower();
            else
                identifier = collectionName;

            identifier += QChar('_');

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

void QBSelect::setResult(const QString & collectionName)
{
    Q_D(QBSelect);
    d->result = collectionName;
    d->resultType = QBSelectPrivate::ResultType::StringResult;
}

QByteArray QBSelect::toJson() const
{
    Q_D(const QBSelect);
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;
    const QString forCollectionTemplate("FOR %1 IN %2 ");
    QString query("FOR %1 IN %2 %3 %4 RETURN %5");

    const int totalCollections = d->collections.size();
    for (int i = 0; i < totalCollections-1; ++i) {
        QString collection = forCollectionTemplate;
        QString collectionName = d->collections.at(i);
        collection = collection.arg(d->getCollectionIdentifier(collectionName), collectionName);
        query = collection + query;
    }

    QString lastCollection = d->collections.last();
    QString collectionIdentifier = d->getCollectionIdentifier(lastCollection);

    // Only if limit is over 0, a limit is set
    if ( d->limit < 1 ) {
        query = query.arg(collectionIdentifier, lastCollection, QStringLiteral(""), d->where);
    }
    else {
        QString limit = QStringLiteral("LIMIT %1").arg(QString::number(d->limit));
        query = query.arg(collectionIdentifier, lastCollection, limit, d->where);
    }

    query = query.arg(d->getResult());

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
