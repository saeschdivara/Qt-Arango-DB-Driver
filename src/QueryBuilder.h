#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include "arangodb-driver_global.h"
#include "QBSelect.h"
#include <QtCore/QSharedPointer>

namespace arangodb
{

/**
 * @brief The QueryBuilder class
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.3
 */
class ARANGODBDRIVERSHARED_EXPORT QueryBuilder
{
    public:
        QueryBuilder();

        /**
         * @brief createSelect
         *
         * @param collection
         * @param batchSize
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        QSharedPointer<QBSelect> createSelect(QString collection, int batchSize = 15);

        /**
         * @brief createSelect
         *
         * @param collections
         * @param batchSize
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.5
         */
        QSharedPointer<QBSelect> createSelect(QStringList & collections, int batchSize = 15);
};

}

#endif // QUERYBUILDER_H
