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

#ifndef EDGE_H
#define EDGE_H

#include "arangodb-driver_global.h"
#include "Document.h"

namespace internal {
class EdgePrivate;
}

namespace arangodb
{

/**
 * @brief The Edge class
 *
 * @since 0.1
 */
class ARANGODBDRIVERSHARED_EXPORT Edge : public Document
{
        Q_OBJECT
    public:
        /**
         * @brief Edge
         *
         * @param parent
         *
         * @since 0.1
         */
        explicit Edge(QObject *parent = 0);

        /**
         * @brief Edge
         *
         * @param collection
         * @param parent
         *
         * @since 0.1
         */
        Edge(QString collection, Document *fromDoc, Document *toDoc, QObject *parent = 0);

        /**
         * @brief from
         *
         * @return
         *
         * @since 0.1
         */
        QString from();

        /**
         * @brief to
         *
         * @return
         *
         * @since 0.1
         */
        QString to();

    private:
        Q_DECLARE_PRIVATE(internal::Edge)
};

}

#endif // EDGE_H
