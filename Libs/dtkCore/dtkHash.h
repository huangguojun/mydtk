// Version: $Id: 8e394c25990ea95b37455c77c558077859251723 $
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

#include <complex>
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <utility>
#include <vector>

// /////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////

template<typename SizeT>
inline void dtkHashCombineImpl(SizeT &seed, SizeT value)
{
    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename T>
inline void dtkHashCombine(std::size_t &seed, const T &v)
{
    std::hash<T> hasher;
    dtkHashCombineImpl(seed, hasher(v));
}

template<typename It>
inline std::size_t dtkHashRange(It first, It last)
{
    std::size_t seed = 0;

    for (; first != last; ++first) {
        dtkHashCombineImpl(seed, *first);
    }

    return seed;
}

// /////////////////////////////////////////////////////////////////
// dtkHash functor
// /////////////////////////////////////////////////////////////////

struct dtkHash
{
public:
    template<typename T, typename U>
    std::size_t operator()(const std::pair<T, U> &c) const;

    template<typename T, typename A>
    std::size_t operator()(const std::vector<T, A> &c) const;

    template<typename T, typename A>
    std::size_t operator()(const std::list<T, A> &c) const;

    template<typename T, typename A>
    std::size_t operator()(const std::deque<T, A> &c) const;

    template<typename K, typename C, typename A>
    std::size_t operator()(const std::set<K, C, A> &c) const;

    template<typename K, typename C, typename A>
    std::size_t operator()(const std::multiset<K, C, A> &c) const;

    template<typename K, typename T, typename C, typename A>
    std::size_t operator()(const std::map<K, T, C, A> &c) const;

    template<typename K, typename T, typename C, typename A>
    std::size_t operator()(const std::multimap<K, T, C, A> &c) const;

    template<typename T>
    std::size_t operator()(const std::complex<T> &c) const;
};

// /////////////////////////////////////////////////////////////////
// dtkHash functor implementation
// /////////////////////////////////////////////////////////////////

template<typename T, typename U>
inline std::size_t dtkHash::operator()(const std::pair<T, U> &c) const
{
    std::size_t seed = 0;
    dtkHashCombine(seed, c.first);
    dtkHashCombine(seed, c.second);
    return seed;
}

template<typename T, typename A>
inline std::size_t dtkHash::operator()(const std::vector<T, A> &c) const
{
    return dtkHashRange(c.begin(), c.end());
}

template<typename T, typename A>
inline std::size_t dtkHash::operator()(const std::list<T, A> &c) const
{
    return dtkHashRange(c.begin(), c.end());
}

template<typename T, typename A>
inline std::size_t dtkHash::operator()(const std::deque<T, A> &c) const
{
    return dtkHashRange(c.begin(), c.end());
}

template<typename K, typename C, typename A>
inline std::size_t dtkHash::operator()(const std::set<K, C, A> &c) const
{
    return dtkHashRange(c.begin(), c.end());
}

template<typename K, typename C, typename A>
inline std::size_t dtkHash::operator()(const std::multiset<K, C, A> &c) const
{
    return dtkHashRange(c.begin(), c.end());
}

template<typename K, typename T, typename C, typename A>
inline std::size_t dtkHash::operator()(const std::map<K, T, C, A> &c) const
{
    return dtkHashRange(c.begin(), c.end());
}

template<typename K, typename T, typename C, typename A>
inline std::size_t dtkHash::operator()(const std::multimap<K, T, C, A> &c) const
{
    return dtkHashRange(c.begin(), c.end());
}

template<typename T>
inline std::size_t dtkHash::operator()(const std::complex<T> &c) const
{
    std::size_t seed = 0;
    dtkHashCombine(seed, c.imag());
    dtkHashCombine(seed, c.real());
    return seed;
}

//
// dtkHash.h ends here
