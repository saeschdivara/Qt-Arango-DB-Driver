#ifndef COLLECTION_H
#define COLLECTION_H

#include "arangodb-driver_global.h"
#include <QtCore/QString>

namespace arangodb
{

class CollectionPrivate;

/**
 * @brief The Collection class
 *
 * @since 0.1
 */
class ARANGODBDRIVERSHARED_EXPORT Collection : public QObject
{
        Q_OBJECT
    public:
        /**
         * @brief Collection
         *
         * @param name
         * @param parent
         *
         * @since 0.1
         */
        explicit Collection(QString name, QObject *parent = 0);

        /**
         * @brief ~Collection
         *
         * @since 0.5
         */
        virtual ~Collection();

        /**
         * @brief isReady
         *
         * @return
         *
         * @since 0.5
         */
        bool isReady();

        /**
         * @brief isCreated
         *
         * @return
         *
         * @since 0.5
         */
        bool isCreated();

        /**
         * @brief name
         *
         * @return
         *
         * @since 0.5
         */
        QString name() const;

        /**
         * @brief errorMessage
         *
         * @return
         *
         * @since 0.5
         */
        QString errorMessage() const;

        /**
         * @brief errorCode
         *
         * @return
         *
         * @since 0.5
         */
        quint32 errorCode();

        /**
         * @brief errorCode
         *
         * @return
         *
         * @since 0.5
         */
        quint32 errorNumber();

        /**
         * @brief hasErrorOccurred
         *
         * @return
         *
         * @since 0.5
         */
        bool hasErrorOccurred();
        
    signals:
        /**
         * @brief ready
         *
         * @since 0.5
         */
        void ready();

        /**
         * @brief error
         *
         * @since 0.5
         */
        void error();

        /**
         * @brief saveData
         *
         * @since 0.5
         */
        void saveData(Collection *);
        
    public slots:
        /**
         * @brief _ar_dataIsAvailable
         *
         * @since 0.5
         */
        void _ar_dataIsAvailable();

    protected:
        CollectionPrivate * d_ptr;

    private:
        Q_DECLARE_PRIVATE(Collection)
        
};

}

#endif // COLLECTION_H
