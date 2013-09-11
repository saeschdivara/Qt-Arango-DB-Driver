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

#ifndef INDEXINTERFACE_H
#define INDEXINTERFACE_H

#include "arangodb-driver_global.h"

namespace arangodb
{

class Collection;

namespace index
{

enum class IndexType {
    UnknownIndex    = 0,
    CapIndex        = 1,
    GeoIndex        = 2,
    HashIndex       = 3,
    FulltextIndex   = 4,
    SkipListIndex   = 5
};

/**
 * @brief The IndexInterface class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.6
 */
class ARANGODBDRIVERSHARED_EXPORT IndexInterface
{
    public:
        // Normal public methods

        /**
         * @brief Returns the unique id of the index
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual QString id() const = 0;

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
        virtual Collection* collection() const = 0;

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
        virtual void waitUntilReady() = 0;

        /**
         * @brief Returns only after the index data is deleted
         * and therefor the object useless is or an error occured
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual void waitUntilDeleted() = 0;

        /**
         * @brief Returns if during a network operation or through
         * a database request an error has occured
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual bool hasErrorOccurred() const = 0;

        /**
         * @brief If an error has occured, it returns the error
         * number, else it returns 0
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual int errorCode() const = 0;

        /**
         * @brief If an error has occured, it returns the error
         * message, else it returns an empty string
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual QString errorMessage() const = 0;

        /**
         * @brief isNewlyCreated
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual bool isNewlyCreated() const = 0;

        // Slots
        // ----------------------------------------
        // They can be overloaded just like normal
        // methods

        /**
         * @brief Creates index in database collection
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual void save() = 0;

        /**
         * @brief Deletes the index in the database
         * from the collection
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual void deleteInDatabase() = 0;

        /**
         * @brief This method is should not be triggered
         * by anything else but from ArangodbDriver methods because
         * as sender will be a QNetworkReply which holds the content
         * of the save request results
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual void _ar_saveRequestFinished() = 0;

        // Signals
        // -----------------------------------------
        // They can neither be pure nor virtual but
        // the declaration here insures that every
        // class which implements the rest, needs to define it

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
         * @brief This signal is emited if IndexInterface::save
         * was triggered
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void saveSignal(IndexInterface *);

        /**
         * @brief This signal is emited if IndexInterface::deleteInDatabase
         * was triggered
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void deleteSignal(IndexInterface *);
};

}
}

#endif // INDEXINTERFACE_H
