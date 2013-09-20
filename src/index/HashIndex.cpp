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

#include "HashIndex.h"
#include "private/HashIndex_p.h"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QStringList>

namespace arangodb
{
namespace index
{

const QString HASH_INDEX_NAME = QStringLiteral("hash");

HashIndex::HashIndex(Collection * collection, QObject *parent) :
    AbstractIndex(collection, new HashIndexPrivate, parent)
{
}

void HashIndex::setUnique(bool isUnique)
{
    Q_D(HashIndex);
    d->isUnique = isUnique;
}

bool HashIndex::isUnique() const
{
    Q_D(const HashIndex);
    return d->isUnique;
}

void HashIndex::setFields(const QStringList & fields)
{
    Q_D(HashIndex);
    d->fields = fields;
}

void HashIndex::addField(const QString & field)
{
    Q_D(HashIndex);
    d->fields.append(field);
}

void HashIndex::removeField(const QString & field)
{
    Q_D(HashIndex);
    d->fields.removeOne(field);
}

QStringList HashIndex::fields() const
{
    Q_D(const HashIndex);
    return d->fields;
}

QString HashIndex::name() const
{
    return HASH_INDEX_NAME;
}

QByteArray HashIndex::toJson() const
{
    Q_D(const HashIndex);

    QJsonDocument doc;
    QJsonObject obj;

    obj.insert(QLatin1String("type"), name());
    obj.insert(QLatin1String("fields"), QJsonArray::fromStringList(d->fields));
    obj.insert(QLatin1String("unique"), QJsonValue(d->isUnique));

    doc.setObject(obj);
    return doc.toJson();
}

HashIndex::HashIndex(Collection * collection, HashIndexPrivate * d, QObject * parent) :
    AbstractIndex(collection, d, parent)
{
}

}
}
