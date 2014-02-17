#include "Database.h"

#include "private/AbstractDbObject_p.h"

namespace arangodb
{

class DatabasePrivate : public AbstractDbObjectPrivate
{
    public:
        QString name;
        QList<Database::User *> users;
};

Database::Database(const QString &name, QObject *parent) :
    AbstractDbObject(new DatabasePrivate, parent)
{
    Q_D(Database);
    d->name = name;

    Database::User * root = new Database::User;
    root->name = "root";
    root->isActive = true;

    d->users.append(root);
}

Database::Database(const QString &name, QList<Database::User *> users, QObject *parent):
    AbstractDbObject(new DatabasePrivate, parent)
{
    Q_D(Database);
    d->name = name;

    d->users = users;
}

Database::~Database()
{
    Q_D(Database);

    qDeleteAll(d->users);
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
