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

#ifndef CAPINDEX_H
#define CAPINDEX_H

#include "arangodb-driver_global.h"
#include "IndexInterface.h"

#include <QtCore/QJsonObject>
#include <QtCore/QString>

namespace arangodb
{
namespace index
{

/**
 * @brief The CapIndex class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.6
 */
class ARANGODBDRIVERSHARED_EXPORT CapIndex : public QObject, public IndexInterface
{
        Q_OBJECT
    public:
        explicit CapIndex(QObject *parent = 0);

        // IndexInterface interface
    public:
        /**
         * @brief id
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual QString id() const Q_DECL_OVERRIDE;
        virtual QString name() const
        {
        }
        virtual Collection::Type collectionType() const
        {
        }
        virtual QByteArray toJson() const
        {
        }
        virtual void waitUntilReady()
        {
        }
        virtual void waitUntilDeleted()
        {
        }

    Q_SIGNALS:
        virtual void _ar_signal_delete(IndexInterface *);
};

}
}

#endif // CAPINDEX_H
