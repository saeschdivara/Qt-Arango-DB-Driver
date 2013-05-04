#include "Collection.h"

using namespace arangodb;

namespace internal {

class CollectionPrivate
{
    public:
};

}

Collection::Collection(QString name, QObject *parent) :
    QObject(parent)
{
}
