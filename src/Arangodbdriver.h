#ifndef ARANGODBDRIVER_H
#define ARANGODBDRIVER_H

#include "arangodb-driver_global.h"
#include "Collection.h"
#include "Document.h"
#include "Edge.h"

namespace internal {
class ArangodbdriverPrivate;
}

namespace arangodb
{

/**
 * @brief The Arangodbdriver class
 *
 * @since 0.1
 */
class ARANGODBDRIVERSHARED_EXPORT Arangodbdriver : public QObject
{
        Q_OBJECT
    public:
        /**
         * @brief Arangodbdriver
         *
         * @param protocol
         * @param host
         * @param port
         *
         * @since 0.1
         */
        Arangodbdriver(QString protocol = QString("http"),
                       QString host = QString("localhost"),
                       qint32 port = 8529);

        /**
         * @brief ~Arangodbdriver
         *
         * @since 0.1
         */
        virtual ~Arangodbdriver();

        /**
         * @brief getCollection
         *
         * @param name
         * @return
         *
         * @since 0.1
         */
        Collection* getCollection(QString name);

        /**
         * @brief getDocument
         *
         * @param id
         * @return
         *
         * @since 0.1
         */
        Document* getDocument(QString id);

        /**
         * @brief createDocument
         *
         * @param collection
         * @return
         *
         * @since 0.1
         */
        Document* createDocument(QString collection);

        /**
         * @brief getEdge
         *
         * @param id
         * @return
         *
         * @since 0.1
         */
        Edge* getEdge(QString id);

        /**
         * @brief createEdge
         *
         * @param collection
         * @return
         *
         * @since 0.1
         */
        Edge* createEdge(QString collection, Document *fromDoc, Document *toDoc);

    protected Q_SLOTS:
        /**
         * @brief _ar_document_save
         *
         * @param doc
         *
         * @since 0.1
         */
        void _ar_document_save(Document *doc);

        /**
         * @brief _ar_document_delete
         *
         * @param doc
         *
         * @since 0.1
         */
        void _ar_document_delete(Document *doc);

        /**
         * @brief _ar_document_updateStatus
         *
         * @param doc
         *
         * @since 0.1
         */
        void _ar_document_updateStatus(Document *doc);

        /**
         * @brief _ar_edge_save
         *
         * @param doc
         *
         * @since 0.1
         */
        void _ar_edge_save(Document *doc);

        /**
         * @brief _ar_edge_delete
         *
         * @param doc
         *
         * @since 0.1
         */
        void _ar_edge_delete(Document *doc);

    private:
        internal::ArangodbdriverPrivate *d;
};

}

#endif // ARANGODBDRIVER_H
