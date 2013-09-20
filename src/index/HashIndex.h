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

#ifndef HASHINDEX_H
#define HASHINDEX_H

#include "AbstractIndex.h"

#include <QtCore/QString>

namespace arangodb
{
namespace index
{

class HashIndexPrivate;

/**
 * @brief The HashIndex class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.6
 */
class ARANGODBDRIVERSHARED_EXPORT HashIndex : public AbstractIndex
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
        explicit HashIndex(Collection * collection, QObject *parent = 0);

        /**
         * @brief Sets according to isUnique, if the fields
         * values have to be unique
         *
         * @param isUnique
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setUnique(bool isUnique);

        /**
         * @brief Returns if the fields values have to
         * be unique
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        bool isUnique() const;

        /**
         * @brief Set for which fields this HashIndex is created
         *
         * @param fields
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void setFields(const QStringList & fields);

        /**
         * @brief Add field to the fields list
         *
         * @param field
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void addField(const QString & field);

        /**
         * @brief Removes field from the fields list
         *
         * @param field
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        void removeField(const QString & field);

        /**
         * @brief Returns all fields which have this HashIndex
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.6
         */
        QStringList fields() const;

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

    protected:
        HashIndex(Collection *collection, HashIndexPrivate *d, QObject *parent);

    private:
        Q_DECLARE_PRIVATE(HashIndex)
};

}
}

#endif // HASHINDEX_H
