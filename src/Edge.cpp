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
