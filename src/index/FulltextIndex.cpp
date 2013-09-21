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

#include "FulltextIndex.h"
#include "private/AbstractIndex_p.h"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QStringList>

namespace arangodb
{
namespace index
{

class FulltextIndexPrivate : public AbstractIndexPrivate
{
    public:
        QStringList fields;
        int minLength = -1;
};

QString FULLTEXT_INDEX_NAME = QStringLiteral("fulltext");

FulltextIndex::FulltextIndex(Collection * collection, QObject *parent) :
    AbstractIndex(collection, new FulltextIndexPrivate, parent)
{
}

void FulltextIndex::setField(const QString & field)
{
    Q_D(FulltextIndex);

    // For now only one field is supported
    if ( d->fields.count() == 1 )
        d->fields[0] = field;
    else
        d->fields.append(field);
}

QStringList FulltextIndex::fields() const
{
    Q_D(const FulltextIndex);
    return d->fields;
}

void FulltextIndex::setMinWordLength(int length)
{
    Q_D(FulltextIndex);
    d->minLength = length;
}

int FulltextIndex::minWordLength() const
{
    Q_D(const FulltextIndex);
    return d->minLength;
}

QString FulltextIndex::name() const
{
    return FULLTEXT_INDEX_NAME;
}

QByteArray FulltextIndex::toJson() const
{
    Q_D(const FulltextIndex);

    QJsonDocument doc;
    QJsonObject obj;

    obj.insert(QLatin1String("type"), name());
    obj.insert(QLatin1String("fields"), QJsonArray::fromStringList(d->fields));

    // check if the user defined a specific min length
    // otherwise a server specific value is used
    if ( d->minLength != -1 )
        obj.insert(QLatin1String("minLength"), d->minLength);

    doc.setObject(obj);
    return doc.toJson();
}

}
}
