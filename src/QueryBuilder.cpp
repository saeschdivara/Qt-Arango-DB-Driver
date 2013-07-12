#include "QueryBuilder.h"

using namespace arangodb;

QueryBuilder::QueryBuilder(QObject *parent) :
    QObject(parent)
{
}

QSharedPointer<QBSelect> QueryBuilder::createSelect(QString collection, int batchSize)
{
    QSharedPointer<QBSelect> select(new QBSelect(collection, batchSize));
    return select;
}
