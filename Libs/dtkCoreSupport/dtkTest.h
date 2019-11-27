#ifndef DTKTEST_H
#define DTKTEST_H

#include <QtTest/QTest>

/* #define DTKTEST_MAIN(TestMain, TestObject) \ */
/* int TestMain(int argc, char *argv[]) \ */
/* { \ */
/*     QApplication app(argc, argv); \ */
/*     QTEST_DISABLE_KEYPAD_NAVIGATION \ */
/*     TestObject tc; \ */
/*     return QTest::qExec(&tc, argc, argv); \ */
/* } */

#define DTKTEST_NOGUI_MAIN(TestMain, TestObject) \
int TestMain(int argc, char *argv[]) \
{ \
    QCoreApplication app(argc, argv); \
    TestObject tc; \
    return QTest::qExec(&tc, argc, argv); \
}

#define DTKTEST_APPLESS_MAIN(TestMain, TestObject) \
int TestMain(int argc, char *argv[]) \
{ \
    TestObject tc; \
    return QTest::qExec(&tc, argc, argv); \
}

#define DTKTEST_NOOP_MAIN(TestMain, TestObject) \
int TestMain(int argc, char *argv[]) \
{ \
    QObject tc; \
    return QTest::qExec(&tc, argc, argv); \
}

#endif // DTKTEST_H
