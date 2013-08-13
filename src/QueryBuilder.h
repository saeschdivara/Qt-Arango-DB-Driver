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

        QSharedPointer<QBSelect> createSelect(QString collection, int batchSize = 15);
        QSharedPointer<QBSelect> createSelect(QStringList & collections, int batchSize = 15);
};

}

#endif // QUERYBUILDER_H
