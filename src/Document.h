#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "arangodb-driver_global.h"
#include <QtCore/QByteArray>
#include <QtCore/QStringList>

namespace internal {
class DocumentPrivate;
}

namespace arangodb
{

/**
 * @brief The Document class
 *
 * @since 0.1
 */
class ARANGODBDRIVERSHARED_EXPORT Document : public QObject
{
        Q_OBJECT
    public:
        /**
         * @brief Document
         *
         * @param parent
         *
         * @since 0.1
         */
        explicit Document(QObject *parent = 0);

        /**
         * @brief Document
         *
         * @param collection
         * @param parent
         *
         * @since 0.1
         */
        explicit Document(QString collection, QObject *parent = 0);

        /**
         * @brief ~Document
         *
         * @since 0.1
         */
        virtual ~Document();

        /**
         * @brief isReady
         *
         * @return
         *
         * @since 0.1
         */
        bool isReady();

        /**
         * @brief isCreated
         *
         * @return
         *
         * @since 0.1
         */
        bool isCreated();

        /**
         * @brief toJsonString
         *
         * @return
         *
         * @since 0.1
         */
        QByteArray toJsonString() const;

        /**
         * @brief docID
         *
         * @return
         *
         * @since 0.1
         */
        QString docID() const;

        /**
         * @brief key
         *
         * @return
         *
         * @since 0.1
         */
        QString key() const;

        /**
         * @brief rev
         *
         * @return
         *
         * @since 0.1
         */
        QString rev() const;

        /**
         * @brief collection
         *
         * @return
         *
         * @since 0.1
         */
        QString collection() const;

        /**
         * @brief errorMessage
         *
         * @return
         *
         * @since 0.1
         */
        QString errorMessage() const;

        /**
         * @brief errorCode
         *
         * @return
         *
         * @since 0.1
         */
        quint32 errorCode();

        /**
         * @brief errorCode
         *
         * @return
         *
         * @since 0.1
         */
        quint32 errorNumber();

        /**
         * @brief hasErrorOccurred
         *
         * @return
         *
         * @since 0.1
         */
        bool hasErrorOccurred();

        /**
         * @brief set
         *
         * @param name
         * @param data
         *
         * @since 0.1
         */
        void set(const QString &key, QVariant data);

        /**
         * @brief get
         *
         * @param name
         * @return
         *
         * @since 0.1
         */
        QVariant get(const QString &key) const;

        /**
         * @brief dirtyAttributes
         *
         * @return
         *
         * @since 0.1
         */
        QStringList dirtyAttributes() const;

        /**
         * @brief isEveryAttributeDirty
         * @return
         */
        bool isEveryAttributeDirty() const;

    Q_SIGNALS:
        /**
         * @brief ready
         *
         * @since 0.1
         */
        void ready();

        /**
         * @brief error
         *
         * @since 0.1
         */
        void error();

        /**
         * @brief save
         *
         * @since 0.1
         */
        void saveData(Document *);

        /**
         * @brief deleteData
         *
         * @since 0.1
         */
        void deleteData(Document *);

        /**
         * @brief dataDeleted
         *
         * @since 0.1
         */
        void dataDeleted();

    public Q_SLOTS:
        /**
         * @brief save
         *
         * @since 0.1
         */
        void save();

        /**
         * @brief drop
         *
         * @since 0.1
         */
        void drop();

        /**
         * @brief _ar_dataIsAvailable
         *
         * @since 0.1
         */
        void _ar_dataIsAvailable();

        /**
         * @brief _ar_dataDeleted
         *
         * @since 0.1
         */
        void _ar_dataDeleted();

    private:
        internal::DocumentPrivate *d;
};

}

#endif // DOCUMENT_H
