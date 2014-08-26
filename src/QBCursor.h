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

#ifndef QBCURSOR_H
#define QBCURSOR_H

#include "arangodb-driver_global.h"
#include "Document.h"

#include <QtCore/QList>

namespace arangodb
{

class QBCursorPrivate;

/**
 * @brief The QBCursor class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.4
 */
class ARANGODBDRIVERSHARED_EXPORT QBCursor : public QObject
{
        Q_OBJECT
    public:
        /**
         * @brief QBCursor
         *
         * @param parent
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        QBCursor(QObject *parent = 0);

        /**
         * @brief hasMore
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        bool hasMore() const;

        /**
         * @brief id
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        QString id() const;

        /**
         * @brief errorMessage
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        QString errorMessage() const;

        /**
         * @brief errorCode
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        quint32 errorCode() const;

        /**
         * @brief errorNumber
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        quint32 errorNumber() const;

        /**
         * @brief hasErrorOccurred
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        bool hasErrorOccurred() const;

        /**
         * @brief data
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        QList<Document *> data();

        /**
         * @brief count
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        int count() const;

        /**
         * @brief getMoreData
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        void getMoreData();

        /**
         * @brief waitForResult
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        void waitForResult();

    public Q_SLOTS:
        /**
         * @brief _ar_cursor_result_loaded
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        void _ar_cursor_result_loaded();

    Q_SIGNALS:
        /**
         * @brief ready
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        void ready();

        /**
         * @brief error
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        void error();

    protected:
        QBCursorPrivate *d_ptr;

    private:
        Q_DECLARE_PRIVATE(QBCursor)
};

}

#endif // QBCURSOR_H
