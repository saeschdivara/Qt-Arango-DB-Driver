#include "Edge.h"
#include "private/Document_p.h"

using namespace arangodb;
using namespace internal;

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

Edge::Edge(QString collection, QObject *parent) :
    Document(new internal::EdgePrivate, collection, parent)
{
}

QByteArray Edge::toJsonString()
{
    Q_D(Edge);
    QJsonDocument doc;

    if ( !isEveryAttributeDirty() ) {
            QJsonObject obj;

            obj.insert(internal::ID, obj.value(internal::ID));
            obj.insert(internal::KEY, obj.value(internal::KEY));
            obj.insert(internal::REV, obj.value(internal::REV));
            obj.insert(internal::FROM, obj.value(internal::FROM));
            obj.insert(internal::TO, obj.value(internal::TO));

            for( QString attribute : d->dirtyAttributes ) {
                    obj.insert(attribute, d->data[attribute]);
                }

            doc.setObject(obj);
        }
    else {
            doc.setObject(d->data);
        }

    return doc.toJson();
}

QString Edge::from()
{
    Q_D(Edge);
    return d->data.value(internal::FROM).toString();
}

QString Edge::to()
{
    Q_D(Edge);
    return d->data.value(internal::TO).toString();
}
