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

#ifndef QBSELECT_H
#define QBSELECT_H

#include "arangodb-driver_global.h"
#include <QStringList>

namespace arangodb
{

class QBSelectPrivate;

/**
 * @brief This class represents selects which has
 * little methods to modify it
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.3
 */
class ARANGODBDRIVERSHARED_EXPORT QBSelect
{
    public:
        /**
         * @brief QBSelect
         *
         * @param collection
         * @param batchSize
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        QBSelect(const QString & collection, int batchSize);

        /**
         * @brief QBSelect
         *
         * @param collections
         * @param batchSize
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        QBSelect(const QStringList & collections, int batchSize);
        ~QBSelect();

        /**
         * @brief addNewCollection
         *
         * @param collection
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void addNewCollection(const QString & collection);

        /**
         * @brief collection
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        QStringList collections() const;

        /**
         * @brief batchSize
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        int batchSize() const;

        /**
         * @brief setCounting
         *
         * @param c
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        void setCounting(bool c);

        /**
         * @brief isCounting
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        bool isCounting() const;

        /**
         * @brief This method assumes that only one collection
         * is set for the select and therefor the where statement
         * is set for this collection (first collection in the list)
         *
         * @param field
         * @param op
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        void setWhere(const QString & field, const QString & op);

        /**
         * @brief Set the where conditions for one collection
         * field but gives multiple choices.
         *
         * @param field
         * @param op
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        void setWhere(const QString & field, const QStringList & op);

        /**
         * @brief setResult
         *
         * @param collectionName
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void setResult(const QString & collectionName);

        /**
         * @brief Returns the json representation of the query
         * and all its extra information which will be sent to
         * the database server.
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        QByteArray toJson() const;

    protected:
        QBSelectPrivate *d_ptr;

    private:
        Q_DECLARE_PRIVATE(QBSelect)
};

}

#endif // QBSELECT_H
