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

#include <arangodb/ArangoDbDriver>
#include <transaction/TransactionController.h>

class TransactionTest : public QObject
{
        Q_OBJECT

    public:
        TransactionTest();

    private Q_SLOTS:
        void initTestCase();
        void cleanupTestCase();
        void testCase1();

    private:
        arangodb::ArangoDBDriver driver;
        arangodb::TransactionController transactionCtrl;
};

TransactionTest::TransactionTest()
{
}

void TransactionTest::initTestCase()
{
    driver.connectTransactionController(&transactionCtrl);
}

void TransactionTest::cleanupTestCase()
{
}

void TransactionTest::testCase1()
{
    auto transaction = transactionCtrl.createTransaction();
    transaction->setAutoCommit(false);
    // Start transaction so all actions will be recorded (if the objects are connected to it)
    transaction->start();

    auto collection = driver.createCollection("test_coll");
    // The transaction now records every action
    transaction->connectCollection(collection);

    // Collection shouldn't be really saved, only saved as transaction operation
    collection->save();

    transaction->commit();

    // For now it needs to be disconnected manually
    transaction->disconnectCollection(collection);
    QFAIL("For now not usable");
}

QTEST_GUILESS_MAIN(TransactionTest)

#include "tst_TransactionTest.moc"
