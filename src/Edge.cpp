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

Edge::Edge(QString collection, QObject *parent) :
    Document(new internal::EdgePrivate, collection, parent)
{
}
