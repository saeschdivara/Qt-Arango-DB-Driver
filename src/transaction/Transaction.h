#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "arangodb-driver_global.h"

#include <transaction/TransactionController.h>

namespace arangodb
{

class TransactionPrivate;

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
         * @brief ~Transaction
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual ~Transaction();

        /**
         * @brief setController
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

    public Q_SLOTS:
        /**
         * @brief commit
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void commit();

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
         * @brief succeeded
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
