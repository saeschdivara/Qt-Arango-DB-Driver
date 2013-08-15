#include "QueryBuilder.h"

namespace arangodb
{

QueryBuilder::QueryBuilder()
{
}

QSharedPointer<QBSelect> QueryBuilder::createSelect(QString collection, int batchSize)
{
    QSharedPointer<QBSelect> select(new QBSelect(collection, batchSize));
    return select;
}

QSharedPointer<QBSelect> QueryBuilder::createSelect(QStringList & collections, int batchSize)
{
    QSharedPointer<QBSelect> select(new QBSelect(collections, batchSize));
    return select;
}

}
