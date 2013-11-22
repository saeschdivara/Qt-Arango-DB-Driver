#include "Transaction.h"

namespace arangodb
{

class TransactionPrivate
{
    public:
        TransactionController * controller;
        bool isStarted = false;
        bool autoCommit = false;
};

Transaction::Transaction(QObject *parent) :
    QObject(parent),
    d_ptr(new TransactionPrivate)
{
}

Transaction::~Transaction()
{
    Q_D(Transaction);

    // If a transaction is started and auto commit is set
    // the transaction is commited
    if (d->isStarted && d->autoCommit) {
            commit();
        }

    delete d_ptr;
}

void Transaction::setController(TransactionController *ctrl)
{
    Q_D(Transaction);
    d->controller = ctrl;
}

void Transaction::setAutoCommit(bool autoCommit)
{
    Q_D(Transaction);
    d->autoCommit = autoCommit;
}

bool Transaction::autoCommit() const
{
    Q_D(const Transaction);
    return d->autoCommit;
}

void Transaction::start()
{
    Q_D(Transaction);
    d->isStarted = true;
}

void Transaction::commit()
{
    Q_D(Transaction);
    d->isStarted = false;

    Q_EMIT commitSignal(this);
}

}
