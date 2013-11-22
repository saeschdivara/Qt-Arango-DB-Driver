#ifndef TRANSACTIONOPERATION_H
#define TRANSACTIONOPERATION_H

#include "arangodb-driver_global.h"

namespace arangodb
{

class TransactionOperationPrivate;

/**
 * @brief The TransactionOperation class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.6
 */
class TransactionOperation : public QObject
{
        Q_OBJECT
    public:
        /**
         * @brief The Type enum
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        enum class Type {
            Save,
            Delete,
            Update
        };

        /**
         * @brief TransactionOperation
         *
         * @param parent
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        explicit TransactionOperation(QObject *parent = 0);

        /**
         * @brief ~TransactionOperation
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual ~TransactionOperation();

    protected:
        TransactionOperationPrivate * d_ptr;

    private:
        Q_DECLARE_PRIVATE(TransactionOperation)
};

}

#endif // TRANSACTIONOPERATION_H
