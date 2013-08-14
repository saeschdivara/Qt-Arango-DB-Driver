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

        void testCreateAndDeleteCollection();
        void testCreateAndDeleteCollection_data();

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

void CollectionTest::testCreateAndDeleteCollection()
{
    QFETCH(QString, collectionName);

    arangodb::Collection * collection = driver.createCollection(collectionName);
    collection->save();
    collection->waitUntilReady();

    QCOMPARE(collection->hasErrorOccurred(), false);
    QCOMPARE(driver.isColllectionExisting(collectionName), true);

    collection->deleteAll();
    collection->waitUntilDeleted();

    QCOMPARE(collection->hasErrorOccurred(), false);
    QCOMPARE(driver.isColllectionExisting(collectionName), false);
}

void CollectionTest::testCreateAndDeleteCollection_data()
{
    QTest::addColumn<QString>("collectionName");
    QTest::newRow("0") << QString("fuubar");
}

QTEST_MAIN(CollectionTest)

#include "tst_CollectionTest.moc"
