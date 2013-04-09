#ifndef ARANGODBDRIVER_H
#define ARANGODBDRIVER_H

#include "arangodb-driver_global.h"
#include "Document.h"
#include <QtCore/QObject>

namespace internal {
class ArangodbdriverPrivate;
}

namespace arangodb
{

/**
 * @brief The Arangodbdriver class
 */
class ARANGODBDRIVERSHARED_EXPORT Arangodbdriver : public QObject
{
        Q_OBJECT
    public:
        /**
         * @brief Arangodbdriver
         * @param protocol
         * @param host
         * @param port
         */
        Arangodbdriver(QString protocol = QString("http"),
                       QString host = QString("localhost"),
                       qint32 port = 8529);

        /**
         * @brief ~Arangodbdriver
         */
        virtual ~Arangodbdriver();

        /**
         * @brief getDocument
         * @param id
         * @return
         */
        Document* getDocument(QString id);

        /**
         * @brief createDocument
         * @param collection
         * @return
         */
        Document* createDocument(QString collection);

    protected Q_SLOTS:
        /**
         * @brief _ar_save
         * @param doc
         */
        void _ar_save(Document *doc);

    private:
        internal::ArangodbdriverPrivate *d;
};

}

#endif // ARANGODBDRIVER_H
