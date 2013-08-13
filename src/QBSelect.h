#ifndef QBSELECT_H
#define QBSELECT_H

#include "arangodb-driver_global.h"
#include <QStringList>

namespace arangodb
{

class QBSelectPrivate;

/**
 * @brief This class represents selects which has
 * little methods to modify it
 *
 * @author Sascha Häusler <saeschdivara@gmail.com>
 * @since 0.3
 */
class ARANGODBDRIVERSHARED_EXPORT QBSelect
{
    public:
        /**
         * @brief QBSelect
         *
         * @param collection
         * @param batchSize
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        QBSelect(const QString & collection, int batchSize);
        ~QBSelect();

        /**
         * @brief collection
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        QStringList collections() const;

        /**
         * @brief batchSize
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        int batchSize() const;

        /**
         * @brief setCounting
         *
         * @param c
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        void setCounting(bool c);

        /**
         * @brief isCounting
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        bool isCounting() const;

        /**
         * @brief This method assumes that only one collection
         * is set for the select and therefor the where statement
         * is set for this collection (first collection in the list)
         *
         * @param field
         * @param op
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        void setWhere(const QString & field, const QString & op);

        /**
         * @brief Set the where conditions for one collection
         * field but gives multiple choices.
         *
         * @param field
         * @param op
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        void setWhere(const QString & field, const QStringList & op);

        /**
         * @brief Returns the json representation of the query
         * and all its extra information which will be sent to
         * the database server.
         *
         * @return
         *
         * @author Sascha Häusler <saeschdivara@gmail.com>
         * @since 0.3
         */
        QByteArray toJson() const;

    protected:
        QBSelectPrivate *d_ptr;

    private:
        Q_DECLARE_PRIVATE(QBSelect)
};

}

#endif // QBSELECT_H
