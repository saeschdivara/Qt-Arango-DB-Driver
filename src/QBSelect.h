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
        enum class SortingOrder {
            AscSorting = 1,
            DescSorting = 2
        };

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
         * @brief setFullCounting
         *
         * @param count
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setFullCounting(bool count);

        /**
         * @brief isFullyCounting
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        bool isFullyCounting() const;

        /**
         * @brief setLimit
         *
         * @param start
         * @param number
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setLimit(int start, int number);

        /**
         * @brief setLimit
         *
         * @param number
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setLimit(int number);

        /**
         * @brief setWhereNot
         *
         * @param field
         * @param op
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setWhereNot(const QString & field, const QString & op);

        /**
         * @brief setWhereNot
         *
         * @param field
         * @param op
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setWhereNot(const QString & field, const QStringList & op);

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
         * @brief This method assumes that only one collection
         * is set for the select and therefor the where statement
         * is set for this collection (first collection in the list)
         *
         * @param field
         * @param op
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void setWhere(const QString &field, bool op);

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
         * @brief Sets the filter where the content of the field1 in
         * the collection1 is equal field2 in collection2
         *
         * @param collection1
         * @param field1
         * @param collection2
         * @param field2
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void setWhere(const QString & collection1, const QString & field1,
                      const QString & collection2, const QString & field2);

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
         * @brief setResult
         *
         * @param collectionNames
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void setResult(const QStringList & collectionNames);

        /**
         * @brief Keys are the collection names and the values
         * are either an only string with one field name or
         * a string list which can contain multiple fields
         *
         * @param collectionFields
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        void setResult(const QHash<QString, QVariant> & collectionFields);

        /**
         * @brief setSortingColumn
         *
         * @param collection
         * @param column
         * @param order
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setSortingColumn(const QString & collection, const QString & column, SortingOrder order);

        /**
         * @brief setRandom
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setRandom(bool isRandom);

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
