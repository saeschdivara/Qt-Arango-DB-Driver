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
         * @brief set
         * @param name
         * @param data
         */
        void set(const QString &key, QVariant data);

        /**
         * @brief get
         * @param name
         * @return
         */
        QVariant get(const QString &key) const;

    Q_SIGNALS:
        /**
         * @brief ready
         */
        void ready();

        /**
         * @brief save
         */
        void saveData(Document *);

    public Q_SLOTS:
        /**
         * @brief save
         */
        void save();

        /**
         * @brief _ar_dataIsAvailable
         */
        void _ar_dataIsAvailable();

    private:
        internal::DocumentPrivate *d;
};

}

#endif // DOCUMENT_H
