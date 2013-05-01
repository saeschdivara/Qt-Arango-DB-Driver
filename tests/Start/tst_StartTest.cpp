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
        void testCase1();
        void testCase1_data();
        void testCase2();

    private:
        /**
         * @brief waitForDocumentReady
         * @param doc
         */
        inline void waitForDocumentReady(Document* doc) {
            QEventLoop loop;
            connect(doc, &Document::ready, &loop, &QEventLoop::quit);
            loop.exec();
        }

        /**
         * @brief waitForDocumentReady
         * @param doc
         */
        inline void waitForDocumentDeleted(Document* doc) {
            QEventLoop loop;
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

void StartTest::testCase1()
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

void StartTest::testCase2()
{
    Arangodbdriver driver;
    Document* doc = driver.createDocument("test");
    // Set fuu with ss
    doc->Set("fuu", QVariant("ss"));
    // Save the document
    doc->Save();
    waitForDocumentReady(doc);

    doc->Delete();
    waitForDocumentDeleted(doc);
}

QTEST_MAIN(StartTest)

#include "tst_StartTest.moc"
