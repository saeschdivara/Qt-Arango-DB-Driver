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
 * @since 0.3
 */
class ARANGODBDRIVERSHARED_EXPORT QueryBuilder : public QObject
{
        Q_OBJECT
    public:
        explicit QueryBuilder(QObject *parent = 0);

        QSharedPointer<QBSelect> createSelect(QString collection);
};

}

#endif // QUERYBUILDER_H
