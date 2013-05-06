#ifndef EDGE_H
#define EDGE_H

#include "arangodb-driver_global.h"
#include "Document.h"

namespace internal {
class EdgePrivate;
}

namespace arangodb
{

/**
 * @brief The Edge class
 *
 * @since 0.1
 */
class ARANGODBDRIVERSHARED_EXPORT Edge : public Document
{
        Q_OBJECT
    public:
        /**
         * @brief Edge
         *
         * @param parent
         *
         * @since 0.1
         */
        explicit Edge(QObject *parent = 0);

        /**
         * @brief Edge
         *
         * @param collection
         * @param parent
         *
         * @since 0.1
         */
        Edge(QString collection, QObject *parent = 0);

    private:
        Q_DECLARE_PRIVATE(internal::Edge)
};

}

#endif // EDGE_H
