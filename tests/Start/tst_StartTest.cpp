#include <QtTest>
#include <QtCore>
#include <QtNetwork>
#include <Arangodbdriver.h>

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
    QVERIFY2(true, "Failure");

    QNetworkAccessManager manager;
    Arangodbdriver driver;

    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl("http://localhost:8529/_api/document/test/11153497")));
    {
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
    }

    qDebug() << reply->readAll();
}

void StartTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(StartTest)

#include "tst_StartTest.moc"
