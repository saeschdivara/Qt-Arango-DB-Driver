#ifndef ABSTRACTDBOBJECT_H
#define ABSTRACTDBOBJECT_H

#include "arangodb-driver_global.h"

namespace arangodb
{

class AbstractDbObjectPrivate;

class ARANGODBDRIVERSHARED_EXPORT AbstractDbObject : public QObject
{
        Q_OBJECT
    public:
        explicit AbstractDbObject(QObject *parent = 0);
        virtual ~AbstractDbObject();

        virtual QByteArray toJsonString() const = 0;

    Q_SIGNALS:
        void saveData(AbstractDbObject *);

    public Q_SLOTS:
        void save();

        void _ar_dataIsAvailable();

    protected:
        AbstractDbObjectPrivate * d_ptr;
        AbstractDbObject(AbstractDbObjectPrivate * ptr, QObject *parent = 0);

        virtual void _onDataAvailable() = 0;

    private:
        Q_DECLARE_PRIVATE(AbstractDbObject)
};

}

#endif // ABSTRACTDBOBJECT_H
