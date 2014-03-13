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

#include "QBSelect.h"

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QPair>

namespace arangodb
{

class QBSelectPrivate
{
    public:
        QStringList collections;
        int batchSize;
        int limitStart;
        int limit;
        bool isCounting;
        bool isFullyCounting;

        QString where;
        QString whereField;
        QStringList bindVars;

        QVariant result;

        QList< QPair<QString, QBSelect::SortingOrder> > sorting;

        /**
         * @brief getSorting
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        QString getSorting() const {

            if ( sorting.size() == 0 ) {
                return " ";
            }

            QPair<QString, QBSelect::SortingOrder> firstSorting = sorting.first();
            QString sortingOrder(" SORT %1 %2 ");

            if ( firstSorting.second == QBSelect::SortingOrder::AscSorting ) {
                return sortingOrder.arg(firstSorting.first).arg("asc");
            }
            else if ( firstSorting.second == QBSelect::SortingOrder::DescSorting ) {
                return sortingOrder.arg(firstSorting.first).arg("desc");
            }
        }

        /**
         * @brief The ResultType enum
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        enum class ResultType : quint8 {
            NoResult            = 0,
            StringResult        = 1,
            StringListResult    = 2,
            HashResult          = 3
        };

        ResultType resultType = ResultType::NoResult;

        /**
         * @brief getResult
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
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
                        QString results;
                        QHash<QString, QVariant> resultHash = result.toHash();
                        QStringList keyList = resultHash.keys();
                        for ( const QString key : keyList) {
                            if ( key == keyList.first() ) {
                                results = getResultPart(resultHash, key);
                            }
                            else {
                                results += QChar(',') + getResultPart(resultHash, key);
                            }
                        }

                        realResult = QString("{%1}").arg(results);
                    }
                    break;

                default:
                    break;
            }

            return realResult;
        }

        /**
         * @brief getResultPart
         *
         * @param hash
         * @param key
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        QString getResultPart(const QHash<QString, QVariant> hash, const QString & key) const {
            QVariant part = hash.value(key);
            if ( part.type() == QVariant::String ) {
                return QString("\"%1_%2\": %3.%2").arg(key, part.toString(), getCollectionIdentifier(key));
            }
            else if ( part.type() == QVariant::StringList ) {
                QString result;
                QString resultPartTemplate("\"%1_%2\": %3.%2");
                QStringList fieldList = part.toStringList();
                for ( const QString field : fieldList ) {
                    QString resultPart = resultPartTemplate;
                    if ( field == fieldList.first() ) {
                        result = resultPart.arg(key, field, getCollectionIdentifier(key));
                    }
                    else {
                        result += QChar(',') + resultPart.arg(key, field, getCollectionIdentifier(key));
                    }
                }

                return result;
            }

            return QStringLiteral("");
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

void QBSelect::setFullCounting(bool count)
{
    Q_D(QBSelect);
    d->isFullyCounting = count;
}

bool QBSelect::isFullyCounting() const
{
    Q_D(const QBSelect);
    return d->isFullyCounting;
}

void QBSelect::setLimit(int start, int number)
{
    Q_D(QBSelect);
    d->limit = number;
    d->limitStart = start;
}

void QBSelect::setWhere(const QString & field, const QString & op)
{
    Q_D(QBSelect);
    QString collectionIdentifier = d->getCollectionIdentifier(d->getCollectionName());
    d->where = QStringLiteral("FILTER %1.%2 == \"%3\"").arg(collectionIdentifier, field, op);
}

void QBSelect::setWhere(const QString & field, bool op)
{
    Q_D(QBSelect);
    QString collectionIdentifier = d->getCollectionIdentifier(d->getCollectionName());
    if (op) d->where = QStringLiteral("FILTER %1.%2 == true").arg(collectionIdentifier, field);
    else d->where = QStringLiteral("FILTER %1.%2 == false").arg(collectionIdentifier, field);
}

void QBSelect::setWhere(const QString & field, const QStringList & op)
{
    Q_D(QBSelect);
    QString collectionIdentifier = d->getCollectionIdentifier(d->getCollectionName());
    d->bindVars = op;
    d->where = QStringLiteral("FILTER %1.%2 IN %3").arg(collectionIdentifier, field, QChar('@') + field);
    d->whereField = field;
}

void QBSelect::setWhere(const QString & collection1, const QString & field1,
                        const QString & collection2, const QString & field2)
{
    Q_D(QBSelect);
    QString identifier1 = d->getCollectionIdentifier(collection1);
    QString identifier2 = d->getCollectionIdentifier(collection2);
    d->where = QStringLiteral("FILTER %1.%2 == %3.%4").arg(identifier1, field1, identifier2, field2);
}

void QBSelect::setResult(const QString & collectionName)
{
    Q_D(QBSelect);
    d->result = collectionName;
    d->resultType = QBSelectPrivate::ResultType::StringResult;
}

void QBSelect::setResult(const QStringList & collectionNames)
{
    Q_D(QBSelect);
    d->result = collectionNames;
    d->resultType = QBSelectPrivate::ResultType::StringListResult;
}

void QBSelect::setResult(const QHash<QString, QVariant> & collectionFields)
{
    Q_D(QBSelect);
    d->result = collectionFields;
    d->resultType = QBSelectPrivate::ResultType::HashResult;
}

void QBSelect::setSortingColumn(const QString &collection, const QString &column, QBSelect::SortingOrder order)
{
    Q_D(QBSelect);
    QPair<QString, SortingOrder> sorting;
    sorting.first = d->getCollectionIdentifier(collection) + "." + column;
    sorting.second = order;

    d->sorting.insert(0, sorting);
}

QByteArray QBSelect::toJson() const
{
    Q_D(const QBSelect);
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;
    const QString forCollectionTemplate("FOR %1 IN %2 ");
    QString query("FOR %1 IN %2 %3 %4 %5 RETURN %6");

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
        query = query.arg(collectionIdentifier, lastCollection, d->getSorting(), QStringLiteral(""), d->where);
    }
    else {
        QString limit = QStringLiteral("LIMIT %1,%2")
                .arg(QString::number(d->limitStart))
                .arg(QString::number(d->limit));

        query = query.arg(collectionIdentifier, lastCollection, d->getSorting(), limit, d->where);
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

    if ( d->isFullyCounting ) {
        QJsonObject options;
        options.insert(QStringLiteral("fullCount"), d->isFullyCounting);

        jsonObj.insert("options", options);
    }

    jsonDoc.setObject(jsonObj);
    return jsonDoc.toJson();
}

}
