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

#ifndef ABSTRACTINDEX_H
#define ABSTRACTINDEX_H

#include "arangodb-driver_global.h"

namespace arangodb
{

class Collection;

namespace index
{

class AbstractIndexPrivate;

/**
 * @brief Enum which describes all available
 * index types
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.6
 */
enum class IndexType {
    UnknownIndex    = 0,
    CapIndex        = 1,
    GeoIndex        = 2,
    HashIndex       = 3,
    FulltextIndex   = 4,
    SkipListIndex   = 5
};

/**
 * @brief Abstract class for all index classes
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.6
 */
class ARANGODBDRIVERSHARED_EXPORT AbstractIndex : public QObject
{
        Q_OBJECT
    public:
        // Normal public methods
        /**
         * @brief Deletes d_ptr
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual ~AbstractIndex();

        /**
         * @brief Returns the unique id of the index
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        QString id() const;

        /**
         * @brief Returns the name/type of the index
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual QString name() const = 0;

        /**
         * @brief Returns the collection for which
         * the index is used for
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        Collection* collection() const;

        /**
         * @brief Returns a json representation of the
         * index
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual QByteArray toJson() const = 0;

        /**
         * @brief Returns only after the index data is loaded
         * and is therefor ready or an error occured
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void waitUntilReady();

        /**
         * @brief Returns only after the index data is deleted
         * and therefor the object useless is or an error occured
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void waitUntilDeleted();

        /**
         * @brief Returns if during a network operation or through
         * a database request an error has occured
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        bool hasErrorOccurred() const;

        /**
         * @brief If an error has occured, it returns the error
         * number, else it returns 0
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        int errorCode() const;

        /**
         * @brief If an error has occured, it returns the error
         * message, else it returns an empty string
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        QString errorMessage() const;

        /**
         * @brief isNewlyCreated
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        bool isNewlyCreated() const;

    public Q_SLOTS:

        /**
         * @brief Creates index in database collection
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void save();

        /**
         * @brief Deletes the index in the database
         * from the collection
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void deleteInDatabase();

        /**
         * @brief This method is should not be triggered
         * by anything else but from ArangodbDriver methods because
         * as sender will be a QNetworkReply which holds the content
         * of the save request results
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void _ar_saveRequestFinished();

    Q_SIGNALS:

        /**
         * @brief This signal is emited when a request
         * has successfully ended
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void ready();

        /**
         * @brief This signal is emited when the index
         * has been successfully deleted in the database
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void deleted();

        /**
         * @brief This signal is emited when an error occured
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void error();

        /**
         * @brief This signal is emited if AbstractIndex::save
         * was triggered
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void saveSignal(AbstractIndex *);

        /**
         * @brief This signal is emited if AbstractIndex::deleteInDatabase
         * was triggered
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void deleteSignal(AbstractIndex *);

    protected:
        AbstractIndexPrivate * d_ptr;

        /**
         * @brief AbstractIndex
         *
         * @param collection
         * @param d
         * @param parent
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        AbstractIndex(Collection * collection, AbstractIndexPrivate *d, QObject * parent);

    private:
        Q_DECLARE_PRIVATE(AbstractIndex)
};

}
}

#endif // ABSTRACTINDEX
