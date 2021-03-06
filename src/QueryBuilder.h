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

#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include "arangodb-driver_global.h"
#include "QBSelect.h"
#include <QtCore/QSharedPointer>

namespace arangodb
{

/**
 * @brief The QueryBuilder class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.3
 */
class ARANGODBDRIVERSHARED_EXPORT QueryBuilder
{
    public:
        QueryBuilder();

        /**
         * @brief createSelect
         *
         * @param collection
         * @param batchSize
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        QSharedPointer<QBSelect> createSelect(QString collection, int batchSize = 15);

        /**
         * @brief createSelect
         *
         * @param collections
         * @param batchSize
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        QSharedPointer<QBSelect> createSelect(QStringList & collections, int batchSize = 15);
};

}

#endif // QUERYBUILDER_H
