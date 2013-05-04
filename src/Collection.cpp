#include "Collection.h"

using namespace arangodb;

namespace internal {

class CollectionPrivate
{
    public:
};

}

Collection::Collection(QObject *parent) :
    QObject(parent)
{
}
