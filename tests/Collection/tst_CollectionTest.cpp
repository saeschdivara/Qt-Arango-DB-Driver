/********************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2013 Sascha Ludwig Häusler
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy of
 ** this software and associated documentation files (the "Software"), to deal in
 ** the Software without restriction, including without limitation the rights to
 ** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 ** the Software, and to permit persons to whom the Software is furnished to do so,
 ** subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in all
 ** copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 ** FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 ** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 ** IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 ** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *********************************************************************************/

#include <QString>
#include <QtTest>
#include <QCoreApplication>

// Drivers
#include <arangodb/ArangoDbDriver>

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
        arangodb::ArangoDBDriver driver;
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
