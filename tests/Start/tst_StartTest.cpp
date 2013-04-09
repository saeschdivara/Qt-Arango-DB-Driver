#include <QtTest>
#include <QtCore>
#include <QtNetwork>
#include <Arangodbdriver.h>

using namespace arangodb;

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
    {
        QEventLoop loop;
        connect(doc, &Document::ready, &loop, &QEventLoop::quit);
        loop.exec();
    }

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
    doc->set("fuu", QVariant("ss"));
    doc->save();
    {
        QEventLoop loop;
        connect(doc, &Document::ready, &loop, &QEventLoop::quit);
        loop.exec();
    }
}

QTEST_MAIN(StartTest)

#include "tst_StartTest.moc"
