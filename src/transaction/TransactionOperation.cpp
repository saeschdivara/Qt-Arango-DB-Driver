#include "TransactionOperation.h"

namespace arangodb
{

class TransactionOperationPrivate
{
    public:
        TransactionOperation::Type type;
        TransactionOperation::Source source;
};

TransactionOperation::TransactionOperation(Source source, Type type) :
    d_ptr(new TransactionOperationPrivate)
{
    d_ptr->type = type;
    d_ptr->source = source;
}

TransactionOperation::~TransactionOperation()
{
    delete d_ptr;
}

TransactionOperation::Source TransactionOperation::source() const
{
    Q_D(const TransactionOperation);
    return d->source;
}

TransactionOperation::Type TransactionOperation::type() const
{
    Q_D(const TransactionOperation);
    return d->type;
}

}
