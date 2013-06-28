#include "QueryBuilder.h"

using namespace arangodb;

QueryBuilder::QueryBuilder(QObject *parent) :
    QObject(parent)
{
}

QSharedPointer<QBSelect> QueryBuilder::createSelect(QString collection)
{
    QSharedPointer<QBSelect> select(new QBSelect);
    return select;
}
