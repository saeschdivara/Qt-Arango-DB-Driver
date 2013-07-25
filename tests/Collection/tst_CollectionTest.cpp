#include <QString>
#include <QtTest>
#include <QCoreApplication>

class CollectionTest : public QObject
{
        Q_OBJECT
        
    public:
        CollectionTest();
        
    private Q_SLOTS:
        void initTestCase();
        void cleanupTestCase();
        void testCase1();
        void testCase1_data();
};

CollectionTest::CollectionTest()
{
}

void CollectionTest::initTestCase()
{
}

void CollectionTest::cleanupTestCase()
{
}

void CollectionTest::testCase1()
{
    QFETCH(QString, collectionName);
    QVERIFY2(true, "Failure");
}

void CollectionTest::testCase1_data()
{
    QTest::addColumn<QString>("collectionName");
    QTest::newRow("0") << QString("");
}

QTEST_MAIN(CollectionTest)

#include "tst_CollectionTest.moc"
