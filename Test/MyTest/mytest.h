#ifndef MYCLASSTEST_H
#define MYCLASSTEST_H

#include <QObject>

class MyTest : public QObject
{
    Q_OBJECT
private slots:
    //void initTestCase();
    //void cleanupTestCase();
    //void init();
    //void cleanup();
    void myTest();

};

#endif // MYCLASSTEST_H
