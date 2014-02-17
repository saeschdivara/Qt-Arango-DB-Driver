#include "AbstractDbObject.h"
#include "private/AbstractDbObject_p.h"

namespace arangodb
{

AbstractDbObject::AbstractDbObject(QObject *parent) :
    AbstractDbObject(new AbstractDbObjectPrivate, parent)
{
}

AbstractDbObject::~AbstractDbObject()
{
    delete d_ptr;
}

void AbstractDbObject::save()
{
    Q_EMIT saveData(this);
}

void AbstractDbObject::_ar_dataIsAvailable()
{
    _onDataAvailable();
}

AbstractDbObject::AbstractDbObject(AbstractDbObjectPrivate *ptr, QObject *parent) :
    QObject(parent),
    d_ptr(ptr)
{
    //
}

}
