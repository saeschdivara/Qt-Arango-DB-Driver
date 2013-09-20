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

#ifndef GEOINDEX_H
#define GEOINDEX_H

#include "HashIndex.h"

namespace arangodb
{
namespace index
{

class GeoIndexPrivate;

/**
 * @brief Expects an object containing the index details.
 *
 * • type: must be equal to "geo".
 *
 * • fields: A list with one or two attribute paths.
 * If it is a list with one attribute path location, then a geo-spatial index on
 * all documents is created using location as path to the coordinates. The
 * value of the attribute must be a list with at least two double values. The list
 * must contain the latitude (first value) and the longitude (second value). All
 * documents, which do not have the attribute path or with value that are not
 * suitable, are ignored.
 * If it is a list with two attribute paths latitude and longitude, then a
 * geo-spatial index on all documents is created using latitude and longitude
 * as paths the latitude and the longitude. The value of the attribute latitude
 * and of the attribute longitude must a double. All documents, which do
 * not have the attribute paths or which values are not suitable, are ignored.
 *
 * • geoJson: If a geo-spatial index on a location is constructed and geo-
 * Json is true, then the order within the list is longitude followed by lat-
 * itude. This corresponds to the format described in http://geojson.org/geojson-spec.html#positions
 *
 * • constraint: If constraint is true, then a geo-spatial constraint is
 * created. The constraint is a non-unique variant of the index. Note that it
 * is also possible to set the unique attribute instead of the constraint
 * attribute.
 *
 * • ignoreNull: If a geo-spatial constraint is created and ignoreNull
 * is true, then documents with a null in location or at least one null in
 * latitude or longitude are ignored.
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.6
 */
class GeoIndex : public HashIndex
{
        Q_OBJECT
    public:
        /**
         * @brief Constructor
         *
         * @param collection
         * @param parent
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        explicit GeoIndex(Collection * collection, QObject *parent = 0);

        /**
         * @brief If a geo-spatial index on a location is constructed and geo-
         * Json is true, then the order within the list is longitude followed by lat-
         * itude.
         *
         * @param b
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setIsGeoJson(bool b);

        /**
         * @brief Returns if geo json format is used
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        bool isGeoJson() const;

        /**
         * @brief Returns the name/type of the index
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual QString name() const Q_DECL_OVERRIDE;

        /**
         * @brief Returns a json representation of the
         * index
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        virtual QByteArray toJson() const Q_DECL_OVERRIDE;

    private:
        Q_DECLARE_PRIVATE(GeoIndex)
};

}
}

#endif // GEOINDEX_H
