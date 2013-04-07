#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "arangodb-driver_global.h"
#include <QObject>

namespace internal {
class DocumentPrivate;
}

namespace arangodb
{

/**
 * @brief The Document class
 */
class ARANGODBDRIVERSHARED_EXPORT Document : public QObject
{
        Q_OBJECT
    public:
        /**
         * @brief Document
         * @param parent
         */
        explicit Document(QObject *parent = 0);

        /**
         * @brief ~Document
         */
        virtual ~Document();

    Q_SIGNALS:
        /**
         * @brief ready
         */
        void ready();

        /**
         * @brief save
         */
        void saveData(Document *);

    public Q_SLOTS:
        /**
         * @brief save
         */
        void save();

        /**
         * @brief _ar_dataIsAvailable
         */
        void _ar_dataIsAvailable();

    private:
        internal::DocumentPrivate *d;
};

}

#endif // DOCUMENT_H
