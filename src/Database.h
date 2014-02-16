#ifndef DATABASE_H
#define DATABASE_H

#include "arangodb-driver_global.h"

#include "AbstractDbObject.h"

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
        explicit Database(QObject *parent = 0);

        virtual QByteArray toJsonString() const;

    protected:
        virtual void _onDataAvailable();

    private:
        Q_DECLARE_PRIVATE(Database)
};

}

#endif // DATABASE_H
