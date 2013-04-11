#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "arangodb-driver_global.h"
#include <QtCore/QByteArray>

namespace internal {
class DocumentPrivate;
}

namespace arangodb
{

/**
 * @brief The Document class
 */
class ARANGODBDRIVERSHARED_EXPORT Document : public QObject
{
        Q_OBJECT
    public:
        /**
         * @brief Document
         * @param parent
         */
        explicit Document(QObject *parent = 0);

        /**
         * @brief Document
         * @param collection
         * @param parent
         */
        explicit Document(QString collection, QObject *parent = 0);

        /**
         * @brief ~Document
         */
        virtual ~Document();

        /**
         * @brief isReady
         * @return
         */
        bool isReady();

        /**
         * @brief isCreated
         * @return
         */
        bool isCreated();

        /**
         * @brief toJsonString
         * @return
         */
        QByteArray toJsonString() const;

        /**
         * @brief docID
         * @return
         */
        QString docID() const;

        /**
         * @brief key
         * @return
         */
        QString key() const;

        /**
         * @brief rev
         * @return
         */
        QString rev() const;

        /**
         * @brief collection
         * @return
         */
        QString collection() const;

        /**
         * @brief errorMessage
         * @return
         */
        QString errorMessage() const;

        /**
         * @brief errorCode
         * @return
         */
        quint32 errorCode();

        /**
         * @brief errorCode
         * @return
         */
        quint32 errorNumber();

        /**
         * @brief set
         * @param name
         * @param data
         */
        void Set(const QString &key, QVariant data);

        /**
         * @brief get
         * @param name
         * @return
         */
        QVariant Get(const QString &key) const;

    Q_SIGNALS:
        /**
         * @brief ready
         */
        void ready();

        /**
         * @brief error
         */
        void error();

        /**
         * @brief save
         */
        void saveData(Document *);

        /**
         * @brief deleteData
         */
        void deleteData(Document *);

        /**
         * @brief dataDeleted
         */
        void dataDeleted();

    public Q_SLOTS:
        /**
         * @brief save
         */
        void Save();

        /**
         * @brief Delete
         */
        void Delete();

        /**
         * @brief _ar_dataIsAvailable
         */
        void _ar_dataIsAvailable();

        /**
         * @brief _ar_dataDeleted
         */
        void _ar_dataDeleted();

    private:
        internal::DocumentPrivate *d;
};

}

#endif // DOCUMENT_H
