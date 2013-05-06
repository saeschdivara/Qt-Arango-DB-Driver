#ifndef EDGE_H
#define EDGE_H

#include "arangodb-driver_global.h"

namespace arangodb
{

/**
 * @brief The Edge class
 *
 * @since 0.1
 */
class ARANGODBDRIVERSHARED_EXPORT Edge : public QObject
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
        
    signals:
        
    public slots:
        
};

}

#endif // EDGE_H
