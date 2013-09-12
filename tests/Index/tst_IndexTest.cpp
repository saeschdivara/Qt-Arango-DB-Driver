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

#include <arangodb/Arangodbdriver.h>
#include <arangodb/QueryBuilder.h>
#include <arangodb/index/CapIndex.h>

using namespace arangodb;
using namespace arangodb::index;

class IndexTest : public QObject
{
        Q_OBJECT
        
    public:
        IndexTest();
        
    private Q_SLOTS:
        void initTestCase();
        void cleanupTestCase();
        void testCreatingCapIndex();

    protected:
        Arangodbdriver driver;
        QueryBuilder qb;
        Collection * tempCollection = Q_NULLPTR;

        Document * saveDoc(Collection * collection) {
            Document * doc = collection->createDocument();
            doc->save();
            doc->waitForResult();
        }
};

IndexTest::IndexTest()
{
}

void IndexTest::initTestCase()
{
    tempCollection = driver.createCollection("temp");
    tempCollection->save();
    tempCollection->waitUntilReady();
}

void IndexTest::cleanupTestCase()
{
    tempCollection->deleteAll();
    tempCollection->waitUntilDeleted();
}

void IndexTest::testCreatingCapIndex()
{
    CapIndex * index = dynamic_cast<CapIndex *>(tempCollection->createIndex(IndexType::CapIndex));
    QVERIFY(index != Q_NULLPTR);

    index->setSize(1);
    index->save();
    index->waitUntilReady();

    QVERIFY2(!index->hasErrorOccurred(), qPrintable(index->errorMessage()));
    QCOMPARE(index->isNewlyCreated(), true);

    Document * doc1 = saveDoc(tempCollection);
    Document * doc2 = saveDoc(tempCollection);

    QVERIFY2(!doc1->hasErrorOccurred(), qPrintable(doc1->errorMessage()));
    QVERIFY2(!doc2->hasErrorOccurred(), qPrintable(doc2->errorMessage()));

    auto cursor = tempCollection->getAllDocuments();
    cursor->waitForResult();

    QVERIFY2(!cursor->hasErrorOccurred(), qPrintable(cursor->errorMessage()));
    QList<Document *> data = cursor->data();

    QCOMPARE(data.size(), 1);
    Document * dataDoc = data.first();

    // This shows that everything can be saved into the database
    // but after limit it starts to override older entries
    QCOMPARE(dataDoc->key(), doc2->key());

    index->deleteLater();
    doc1->deleteLater();
    doc2->deleteLater();
}

QTEST_GUILESS_MAIN(IndexTest)

#include "tst_IndexTest.moc"
