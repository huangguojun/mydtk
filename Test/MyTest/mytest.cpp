#include "mytest.h"

#include <QDebug>
#include <QtTest/QtTest>
/*
void MyTest::initTestCase()
{
    qDebug()<<"Test is starting";
}

void MyTest::cleanupTestCase()
{
    qDebug()<<"Test is finished";
}

void MyTest::init()
{
    qDebug()<<"init";
}

void MyTest::cleanup()
{
    qDebug()<<"cleanup";
}
*/
void MyTest::myTest()
{
    qDebug() << "------------------------MyTest";
    
    //QVERIFY(m_pImgCorrMgr->LoadAllRef());
}



QTEST_APPLESS_MAIN(MyTest)
