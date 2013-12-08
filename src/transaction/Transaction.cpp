#include "Transaction.h"

#include <transaction/TransactionController.h>
#include <transaction/TransactionOperation.h>

namespace arangodb
{

class TransactionPrivate
{
    public:
        TransactionController * controller;
        bool isStarted = false;
        bool autoCommit = false;

        QList<TransactionOperation *> recordedOperations;
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

void Transaction::connectCollection(Collection *coll)
{
    ArangoDBDriver * driver = qobject_cast<ArangoDBDriver *>(coll->parent());

    if ( driver )
        driver->disconnectCollection(coll);

    QObject::connect( coll, &Collection::saveData,
                      this, static_cast<void (Transaction::*)(Collection *)>(&Transaction::recordSaveEvent)
                      );
}

void Transaction::disconnectCollection(Collection *coll)
{
    ArangoDBDriver * driver = qobject_cast<ArangoDBDriver *>(coll->parent());

    if ( driver )
        driver->connectCollection(coll);

    QObject::disconnect( coll, &Collection::saveData,
                      this, static_cast<void (Transaction::*)(Collection *)>(&Transaction::recordSaveEvent)
                         );
}

void Transaction::connectDocument(Document *doc)
{
    ArangoDBDriver * driver = qobject_cast<ArangoDBDriver *>(doc->parent());

    if ( driver )
        driver->disconnectDocument(doc);

    QObject::connect( doc, &Document::saveData,
                      this, static_cast<void (Transaction::*)(Document *)>(&Transaction::recordSaveEvent)
                      );
}

void Transaction::disconnectDocument(Document *doc)
{
    ArangoDBDriver * driver = qobject_cast<ArangoDBDriver *>(doc->parent());

    if ( driver )
        driver->connectDocument(doc);

    QObject::disconnect( doc, &Document::saveData,
                      this, static_cast<void (Transaction::*)(Document *)>(&Transaction::recordSaveEvent)
                         );
}

void Transaction::connectIndex(AbstractIndex *index)
{
    ArangoDBDriver * driver = qobject_cast<ArangoDBDriver *>(index->parent());

    if ( driver )
        driver->disconnectIndex(index);

    QObject::connect( index, &AbstractIndex::saveSignal,
                      this, static_cast<void (Transaction::*)(AbstractIndex *)>(&Transaction::recordSaveEvent)
                      );
}

void Transaction::commit()
{
    Q_D(Transaction);
    d->isStarted = false;

    Q_EMIT commitSignal(this);
}

void Transaction::recordSaveEvent(Collection *coll)
{
    Q_D(Transaction);
    qDebug() << this << "recorded save event from " << coll;
    auto operation = new TransactionOperation(TransactionOperation::Source::Collection, TransactionOperation::Type::Save);
    d->recordedOperations.append(operation);
}

void Transaction::recordSaveEvent(Document *doc)
{
    Q_D(Transaction);
    qDebug() << this << "recorded save event from " << doc;
    auto operation = new TransactionOperation(TransactionOperation::Source::Document, TransactionOperation::Type::Save);
    d->recordedOperations.append(operation);
}

void Transaction::recordSaveEvent(AbstractIndex *index)
{
    Q_D(Transaction);
    qDebug() << this << "recorded save event from " << index;
    auto operation = new TransactionOperation(TransactionOperation::Source::Index, TransactionOperation::Type::Save);
    d->recordedOperations.append(operation);
}

}
