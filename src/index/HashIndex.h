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

#ifndef HASHINDEX_H
#define HASHINDEX_H

#include "IndexInterface.h"

#include <QtCore/QString>

namespace arangodb
{
namespace index
{

class HashIndexPrivate;

/**
 * @brief The HashIndex class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.6
 */
class ARANGODBDRIVERSHARED_EXPORT HashIndex : public QObject, public IndexInterface
{
        Q_OBJECT
    public:
        explicit HashIndex(Collection * collection, QObject *parent = 0);

        // IndexInterface interface
        /**
         * @brief Returns the unique id of the index
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual QString id() const Q_DECL_OVERRIDE;

        /**
         * @brief Returns the name/type of the index
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual QString name() const Q_DECL_OVERRIDE;

        /**
         * @brief Returns the collection for which
         * the index is used for
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual Collection* collection() const Q_DECL_OVERRIDE;

        /**
         * @brief Returns a json representation of the
         * index
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual QByteArray toJson() const Q_DECL_OVERRIDE;

        /**
         * @brief Returns only after the index data is loaded
         * and is therefor ready or an error occured
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual void waitUntilReady() Q_DECL_OVERRIDE;

        /**
         * @brief Returns only after the index data is deleted
         * and therefor the object useless is or an error occured
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual void waitUntilDeleted() Q_DECL_OVERRIDE;

        /**
         * @brief Returns if during a network operation or through
         * a database request an error has occured
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual bool hasErrorOccurred() const Q_DECL_OVERRIDE;

        /**
         * @brief If an error has occured, it returns the error
         * number, else it returns 0
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual int errorCode() const Q_DECL_OVERRIDE;

        /**
         * @brief If an error has occured, it returns the error
         * message, else it returns an empty string
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual QString errorMessage() const Q_DECL_OVERRIDE;

        /**
         * @brief Returns true if the index has not exists
         * before
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual bool isNewlyCreated() const Q_DECL_OVERRIDE;

    public Q_SLOTS:
        /**
         * @brief Creates index in database collection
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual void save() Q_DECL_OVERRIDE;

        /**
         * @brief Deletes the index in the database
         * from the collection
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual void deleteInDatabase() Q_DECL_OVERRIDE;

        /**
         * @brief This method is should not be triggered
         * by anything else but from ArangodbDriver methods because
         * as sender will be a QNetworkReply which holds the content
         * of the save request results
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual void _ar_saveRequestFinished() Q_DECL_OVERRIDE;

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
         * @brief This signal is emited if HashIndex::save
         * was triggered
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void saveSignal(IndexInterface *);

        /**
         * @brief This signal is emited if HashIndex::deleteInDatabase
         * was triggered
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void deleteSignal(IndexInterface *);

    protected:
        HashIndexPrivate *d_ptr;

    private:
        Q_DECLARE_PRIVATE(HashIndex)
};

}
}

#endif // HASHINDEX_H
