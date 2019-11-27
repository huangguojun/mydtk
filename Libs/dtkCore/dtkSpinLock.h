// Version: $Id: 33b8ad289f27c864ea405b17b9fc887a37756a51 $
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

#include <QtCore>

class dtkSpinLock: private QAtomicInt
{
public:
    dtkSpinLock(void) : QAtomicInt(Unlocked) {

    }

public:
    class Acquire
    {
    public:
        Acquire(dtkSpinLock& spinLock): m_spinLock(spinLock) {
            m_spinLock.lock();
        }

        ~Acquire(void) {
            m_spinLock.unlock();
        }

    private:
        dtkSpinLock& m_spinLock;

    public:
        Acquire& operator=(const Acquire&);

    public:
        Acquire(const Acquire&);
    };

    void lock() {
        while (!testAndSetOrdered(Unlocked, Locked));
    }

    void unlock() {
        while (!testAndSetOrdered(Locked, Unlocked));
    }

    bool tryLock() {
        return testAndSetOrdered(Unlocked, Locked);
    }

private:
    static const int Unlocked = 1;
    static const int Locked = 0;
};

//
// dtkSpinLock.h ends here
