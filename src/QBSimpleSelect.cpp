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

#include "QBSimpleSelect.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

namespace arangodb
{

class QBSimpleSelectPrivate
{
    public:
        QBSimpleSelect::Type type;
        QString collection;
};

QBSimpleSelect::QBSimpleSelect(Type type, const QString & collection) :
    d_ptr(new QBSimpleSelectPrivate)
{
    d_ptr->type = type;
    d_ptr->collection = collection;
}

QBSimpleSelect::Type QBSimpleSelect::type() const
{
    Q_D(const QBSimpleSelect);
    return d->type;
}

QString QBSimpleSelect::url() const
{
    Q_D(const QBSimpleSelect);

    QString url;

    switch (d->type)
    {
        case Type::GetAllDocumentsType:
            url = QLatin1String("/simple/all");
            break;
        case Type::UnknownType:
            break;
        default:
            break;
    }

    return url;
}

QByteArray QBSimpleSelect::toJson() const
{
    Q_D(const QBSimpleSelect);

    QJsonDocument doc;
    QJsonObject obj;

    obj.insert(QLatin1String("collection"), d->collection);

    switch (d->type) {
        case Type::GetAllDocumentsType: {
            }
            break;
        default:
            break;
    }

    doc.setObject(obj);
    return doc.toJson();
}

}
