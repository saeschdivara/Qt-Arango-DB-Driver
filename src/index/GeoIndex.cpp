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

#include "GeoIndex.h"
#include "private/HashIndex_p.h"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

namespace arangodb
{
namespace index
{

class GeoIndexPrivate : public HashIndexPrivate
{
    public:
        bool isGeoJson;
        bool hasConstraint;
        bool ignoreNull;
};

QString GEO_INDEX_NAME = QStringLiteral("geo");

GeoIndex::GeoIndex(Collection * collection, QObject *parent) :
    HashIndex(collection, new GeoIndexPrivate, parent)
{
}

void GeoIndex::setIsGeoJson(bool b)
{
    Q_D(GeoIndex);
    d->isGeoJson = b;
}

bool GeoIndex::isGeoJson() const
{
    Q_D(const GeoIndex);
    return d->isGeoJson;
}

void GeoIndex::setHasConstraint(bool b)
{
    Q_D(GeoIndex);
    d->hasConstraint = b;
}

bool GeoIndex::hasConstraint() const
{
    Q_D(const GeoIndex);
    return d->hasConstraint;
}

void GeoIndex::setIgnoreNull(bool b)
{
    Q_D(GeoIndex);
    d->ignoreNull = b;
}

bool GeoIndex::ignoreNull() const
{
    Q_D(const GeoIndex);
    return d->ignoreNull;
}

QString GeoIndex::name() const
{
    return GEO_INDEX_NAME;
}

QByteArray GeoIndex::toJson() const
{
    Q_D(const GeoIndex);

    QJsonDocument doc;
    QJsonObject obj;

    obj.insert(QLatin1String("type"), name());
    obj.insert(QLatin1String("fields"), QJsonArray::fromStringList(d->fields));
    obj.insert(QLatin1String("geoJson"), QJsonValue(d->isGeoJson));
    obj.insert(QLatin1String("ignoreNull"), QJsonValue(d->ignoreNull));

    if ( d->isUnique && d->hasConstraint )
        qWarning() << Q_FUNC_INFO << " Geo Index cannot be unique and has contraint";
    else if ( d->hasConstraint )
        obj.insert(QLatin1String("constraint"), QJsonValue(d->hasConstraint));
    else if ( d->isUnique )
        obj.insert(QLatin1String("unique"), QJsonValue(d->isUnique));

    doc.setObject(obj);
    return doc.toJson();
}

}
}
