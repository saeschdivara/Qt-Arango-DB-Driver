#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "arangodb-driver_global.h"

#include <QtCore/QByteArray>
#include <QtCore/QJsonObject>
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
        Document(QString collection, QObject *parent = 0);

        /**
         * @brief Document
         *
         * @param collection
         * @param key
         * @param parent
         *
         * @since 0.3
         */
        Document(QString collection, QString key, QObject *parent = 0);

        /**
         * @brief Document
         *
         * @param obj
         * @param parent
         *
         * @since 0.4
         */
        Document(QJsonObject obj, QObject * parent = 0);

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
         * @brief Returns true if the documents revision is the current one
         * it gets only updated the updateStatus is triggered
         *
         * @return
         *
         * @since 0.1
         */
        bool isCurrent();

        /**
         * @brief toJsonString
         *
         * @return
         *
         * @since 0.1
         */
        virtual QByteArray toJsonString() const;

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
         * @brief contains
         *
         * @param key
         *
         * @return
         *
         * @since 0.4
         */
        bool contains(const QString & key) const;

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
         *
         * @return
         *
         * @since 0.1
         */
        bool isEveryAttributeDirty() const;

        /**
         * @brief waitForResult
         *
         * @since 0.4
         */
        void waitForResult();

        /**
         * @brief deleteAfterFinished
         *
         * @since 0.5
         */
        void deleteAfterFinished();

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
         * @brief updateDataStatus
         *
         * @since 0.1
         */
        void updateDataStatus(Document *);

        /**
         * @brief syncData
         *
         * @since 0.3
         */
        void syncData(Document *);

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
         * @brief sync
         *
         * @since 0.3
         */
        void sync();

        /**
         * @brief drop
         *
         * @since 0.1
         */
        void drop();

        /**
         * @brief updateStatus
         *
         * @since 0.1
         */
        void updateStatus();

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

        /**
         * @brief _ar_dataUpdated
         */
        void _ar_dataUpdated();

    protected:
        internal::DocumentPrivate* d_ptr;

        /**
         * @brief Document
         *
         * @param d
         * @param parent
         *
         * @since 0.1
         */
        Document(internal::DocumentPrivate *privatePointer, QObject *parent = 0);

        /**
         * @brief Document
         *
         * @param privatePointer
         * @param collection
         * @param parent
         *
         * @since 0.1
         */
        Document(internal::DocumentPrivate *privatePointer, QString collection, QObject *parent = 0);

    private:
        Q_DECLARE_PRIVATE(internal::Document)
};

}

#endif // DOCUMENT_H
