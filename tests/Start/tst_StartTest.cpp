#include <QtTest>
#include <QtCore>
#include <QtNetwork>
#include <Arangodbdriver.h>

using namespace arangodb;

/**
 * @brief The StartTest class
 */
class StartTest : public QObject
{
        Q_OBJECT
        
    public:
        StartTest();
        
    private Q_SLOTS:
        void initTestCase();
        void cleanupTestCase();
        void testDocumentSaveAndDelete();
        void testDocumentSave2Times();
        void testDocumentPartialUpdate();
        void testDocumentHeadOperation();
        void testEdgeSaveAndDelete();

    private:
        /**
         * @brief waitForDocumentReady
         * @param doc
         */
        inline void waitForDocumentReady(Document* doc) {
            QEventLoop loop;
            connect(doc, &Document::error, &loop, &QEventLoop::quit);
            connect(doc, &Document::ready, &loop, &QEventLoop::quit);
            loop.exec();
        }

        /**
         * @brief waitForDocumentReady
         * @param doc
         */
        inline void waitForDocumentDeleted(Document* doc) {
            QEventLoop loop;
            connect(doc, &Document::error, &loop, &QEventLoop::quit);
            connect(doc, &Document::dataDeleted, &loop, &QEventLoop::quit);
            loop.exec();
        }
};

/**
 * @brief StartTest::StartTest
 */
StartTest::StartTest()
{
}

/**
 * @brief StartTest::initTestCase
 */
void StartTest::initTestCase()
{
}

/**
 * @brief StartTest::cleanupTestCase
 */
void StartTest::cleanupTestCase()
{
}

/**
 * @brief StartTest::testDocumentSaveAndDelete
 */
void StartTest::testDocumentSaveAndDelete()
{
    Arangodbdriver driver;
    Document *doc = driver.createDocument("test");
    // Set fuu with ss
    doc->set("fuu", QVariant("ss"));
    // Save the document
    doc->save();
    waitForDocumentReady(doc);

    // Check if the flag isCreated is set
    Q_ASSERT_X(doc->isCreated(), "testSaveAndDeleteDocument", doc->errorMessage().toLocal8Bit());

    // Get the same document from the db again
    Document *docFromDb = driver.getDocument(doc->docID());
    waitForDocumentReady(docFromDb);

    // check if there is really the document
    Q_ASSERT_X( !docFromDb->hasErrorOccurred(), "testSaveAndDeleteDocument", docFromDb->errorMessage().toLocal8Bit() );

    QCOMPARE( docFromDb->get("fuu").toString(), QString("ss") );

    // Delete the test document again
    doc->drop();
    waitForDocumentDeleted(doc);

    // Try get the same document from the db again
    docFromDb = driver.getDocument(doc->docID());
    waitForDocumentReady(docFromDb);

    // check if the document doesn't exist anymore
    Q_ASSERT( docFromDb->hasErrorOccurred() );
}

/**
 * @brief StartTest::testDocumentSave2Times
 */
void StartTest::testDocumentSave2Times()
{
    Arangodbdriver driver;
    Document *doc = driver.createDocument("test");
    // Save the document
    doc->save();
    waitForDocumentReady(doc);

    // Change the document and save/update
    doc->set("lll", QVariant("aaa"));

    QCOMPARE(doc->dirtyAttributes().size(), 1);
    Q_ASSERT(doc->isEveryAttributeDirty());

    doc->save();
    waitForDocumentReady(doc);

    // check if there wasn't an error
    Q_ASSERT_X( !doc->hasErrorOccurred(), "!doc->hasErrorOccurred()", doc->errorMessage().toLocal8Bit() );

    // Delete the test document again
    doc->drop();
    waitForDocumentDeleted(doc);
}

/**
 * @brief StartTest::testDocumentPartialUpdate
 */
void StartTest::testDocumentPartialUpdate()
{
    Arangodbdriver driver;
    Document *doc = driver.createDocument("test");
    doc->set("lll", QVariant("aaa"));
    doc->set("dd", QVariant("aaa"));
    doc->set("aa", QVariant("aaa"));
    // Save the document
    doc->save();
    waitForDocumentReady(doc);

    // Change the document and save/update
    doc->set("lll", QVariant("bb"));

    QCOMPARE(doc->dirtyAttributes().size(), 1);
    Q_ASSERT( !doc->isEveryAttributeDirty() );

    doc->save();
    waitForDocumentReady(doc);

    // check if there wasn't an error
    Q_ASSERT_X( !doc->hasErrorOccurred(), "!doc->hasErrorOccurred()", doc->errorMessage().toLocal8Bit() );

    // Delete the test document again
    doc->drop();
    waitForDocumentDeleted(doc);
}

/**
 * @brief StartTest::testDocumentHeadOperation
 */
void StartTest::testDocumentHeadOperation()
{
    Arangodbdriver driver;
    Document *doc = driver.createDocument("test");
    doc->set("lll", QVariant("aaa"));
    // Save the document
    doc->save();
    waitForDocumentReady(doc);

    // Try get the same document from the db again
    Document *docFromDb = driver.getDocument(doc->docID());
    waitForDocumentReady(docFromDb);

    docFromDb->updateStatus();
    waitForDocumentReady(docFromDb);

    QCOMPARE( docFromDb->isCreated(), true );
    QCOMPARE( docFromDb->isCurrent(), true );

    doc->set("lllmmmm", "fuu");
    doc->save();
    waitForDocumentReady(doc);

    docFromDb->updateStatus();
    waitForDocumentReady(docFromDb);

    QCOMPARE( docFromDb->isCreated(), true );
    QCOMPARE( docFromDb->isCurrent(), false );

    doc->drop();
    waitForDocumentDeleted(doc);

    docFromDb->updateStatus();
    waitForDocumentReady(docFromDb);

    QCOMPARE( docFromDb->isCreated(), false );
    QCOMPARE( docFromDb->isCurrent(), false );
}

/**
 * @brief StartTest::testEdgeSaveAndDelete
 */
void StartTest::testEdgeSaveAndDelete()
{
    Arangodbdriver driver;
    Document *doc1 = driver.createDocument("test");
    Document *doc2 = driver.createDocument("test");

    doc1->save();
    waitForDocumentReady(doc1);

    doc2->save();
    waitForDocumentReady(doc2);

    Edge *e = driver.createEdge("fubar", doc1, doc2);
    e->save();
    waitForDocumentReady(e);

    Edge *dbEdge = driver.getEdge(e->docID());
    waitForDocumentReady(dbEdge);

    QVERIFY2( dbEdge->isCreated() == true, dbEdge->errorMessage().toLocal8Bit() );

    e->drop();
    waitForDocumentDeleted(e);

    QCOMPARE( e->isCreated(), false );
}

QTEST_MAIN(StartTest)

#include "tst_StartTest.moc"
