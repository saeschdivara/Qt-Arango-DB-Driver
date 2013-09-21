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

#ifndef ARANGODBDRIVER_H
#define ARANGODBDRIVER_H

#include "arangodb-driver_global.h"

#include "Collection.h"
#include "Document.h"
#include "Edge.h"
#include "QBSelect.h"
#include "QBSimpleSelect.h"
#include "QBCursor.h"
#include "index/AbstractIndex.h"

#include <QtCore/QSharedPointer>

namespace internal {
class ArangodbdriverPrivate;
}

namespace arangodb
{

using namespace index;

/**
 * @brief The Arangodbdriver class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
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
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        Arangodbdriver(QString protocol = QString("http"),
                       QString host = QString("localhost"),
                       qint32 port = 8529);

        /**
         * @brief ~Arangodbdriver
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        virtual ~Arangodbdriver();

        /**
         * @brief existsCollection
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        bool isColllectionExisting(const QString & collectionName);

        /**
         * @brief getCollection
         *
         * @param name
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        Collection* getCollection(QString name);

        /**
         * @brief createCollection
         *
         * @param name
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        Collection * createCollection(const QString & name,
                                      bool waitForSync = false,
                                      int journalSize = -1,
                                      bool isSystem = false,
                                      bool isVolatile = false,
                                      Collection::KeyOption * keyOption = Q_NULLPTR,
                                      Collection::Type type = Collection::Type::DocumentType);

        /**
         * @brief connectCollection
         *
         * @param collection
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void connectCollection(Collection * collection);

        /**
         * @brief getDocument
         *
         * @param id
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        Document* getDocument(QString id);

        /**
         * @brief Creates document for a collection
         *
         * @param collection
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        Document* createDocument(QString collection);

        /**
         * @brief Creates a document for a collection
         * and with a specific key
         *
         * @param collection
         * @param key
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        Document* createDocument(QString collection, QString key);

        /**
         * @brief connectDocument
         *
         * @param doc
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        void connectDocument(Document * doc);

        /**
         * @brief getEdge
         *
         * @param id
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        Edge* getEdge(QString id);

        /**
         * @brief createEdge
         *
         * @param collection
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        Edge* createEdge(QString collection, Document *fromDoc, Document *toDoc);

        /**
         * @brief connectIndex
         * @param index
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void connectIndex(index::AbstractIndex* index);

        // TODO: Create interface (QBQueryInterface) for QBSelect and QBSimpleSelect
        // TODO: Rename QBSimpleSelect to QBSimpleQuery
        /**
         * @brief executeSelect
         *
         * @param select
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        QSharedPointer<QBCursor> executeSelect(QSharedPointer<QBSelect> select);

        /**
         * @brief executeSelect
         *
         * @param select
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        QSharedPointer<QBCursor> executeSelect(QSharedPointer<QBSimpleSelect> select);

        /**
         * @brief loadMoreResults
         *
         * @param cursor
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        void loadMoreResults(QBCursor * cursor);

        /**
         * @brief Variadic template method to wait for an
         * unlimited number of Document's, Collection's
         * and Edge's objects until they are ready
         * or an error occured
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void waitUntilFinished();

        /**
         * @brief Variadic template method to wait for an
         * unlimited number of Collection's
         * objects until they are ready or an error occured
         *
         * @param collection    Collection
         * @param others        Variadic template parameter
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        template<typename ... OtherTypes>
        void waitUntilFinished(Collection * collection, OtherTypes ... others) {
            privateWaitUntilFinished(collection);
            waitUntilFinished(others...);
        }

        /**
         * @brief Variadic template method to wait for an
         * unlimited number of Document's
         * objects until they are ready or an error occured
         *
         * @param document      Document
         * @param others        Variadic template parameter
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        template<typename ... OtherTypes>
        void waitUntilFinished(Document * document, OtherTypes ... others) {
            privateWaitUntilFinished(document);
            waitUntilFinished(others...);
        }

    protected:
        void privateWaitUntilFinished(Collection * collection);
        void privateWaitUntilFinished(Document * document);

    protected Q_SLOTS:
        /**
         * @brief _ar_document_save
         *
         * @param doc
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        void _ar_document_save(Document *doc);

        /**
         * @brief _ar_document_update
         *
         * @param doc
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void _ar_document_update(Document *doc);

        /**
         * @brief _ar_document_delete
         *
         * @param doc
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        void _ar_document_delete(Document *doc);

        /**
         * @brief _ar_document_updateStatus
         *
         * @param doc
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        void _ar_document_updateStatus(Document *doc);

        /**
         * @brief _ar_document_sync
         *
         * @param doc
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        void _ar_document_sync(Document *doc);

        /**
         * @brief _ar_edge_save
         *
         * @param doc
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        void _ar_edge_save(Document *doc);

        /**
         * @brief _ar_edge_delete
         *
         * @param doc
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        void _ar_edge_delete(Document *doc);

        /**
         * @brief _ar_collection_save
         *
         * @param collection
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void _ar_collection_save(Collection * collection);

        /**
         * @brief _ar_collection_load
         *
         * @param collection
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void _ar_collection_load(Collection * collection);

        /**
         * @brief _ar_collection_delete
         *
         * @param collection
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void _ar_collection_delete(Collection * collection);

        /**
         * @brief Saves the index
         *
         * @param index
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void _ar_index_save(AbstractIndex * index);

        /**
         * @brief Deletes the index from the collection
         *
         * @param index
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void _ar_index_delete(AbstractIndex * index);

    private:
        internal::ArangodbdriverPrivate *d;
};

}

#endif // ARANGODBDRIVER_H
