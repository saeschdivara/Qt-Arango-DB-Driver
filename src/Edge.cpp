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

#include "Edge.h"
#include "private/Document_p.h"

using namespace arangodb;

namespace internal {

class EdgePrivate : public DocumentPrivate
{
    public:
};

const QString FROM  = QStringLiteral("_from");
const QString TO = QStringLiteral("_to");

}

Edge::Edge(QObject *parent) :
    Document(new internal::EdgePrivate, parent)
{
}

Edge::Edge(QString collection, Document *fromDoc, Document *toDoc, QObject *parent) :
    Document(new internal::EdgePrivate, collection, parent)
{
    d_func()->data.insert(internal::FROM, fromDoc->docID());
    d_func()->data.insert(internal::TO, toDoc->docID());
}

QString Edge::from()
{
    return d_func()->data.value(internal::FROM).toString();
}

QString Edge::to()
{
    return d_func()->data.value(internal::TO).toString();
}
