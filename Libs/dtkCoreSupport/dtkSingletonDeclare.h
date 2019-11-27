/* dtkSingletonDeclare.h ---
 *
 * Author: John Stark
 * Created: July 28 10:28:00 2011 (+0100)
 */

/* Change log:
 *
 */

#ifndef DTKSINGLETONDECLARE_H
#define DTKSINGLETONDECLARE_H

template<class T> class dtkSingleton;
template <typename T> struct QScopedPointerDeleter;

#define DTK_DECLARE_SINGLETON( T )          \
public:                                     \
    static T* instance();                   \
private:                                    \
    friend class dtkSingleton<T>;           \
    friend struct QScopedPointerDeleter<T>

#endif //DTKSINGLETONDECLARE_H
