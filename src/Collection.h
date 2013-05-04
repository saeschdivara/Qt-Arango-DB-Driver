#ifndef COLLECTION_H
#define COLLECTION_H

#include <QObject>
#include "arangodb-driver_global.h"

namespace internal {
class CollectionPrivate;
}

namespace arangodb
{

/**
 * @brief The Collection class
 *
 * @since 0.1
 */
class ARANGODBDRIVERSHARED_EXPORT Collection : public QObject
{
        Q_OBJECT
    public:
        /**
         * @brief Collection
         *
         * @param parent
         *
         * @since 0.1
         */
        explicit Collection(QObject *parent = 0);
        
    signals:
        
    public slots:
        
};

}

#endif // COLLECTION_H
