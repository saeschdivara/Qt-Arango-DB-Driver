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
}

void StartTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString("test/11153497");
}

QTEST_MAIN(StartTest)

#include "tst_StartTest.moc"
