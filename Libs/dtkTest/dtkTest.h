// Version: $Id: 0fe3a08e076660c1b990eae5d111a3d552370a16 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <QtTest>

#define DTKTEST_MAIN(TestMain, TestObject)          \
    int TestMain(int argc, char *argv[])            \
    {                                               \
        QApplication app(argc, argv);               \
        TestObject tc;                              \
        return QTest::qExec(&tc, argc, argv);       \
    }

#define DTKTEST_MAIN_GUI(TestMain, TestObject)      \
    int TestMain(int argc, char *argv[])            \
    {                                               \
        QGuiApplication app(argc, argv);            \
        TestObject tc;                              \
        return QTest::qExec(&tc, argc, argv);       \
    }

#define DTKTEST_MAIN_NOGUI(TestMain, TestObject)    \
    int TestMain(int argc, char *argv[])            \
    {                                               \
        QCoreApplication app(argc, argv);           \
        TestObject tc;                              \
        return QTest::qExec(&tc, argc, argv);       \
    }

#define DTKDISTRIBUTEDTEST_MAIN_NOGUI(TestMain, TestObject) \
    int TestMain(int argc, char *argv[])            \
    {                                               \
        qputenv("DTK_TEST", QByteArrayLiteral("1")); \
        qputenv("QT_QPA_PLATFORM", QByteArrayLiteral("minimal")); \
        dtkDistributedApplication *app = dtkDistributed::create(argc,argv);    \
        app->initialize();    \
        TestObject tc;                              \
        return QTest::qExec(&tc, argc, argv);       \
    }

//
// dtkTest.h ends here
