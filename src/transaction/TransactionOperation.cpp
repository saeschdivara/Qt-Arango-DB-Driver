#include "TransactionOperation.h"

namespace arangodb
{

class TransactionOperationPrivate
{
    public:
};

TransactionOperation::TransactionOperation(QObject *parent) :
    QObject(parent),
    d_ptr(new TransactionOperationPrivate)
{
}

TransactionOperation::~TransactionOperation()
{
    delete d_ptr;
}

}
