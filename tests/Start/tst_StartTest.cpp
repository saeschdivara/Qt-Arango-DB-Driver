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
        void testSaveAndDeleteDocument();
        void testSaveDocument2Times();
        void testPartialUpdate();

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

StartTest::StartTest()
{
}

void StartTest::initTestCase()
{
}

void StartTest::cleanupTestCase()
{
}

/**
 * @brief StartTest::testSaveAndDeleteDocument
 */
void StartTest::testSaveAndDeleteDocument()
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
 * @brief StartTest::testSaveDocument2Times
 */
void StartTest::testSaveDocument2Times()
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
 * @brief StartTest::testPartialUpdate
 */
void StartTest::testPartialUpdate()
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

QTEST_MAIN(StartTest)

#include "tst_StartTest.moc"
