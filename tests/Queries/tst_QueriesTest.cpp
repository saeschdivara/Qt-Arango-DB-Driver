#include <QString>
#include <QtTest>
#include <Arangodbdriver.h>
#include <QueryBuilder.h>
#include <QBSelect.h>

class QueriesTest : public QObject
{
        Q_OBJECT
        
    public:
        QueriesTest();
        
    private Q_SLOTS:
        void testCase1();
};

QueriesTest::QueriesTest()
{
}

void QueriesTest::testCase1()
{
    arangodb::Arangodbdriver driver;
    arangodb::QueryBuilder qb;

    auto select = qb.createSelect(QStringLiteral("test"));

    QCOMPARE(select->collection(), QStringLiteral("test"));
    QCOMPARE(select->batchSize(), 15);
    QCOMPARE(select->isCounting(), false);

    auto cursor = driver.executeSelect(select);
    cursor->waitForResult();

    QCOMPARE(cursor->hasErrorOccurred(), false);
}

QTEST_MAIN(QueriesTest)

#include "tst_QueriesTest.moc"
