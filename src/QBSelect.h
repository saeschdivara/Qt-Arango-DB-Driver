#ifndef QBSELECT_H
#define QBSELECT_H

#include "arangodb-driver_global.h"
#include "QBSelect.h"

namespace arangodb
{

class QBSelectPrivate;

class ARANGODBDRIVERSHARED_EXPORT QBSelect
{
    public:
        QBSelect(const QString & collection, int batchSize);
        ~QBSelect();

        QString collection() const;
        int batchSize() const;

        void setCounting(bool c);
        bool isCounting() const;

        QByteArray toJson() const;

    protected:
        QBSelectPrivate *d_ptr;

    private:
        Q_DECLARE_PRIVATE(QBSelect)
};

}

#endif // QBSELECT_H
