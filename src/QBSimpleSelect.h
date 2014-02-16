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

#ifndef QBSIMPLESELECT_H
#define QBSIMPLESELECT_H

#include "arangodb-driver_global.h"

namespace arangodb
{

class QBSimpleSelectPrivate;


/**
 * @brief The QBSimpleSelect class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.6
 */
class ARANGODBDRIVERSHARED_EXPORT QBSimpleSelect
{
    public:
        /**
         * @brief The Type enum
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        enum class Type {
            UnknownType,
            GetAllDocumentsType,
            GetByExample
        };

        /**
         * @brief QBSimpleSelect
         *
         * @param type
         * @param collection
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        QBSimpleSelect(Type type, const QString & collection);

        /**
         * @brief The number of documents to skip in the query (optional).
         *
         * @param skip
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setSkipNumber(int skip);

        /**
         * @brief If nothing should be skipped, it returns -1
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        int skiptNumber() const;

        /**
         * @brief The maximal amount of documents to return. The skip is applied
         * before the limit restriction. (optional)
         *
         * @param limit
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setLimit(int limit);

        /**
         * @brief If there should be no limit, it returns -1
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        int limit() const;

        /**
         * @brief Sets the example which is in the name of the type.
         *
         * @param example
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setExample(QJsonObject example);

        /**
         * @brief example
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        QJsonObject example() const;

        /**
         * @brief Returns select type
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        Type type() const;

        /**
         * @brief url
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        QString url() const;

        /**
         * @brief toJson
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        QByteArray toJson() const;

    protected:
        QBSimpleSelectPrivate * d_ptr;

    private:
        Q_DECLARE_PRIVATE(QBSimpleSelect)
};

}

#endif // QBSIMPLESELECT_H
