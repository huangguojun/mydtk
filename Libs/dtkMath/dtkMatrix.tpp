/* dtkMatrix.tpp ---
 *
 * Author: Julien Wintz
 * Created: Tue Jul 16 11:18:48 2013 (+0200)
 * Version:
 * Last-Updated: Tue Jul 16 18:14:00 2013 (+0200)
 *           By: Julien Wintz
 *     Update #: 27
 */

/* Change Log:
 *
 */

template <typename T> dtkMatrixPrivate<T>::dtkMatrixPrivate(void)
{
    this->buffer = NULL;
}

template <typename T> dtkMatrixPrivate<T>::dtkMatrixPrivate(qulonglong r, qulonglong c)
{
    this->allocate(r, c);
}

template <typename T> dtkMatrixPrivate<T>::~dtkMatrixPrivate(void)
{
    if (this->buffer)
        delete[] this->buffer;
}

template <typename T> T dtkMatrixPrivate<T>::at(qulonglong i, qulonglong j)
{
    return this->buffer[i * this->r_count + j];
}

template <typename T> void dtkMatrixPrivate<T>::setAt(qulonglong i, qulonglong j, T value)
{
    this->buffer[i * this->r_count + j] = value;
}

template <typename T> void dtkMatrixPrivate<T>::allocate(qulonglong c, qulonglong r)
{
    this->buffer = new T[r * c];
    this->r_count = r;
    this->c_count = c;
}
