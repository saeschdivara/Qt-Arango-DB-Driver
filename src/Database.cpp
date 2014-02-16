#include "Database.h"

#include "private/AbstractDbObject_p.h"

namespace arangodb
{

class DatabasePrivate : public AbstractDbObjectPrivate
{
    public:
};

Database::Database(QObject *parent) :
    AbstractDbObject(new DatabasePrivate, parent)
{
}

QByteArray Database::toJsonString() const
{
    Q_D(const Database);

    return QByteArray();
}

void Database::_onDataAvailable()
{
    //
}

}
