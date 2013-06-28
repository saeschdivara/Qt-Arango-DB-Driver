#include <QString>
#include <QtTest>
#include <Arangodbdriver.h>
#include <QueryBuilder.h>

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

    driver.executeSelect(select);
}

QTEST_APPLESS_MAIN(QueriesTest)

#include "tst_QueriesTest.moc"
