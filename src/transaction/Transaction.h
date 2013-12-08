#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "arangodb-driver_global.h"
#include "Collection.h"
#include "Document.h"
#include "Edge.h"
#include "index/AbstractIndex.h"

namespace arangodb
{

class TransactionPrivate;
class TransactionController;

/**
 * @brief The Transaction class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.6
 */
class ARANGODBDRIVERSHARED_EXPORT Transaction : public QObject
{
        Q_OBJECT
    public:
        /**
         * @brief Transaction
         *
         * @param parent
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        explicit Transaction(QObject *parent = 0);

        /**
         * @brief If autocommit is true and the transaction is started,
         * the transaction is committed
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual ~Transaction();

        /**
         * @brief If the transaction is created by a transaction controller,
         * the controller is already set
         *
         * @param ctrl
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setController(TransactionController * ctrl);

        /**
         * @brief setAutoCommit
         *
         * @param autoCommit
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setAutoCommit(bool autoCommit);

        /**
         * @brief Returns true if the transaction is committed
         * even if it wasn't manually called and the transaction object
         * is destroyed
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        bool autoCommit() const;

        /**
         * @brief Starts transaction, this means that every action
         * from connected objects are recorded and saved so they
         * will be included in the same order
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void start();

        /**
         * @brief Connects the collection to the transaction that
         * all actions which would require actions from the ArangoDBDriver
         * are now only recorded.
         * But for now are only the following Q_SIGNAL recorded:
         * - saveData
         *
         * @param coll
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void connectCollection(Collection * coll);

        /**
         * @brief disconnectCollection
         *
         * @param coll
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void disconnectCollection(Collection * coll);

        /**
         * @brief Connects the collection to the transaction that
         * all actions which would require actions from the ArangoDBDriver
         * are now only recorded.
         * But for now are only the following Q_SIGNAL recorded:
         * - saveData
         *
         * @param doc
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void connectDocument(Document * doc);

        /**
         * @brief disconnectDocument
         *
         * @param doc
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void disconnectDocument(Document * doc);

        // TODO: Check if for Edges, the Document connecting is good enough

        /**
         * @brief connectIndex
         *
         * @param index
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void connectIndex(index::AbstractIndex * index);

    public Q_SLOTS:
        /**
         * @brief commit
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        Q_SLOT void commit();

    protected Q_SLOTS:
        /**
         * @brief recordSaveEvent
         *
         * @param coll
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        Q_SLOT void recordSaveEvent(Collection * coll);

        /**
         * @brief recordSaveEvent
         *
         * @param doc
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        Q_SLOT void recordSaveEvent(Document * doc);

        /**
         * @brief recordSaveEvent
         *
         * @param index
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        Q_SLOT void recordSaveEvent(index::AbstractIndex * index);


    Q_SIGNALS:
        /**
         * @brief When the signal is emitted when the transaction is commited
         *
         * @param
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        Q_SIGNAL void commitSignal(Transaction *);

        /**
         * @brief This signal is emitted after commiting the transaction
         * failed
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        Q_SIGNAL void failed();

        /**
         * @brief This signal is emitted after commiting the transaction
         * succeeded
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        Q_SIGNAL void succeeded();

    protected:
        TransactionPrivate * d_ptr;

    private:
        Q_DECLARE_PRIVATE(Transaction)
};

}

#endif // TRANSACTION_H
