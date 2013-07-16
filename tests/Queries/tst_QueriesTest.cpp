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
        void testGetAllDocuments();
        void testLoadMoreResults();
        void testGetDocByWhere();
        void testGetDocsByWhere();
};

QueriesTest::QueriesTest()
{
}

void QueriesTest::testGetAllDocuments()
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
    QCOMPARE(cursor->count(), 4);
}

void QueriesTest::testLoadMoreResults()
{
    arangodb::Arangodbdriver driver;
    arangodb::QueryBuilder qb;

    auto select = qb.createSelect(QStringLiteral("test"), 2);

    QCOMPARE(select->collection(), QStringLiteral("test"));
    QCOMPARE(select->batchSize(), 2);
    QCOMPARE(select->isCounting(), false);

    auto cursor = driver.executeSelect(select);
    cursor->waitForResult();

    QCOMPARE(cursor->hasErrorOccurred(), false);
    QCOMPARE(cursor->hasMore(), true);
    QCOMPARE(cursor->count(), 2);

    cursor->getMoreData();
    cursor->waitForResult();

    QCOMPARE(cursor->hasErrorOccurred(), false);
    QCOMPARE(cursor->hasMore(), false);
    QCOMPARE(cursor->count(), 4);
}

void QueriesTest::testGetDocByWhere()
{
    arangodb::Arangodbdriver driver;
    arangodb::QueryBuilder qb;

    auto select = qb.createSelect(QStringLiteral("test"), 2);

    QCOMPARE(select->collection(), QStringLiteral("test"));
    QCOMPARE(select->batchSize(), 2);
    QCOMPARE(select->isCounting(), false);

    select->setWhere(QStringLiteral("name"), QStringLiteral("ll"));

    auto cursor = driver.executeSelect(select);
    cursor->waitForResult();

    QCOMPARE(cursor->hasErrorOccurred(), false);
    QCOMPARE(cursor->hasMore(), false);
    QCOMPARE(cursor->count(), 1);
}

void QueriesTest::testGetDocsByWhere()
{
    arangodb::Arangodbdriver driver;
    arangodb::QueryBuilder qb;

    auto select = qb.createSelect(QStringLiteral("webuser"), 2);

    QCOMPARE(select->collection(), QStringLiteral("webuser"));
    QCOMPARE(select->batchSize(), 2);
    QCOMPARE(select->isCounting(), false);

    QStringList vars;
    vars << "saeschdivara" << "root";
    select->setWhere(QStringLiteral("username"), vars);

    auto cursor = driver.executeSelect(select);
    cursor->waitForResult();

    QCOMPARE(cursor->hasErrorOccurred(), false);
    QCOMPARE(cursor->hasMore(), false);
    QCOMPARE(cursor->count(), 1);
}

QTEST_MAIN(QueriesTest)

#include "tst_QueriesTest.moc"
