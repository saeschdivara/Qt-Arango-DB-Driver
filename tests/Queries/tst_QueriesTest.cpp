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
#include <Arangodbdriver.h>
#include <QueryBuilder.h>
#include <QBSelect.h>

class QueriesTest : public QObject
{
        Q_OBJECT
        
    public:
        QueriesTest();
        ~QueriesTest();
        
    private Q_SLOTS:
        void initTestCase();
        void cleanupTestCase();

        void testGetAllDocuments();
        void testLoadMoreResults();
        void testGetDocByWhere();
        void testGetMultipleDocsByWhere();
        void testGetAllDocumentsFromTwoCollections();

    private:
        arangodb::Arangodbdriver driver;
        arangodb::QueryBuilder qb;
        arangodb::Collection * tempCollection = Q_NULLPTR;
        arangodb::Collection * temp2Collection = Q_NULLPTR;
};

QueriesTest::QueriesTest()
{
}

QueriesTest::~QueriesTest()
{
}

void QueriesTest::initTestCase()
{
    tempCollection = driver.createCollection("temp");
    tempCollection->save();

    temp2Collection = driver.createCollection("temp2");
    temp2Collection->save();

    driver.waitUntilFinished(tempCollection, temp2Collection);

    arangodb::Document * doc1 = tempCollection->createDocument();
    doc1->set("test", true);
    doc1->set("test_field_echo", 44);
    arangodb::Document * doc2 = tempCollection->createDocument();
    doc2->set("test", false);
    doc2->set("test_field_echo", 999);
    arangodb::Document * doc3 = tempCollection->createDocument();
    doc3->set("test", true);
    doc3->set("test_field_echo", 3.2);

    doc1->save();
    doc2->save();
    doc3->save();

    driver.waitUntilFinished(doc1, doc2, doc3);

    arangodb::Document * doc4 = temp2Collection->createDocument();
    doc4->set("con", doc1->docID());
    arangodb::Document * doc5 = temp2Collection->createDocument();
    doc5->set("con", doc2->docID());

    doc4->save();
    doc5->save();

    driver.waitUntilFinished(doc4, doc5);

    tempCollection->load();
    tempCollection->waitUntilLoaded();
}

void QueriesTest::cleanupTestCase()
{
    tempCollection->deleteAll();
    tempCollection->waitUntilDeleted();

    temp2Collection->deleteAll();
    temp2Collection->waitUntilDeleted();
}

void QueriesTest::testGetAllDocuments()
{
    auto select = qb.createSelect(QStringLiteral("temp"));

    QCOMPARE(select->collections().first(), QStringLiteral("temp"));
    QCOMPARE(select->batchSize(), 15);
    QCOMPARE(select->isCounting(), false);

    auto cursor = driver.executeSelect(select);
    cursor->waitForResult();

    QVERIFY2(cursor->hasErrorOccurred() == false, cursor->errorMessage().toLocal8Bit());
    QCOMPARE(cursor->count(), 3);
}

void QueriesTest::testLoadMoreResults()
{
    auto select = qb.createSelect(QStringLiteral("temp"), 2);

    QCOMPARE(select->collections().first(), QStringLiteral("temp"));
    QCOMPARE(select->batchSize(), 2);
    QCOMPARE(select->isCounting(), false);

    auto cursor = driver.executeSelect(select);
    cursor->waitForResult();

    QVERIFY2(cursor->hasErrorOccurred() == false, cursor->errorMessage().toLocal8Bit());
    QCOMPARE(cursor->hasMore(), true);
    QCOMPARE(cursor->count(), 2);

    cursor->getMoreData();
    cursor->waitForResult();

    QCOMPARE(cursor->hasErrorOccurred(), false);
    QCOMPARE(cursor->hasMore(), false);
    QCOMPARE(cursor->count(), 3);
}

void QueriesTest::testGetDocByWhere()
{
    auto select = qb.createSelect(QStringLiteral("temp"), 2);

    QCOMPARE(select->collections().first(), QStringLiteral("temp"));
    QCOMPARE(select->batchSize(), 2);
    QCOMPARE(select->isCounting(), false);

    select->setWhere(QStringLiteral("test"), true);

    auto cursor = driver.executeSelect(select);
    cursor->waitForResult();

    QVERIFY2(cursor->hasErrorOccurred() == false, cursor->errorMessage().toLocal8Bit());
    QCOMPARE(cursor->hasMore(), false);
    QCOMPARE(cursor->count(), 2);
}

void QueriesTest::testGetMultipleDocsByWhere()
{
    auto select = qb.createSelect(QStringLiteral("webuser"), 2);

    QCOMPARE(select->collections().first(), QStringLiteral("webuser"));
    QCOMPARE(select->batchSize(), 2);
    QCOMPARE(select->isCounting(), false);

    QStringList vars;
    vars << "saeschdivara" << "root";
    select->setWhere(QStringLiteral("username"), vars);

    auto cursor = driver.executeSelect(select);
    cursor->waitForResult();

    QVERIFY2(cursor->hasErrorOccurred() == false, cursor->errorMessage().toLocal8Bit());
    QCOMPARE(cursor->hasMore(), false);
    QCOMPARE(cursor->count(), 1);
}

void QueriesTest::testGetAllDocumentsFromTwoCollections()
{
    auto select = qb.createSelect(tempCollection->name(), 2);
    select->addNewCollection(temp2Collection->name());

    QCOMPARE(select->collections().size(), 2);
    QCOMPARE(select->collections().at(0), QStringLiteral("temp"));
    QCOMPARE(select->collections().at(1), QStringLiteral("temp2"));

    auto cursor = driver.executeSelect(select);
    cursor->waitForResult();

    QVERIFY2(cursor->hasErrorOccurred() == false, cursor->errorMessage().toLocal8Bit());
    QCOMPARE(cursor->hasMore(), true);

    cursor->getMoreData();
    cursor->waitForResult();

    QVERIFY2(cursor->hasErrorOccurred() == false, cursor->errorMessage().toLocal8Bit());
    QCOMPARE(cursor->hasMore(), true);

    cursor->getMoreData();
    cursor->waitForResult();

    QVERIFY2(cursor->hasErrorOccurred() == false, cursor->errorMessage().toLocal8Bit());
    QCOMPARE(cursor->hasMore(), false);
    QCOMPARE(cursor->data().size(), 6);
}

QTEST_MAIN(QueriesTest)

#include "tst_QueriesTest.moc"
