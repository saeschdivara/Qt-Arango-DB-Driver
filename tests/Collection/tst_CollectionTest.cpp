#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <Arangodbdriver.h>

class CollectionTest : public QObject
{
        Q_OBJECT
        
    public:
        CollectionTest();
        
    private Q_SLOTS:
        void initTestCase();
        void cleanupTestCase();

        void testCreateCollection();
        void testCreateCollection_data();

    private:
        arangodb::Arangodbdriver driver;
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

void CollectionTest::testCreateCollection()
{
    QFETCH(QString, collectionName);

    arangodb::Collection * collection = driver.createCollection(collectionName);
    collection->save();
    collection->waitUntilReady();

    QCOMPARE(driver.isColllectionExisting(collectionName), true);
}

void CollectionTest::testCreateCollection_data()
{
    QTest::addColumn<QString>("collectionName");
    QTest::newRow("0") << QString("fuubar");
}

QTEST_MAIN(CollectionTest)

#include "tst_CollectionTest.moc"
