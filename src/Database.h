#ifndef DATABASE_H
#define DATABASE_H

#include "arangodb-driver_global.h"

#include "AbstractDbObject.h"
#include <QtCore/QJsonObject>

namespace arangodb
{

class DatabasePrivate;

/**
 * @brief The Database class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.6
 */
class ARANGODBDRIVERSHARED_EXPORT Database : public AbstractDbObject
{
        Q_OBJECT
    public:
        class User
        {
            public:
                QString name;
                QString password;
                bool isActive = false;
                QJsonObject extraInformation;
        };

        explicit Database(const QString & name, QObject *parent = 0);
        explicit Database(const QString & name, QList<Database::User *> users, QObject *parent = 0);
        virtual ~Database();

        virtual QByteArray toJsonString() const;

    protected:
        virtual void _onDataAvailable();

    private:
        Q_DECLARE_PRIVATE(Database)
};

}

#endif // DATABASE_H
