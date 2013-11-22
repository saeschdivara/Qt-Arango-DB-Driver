/********************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2013 Sascha Ludwig Häusler
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy of
 ** this software and associated documentation files (the "Software"), to deal in
 ** the Software without restriction, including without limitation the rights to
 ** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 ** the Software, and to permit persons to whom the Software is furnished to do so,
 ** subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in all
 ** copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 ** FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 ** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 ** IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 ** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *********************************************************************************/

#ifndef TRANSACTIONCONTROLLER_H
#define TRANSACTIONCONTROLLER_H

#include "arangodb-driver_global.h"

#include <ArangoDBDriver.h>
#include <transaction/Transaction.h>

#include <QtCore/QSharedPointer>

namespace arangodb
{

class TransactionControllerPrivate;

/**
 * @brief The TransactionController class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.6
 */
class ARANGODBDRIVERSHARED_EXPORT TransactionController : public QObject
{
        Q_OBJECT
    public:
        /**
         * @brief TransactionController
         *
         * @param parent
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        explicit TransactionController(QObject *parent = 0);

        /**
         * @brief ~TransactionController
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual ~TransactionController();

        /**
         * @brief createTransaction
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        QSharedPointer<Transaction> createTransaction();

    Q_SIGNALS:
        /**
         * @brief transactionCommittedSignal
         *
         * @param
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        Q_SIGNAL void transactionCommittedSignal(Transaction *);

    protected:
        TransactionControllerPrivate * d_ptr;

    private:
        Q_DECLARE_PRIVATE(TransactionController)
};

}

#endif // TRANSACTIONCONTROLLER_H
