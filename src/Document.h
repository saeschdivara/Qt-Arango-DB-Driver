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
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        explicit Document(QObject *parent = 0);

        /**
         * @brief Document
         *
         * @param collection
         * @param parent
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
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
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        Document(QString collection, QString key, QObject *parent = 0);

        /**
         * @brief Document
         *
         * @param obj
         * @param parent
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        Document(QJsonObject obj, QObject * parent = 0);

        /**
         * @brief ~Document
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        virtual ~Document();

        /**
         * @brief isReady
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        bool isReady();

        /**
         * @brief Returns true if the Document knows that is has
         * a version of itself saved in the database
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        bool isCreated();

        /**
         * @brief Returns true if the documents revision is the current one
         * it gets only updated the updateStatus is triggered
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        bool isCurrent();

        /**
         * @brief toJsonString
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        virtual QByteArray toJsonString() const;

        /**
         * @brief docID
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        QString docID() const;

        /**
         * @brief Returns the unique key within the Collection
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        QString key() const;

        /**
         * @brief Returns the revision of the Document
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        QString rev() const;

        /**
         * @brief Returns the collection name
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        QString collection() const;

        /**
         * @brief If this returns false, it cannot
         * be saved or synchronised to a collection.
         * It means that its data is a collection of
         * different collection fields.
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        bool isStoredInCollection();

        /**
         * @brief If an error has occured, it will return
         * the error message
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        QString errorMessage() const;

        /**
         * @brief errorCode
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        quint32 errorCode();

        /**
         * @brief errorCode
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        quint32 errorNumber();

        /**
         * @brief hasErrorOccurred
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        bool hasErrorOccurred();

        /**
         * @brief set
         *
         * @param name
         * @param data
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        void set(const QString &key, QVariant data);

        /**
         * @brief get
         *
         * @param name
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
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
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        bool contains(const QString & key) const;

        /**
         * @brief dirtyAttributes
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        QStringList dirtyAttributes() const;

        /**
         * @brief isEveryAttributeDirty
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        bool isEveryAttributeDirty() const;

        /**
         * @brief waitForResult
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.4
         */
        void waitForResult();

        /**
         * @brief deleteAfterFinished
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
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
         * @brief updateData
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void updateData(Document *);

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
         * @brief update
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void update();

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
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
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
