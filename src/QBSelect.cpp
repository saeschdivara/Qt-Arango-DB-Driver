#include "QBSelect.h"

namespace arangodb
{

class QBSelectPrivate
{
    public:
        QString collection;
};

QBSelect::QBSelect() :
    d_ptr(new QBSelectPrivate)
{
}

QBSelect::~QBSelect()
{
    delete d_ptr;
}

}
