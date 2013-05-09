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
        Edge(QString collection, Document *fromDoc, Document *toDoc, QObject *parent = 0);

        /**
         * @brief from
         *
         * @return
         *
         * @since 0.1
         */
        QString from();

        /**
         * @brief to
         *
         * @return
         *
         * @since 0.1
         */
        QString to();

    private:
        Q_DECLARE_PRIVATE(internal::Edge)
};

}

#endif // EDGE_H
