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
        void testGetDocument();
        void testCase1_data();
        void testSaveAndDeleteDocument();

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

void StartTest::testGetDocument()
{
    QFETCH(QString, data);

    Arangodbdriver driver;
    Document *doc = driver.getDocument(data);
    waitForDocumentReady(doc);

    Q_ASSERT(doc->isReady());
    QCOMPARE(doc->docID(), data);
}

void StartTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString("test/11153497");
}

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
    Q_ASSERT(doc->isCreated());

    // Get the same document from the db again
    Document *docFromDb = driver.getDocument(doc->docID());
    waitForDocumentReady(docFromDb);

    // check if there is really the document
    Q_ASSERT(!docFromDb->hasErrorOccurred());

    // Delete the test document again
    doc->drop();
    waitForDocumentDeleted(doc);

    // Try get the same document from the db again
    docFromDb = driver.getDocument(doc->docID());
    waitForDocumentReady(docFromDb);

    // check if the document doesn't exist anymore
    Q_ASSERT(docFromDb->hasErrorOccurred());
}

QTEST_MAIN(StartTest)

#include "tst_StartTest.moc"
