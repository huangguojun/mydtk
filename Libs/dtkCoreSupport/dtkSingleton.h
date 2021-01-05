#ifndef DTKSINGLETON_H
#define DTKSINGLETON_H

#include <QtCore/QMutex>

template<class T>
class dtkSingleton
{
public:
    static T &instance(void)
    {
        static QMutex mutex;

        if (!s_instance) {

            mutex.lock();

            if (!s_instance)
                s_instance = new T;

            mutex.unlock();
        }

        return (*s_instance);
    }

private:
    dtkSingleton(void){};
    ~dtkSingleton(void){};

private:
    Q_DISABLE_COPY(dtkSingleton)

private:
    static T *s_instance;
};

template<class T>
T *dtkSingleton<T>::s_instance = NULL;

#define DTK_IMPLEMENT_SINGLETON(T)                                                                 \
    T *T::instance() { return &(dtkSingleton<T>::instance()); }

#endif // DTKSINGLETON
