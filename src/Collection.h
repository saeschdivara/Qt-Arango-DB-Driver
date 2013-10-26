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

#ifndef COLLECTION_H
#define COLLECTION_H

#include "arangodb-driver_global.h"
#include "QBCursor.h"
#include "index/AbstractIndex.h"

#include <QtCore/QJsonObject>
#include <QtCore/QString>

namespace arangodb
{

class Document;

class CollectionPrivate;

/**
 * @brief The Collection class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.1
 */
class ARANGODBDRIVERSHARED_EXPORT Collection : public QObject
{
        Q_OBJECT
    public:

        /**
         * @brief The KeyOption struct
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        struct KeyOption {
                /**
                 * @brief The KeyGeneratorType enum
                 *
                 * @author Sascha Häusler <saeschdivara@gmail.com>
                 * @since 0.5
                 */
                enum class KeyGeneratorType {
                    TraditionalType = 1,
                    AutoIncrementType = 2
                };

                /**
                 * @brief type specifies the type of the key generator
                 *
                 * @author Sascha Häusler <saeschdivara@gmail.com>
                 * @since 0.5
                 */
                KeyGeneratorType type;

                /**
                 * @brief if set to true, then it is allowed to supply own
                 * key values in the _key attribute of a document. If set to false, then
                 * the key generator will solely be responsible for generating keys and
                 * supplying own key values in the _key attribute of documents is con-
                 * sidered an error.
                 *
                 * @author Sascha Häusler <saeschdivara@gmail.com>
                 * @since 0.5
                 */
                bool allowUserKeys;

                /**
                 * @brief increment value for autoincrement key generator.
                 * Not used for other key generator types.
                 *
                 * @author Sascha Häusler <saeschdivara@gmail.com>
                 * @since 0.5
                 */
                uint increment;

                /**
                 * @brief initial offset value for autoincrement key generator. Not
                 * used for other key generator types.
                 *
                 * @author Sascha Häusler <saeschdivara@gmail.com>
                 * @since 0.5
                 */
                uint offset;

                /**
                 * @brief object
                 *
                 * @return
                 *
                 * @author Sascha Häusler <saeschdivara@gmail.com>
                 * @since 0.5
                 */
                QJsonObject object() {
                    QJsonObject obj;

                    if ( type == KeyGeneratorType::TraditionalType ) {
                        obj.insert(QString("type"), QString("traditional"));
                    }
                    else if ( type == KeyGeneratorType::AutoIncrementType ) {
                        obj.insert(QString("type"), QString("autoincrement"));
                    }

                    obj.insert("allowUserKeys", allowUserKeys);
                    obj.insert(QString("increment"), QJsonValue(int(increment)));
                    obj.insert(QString("offset"), QJsonValue(int(offset)));

                    return obj;
                }
        };

        /**
         * @brief The Type enum
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        enum class Type {
            DocumentType = 2,
            EdgesType = 3
        };

        /**
         * @brief The CreateOptions struct
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        struct CreateOptions {
                /**
                 * @brief Flag if the data is written to the disk before returning
                 */
                bool waitForSync;

                /**
                 * @brief journalSize
                 */
                int journalSize;

                /**
                 * @brief Flag if the collection is a system collection
                 */
                bool isSystem;

                /**
                 * @brief Flag if the collection is never persisted (and only in-memory)
                 */
                bool isVolatile;

                /**
                 * @brief Options for keys of the collection
                 */
                Collection::KeyOption * keyOption;

                /**
                 * @brief Collection type (Document|Edge)
                 */
                Collection::Type type;
        };

        /**
         * @brief Collection
         *
         * @param name
         * @param parent
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.1
         */
        explicit Collection(const QString & name, QObject * parent = 0);

        /**
         * @brief Collection
         *
         * @param name
         * @param waitForSync
         * @param journalSize
         * @param isSystem
         * @param isVolatile
         * @param keyOption
         * @param type
         * @param parent
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        explicit Collection(const QString & name,
                            bool waitForSync,
                            int journalSize,
                            bool isSystem,
                            bool isVolatile,
                            Collection::KeyOption * keyOption,
                            Collection::Type type,
                            QObject *parent = 0);

        /**
         * @brief Collection
         *
         * @param name
         * @param options
         * @param parent
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        explicit Collection(const QString & name,
                            CreateOptions options,
                            QObject * parent = 0);

        /**
         * @brief ~Collection
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        virtual ~Collection();

        /**
         * @brief isReady
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        bool isReady();

        /**
         * @brief isCreated
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        bool isCreated();

        /**
         * @brief id
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        QString id() const;

        /**
         * @brief name
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        QString name() const;

        /**
         * @brief Returns the type of the collection
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        Collection::Type type() const;

        /**
         * @brief waitForSync (optional, default: false): If true then the data is synchro-
         * nised to disk before returning from a create or update of an document.
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        bool isWaitingForSync();

        /**
         * @brief journalSize (optional, default is a configuration parameter (p. ??))-
         * : The maximal size of a journal or datafile. Note that this also limits the
         * maximal size of a single object. Must be at least 1MB.
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        int journalSize();

        /**
         * @brief isSystem (optional, default is false): If true, create a system collec-
         * tion. In this case collection-name should start with an underscore. End
         * users should normally create non-system collections only. API implemen-
         * tors may be required to create system collections in very special occasions,
         * but normally a regular collection will do.
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        bool isSystem();

        /**
         * @brief isVolatile (optional, default is false): If true then the collection
         * data is kept in-memory only and not made persistent. Unloading the collec-
         * tion will cause the collection data to be discarded. Stopping or re-starting the
         * server will also cause full loss of data in the collection. Setting this option
         * will make the resulting collection be slightly faster than regular collections
         * because ArangoDB does not enforce any synchronisation to disk and does
         * not calculate any CRC checksums for datafiles (as there are no datafiles).
         * This option should threrefore be used for cache-type collections only, and
         * not for data that cannot be re-created otherwise.
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        bool isVolatile();

        /**
         * @brief keyOptions (optional) additional options for key generation. If speci-
         * fied, then keyOptions should be a JSON array containing the following
         * attributes (note: some of them are optional):
         *      – type: specifies the type of the key generator. The currently available
         *        generators are traditional and autoincrement.
         *
         *      – allowUserKeys: if set to true, then it is allowed to supply own
         *        key values in the _key attribute of a document. If set to false, then
         *        the key generator will solely be responsible for generating keys and
         *        supplying own key values in the _key attribute of documents is con-
         *        sidered an error.
         *
         *      – increment: increment value for autoincrement key generator.
         *        Not used for other key generator types.
         *
         *      – offset: initial offset value for autoincrement key generator. Not
         *        used for other key generator types.
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        KeyOption * keyOption() const;

        /**
         * @brief errorMessage
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        QString errorMessage() const;

        /**
         * @brief errorCode
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        quint32 errorCode();

        /**
         * @brief errorCode
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        quint32 errorNumber();

        /**
         * @brief hasErrorOccurred
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        bool hasErrorOccurred();

        /**
         * @brief createDocument
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        Document * createDocument();

        /**
         * @brief Returns QBCursor, when it emits the
         * QBCursor::ready signal, all data is loaded
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        QSharedPointer<QBCursor> getAllDocuments();

        /**
         * @brief createDocument
         *
         * @param key
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        Document * createDocument(const QString & key);

        /**
         * @brief createIndex
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        index::AbstractIndex * createIndex(index::IndexType type);

        /**
         * @brief save
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void save();

        /**
         * @brief deleteAll
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void deleteAll();

        /**
         * @brief Loads collection into memory
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void load();

        /**
         * @brief Waits until either the ready or the error
         * signal has been emitted
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void waitUntilReady();

        /**
         * @brief Waits until the collection is loaded
         * into memory or an error occurs
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void waitUntilLoaded();

        /**
         * @brief Waits until the collection is deleted
         * or an error occurs
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void waitUntilDeleted();

        /**
         * @brief Serializes all data and returns it
         * in the json format
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        QByteArray toJsonString();
        
    Q_SIGNALS:
        /**
         * @brief ready
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void ready();

        /**
         * @brief deleted
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void deleted();

        /**
         * @brief loaded
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void loaded();

        /**
         * @brief error
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void error();

        /**
         * @brief saveData
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void saveData(Collection *);

        /**
         * @brief deleteData
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void deleteData(Collection *);

        /**
         * @brief loadIntoMemory
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void loadIntoMemory(Collection *);
        
    public Q_SLOTS:
        /**
         * @brief _ar_dataIsAvailable
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void _ar_dataIsAvailable();

        /**
         * @brief _ar_loaded
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void _ar_loaded();

        /**
         * @brief _ar_isDeleted
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void _ar_isDeleted();

    protected:
        CollectionPrivate * d_ptr;

    private:
        Q_DECLARE_PRIVATE(Collection)
        
};

}

#endif // COLLECTION_H
