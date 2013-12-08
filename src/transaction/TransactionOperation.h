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
class TransactionOperation
{
    public:
        /**
         * @brief The Type enum
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        enum class Type : quint8 {
            Save,
            Delete,
            Update
        };

        /**
         * @brief The Source enum
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        enum class Source : quint8 {
            Collection,
            Document,
            Edge,
            Index
        };

        /**
         * @brief TransactionOperation
         *
         * @param parent
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        explicit TransactionOperation(Source source, Type type);

        /**
         * @brief ~TransactionOperation
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual ~TransactionOperation();

        /**
         * @brief source
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        Source source() const;

        /**
         * @brief type
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        Type type() const;

    protected:
        TransactionOperationPrivate * d_ptr;

    private:
        Q_DECLARE_PRIVATE(TransactionOperation)
};

}

#endif // TRANSACTIONOPERATION_H
