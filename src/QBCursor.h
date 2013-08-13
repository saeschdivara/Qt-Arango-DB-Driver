#ifndef QCURSOR_H
#define QCURSOR_H

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

#endif // QCURSOR_H
