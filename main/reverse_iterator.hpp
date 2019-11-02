/* LEON::vector 用の ランダムアクセスイテレータ
* 
* <algorithm> などのライブラリで ::std::iterator_traits が
* 正常に動作するように
* ::std::random_access_iterator_tag をつけた
*/

#pragma once

#include <iterator> // ::std::random_access_iterator_tag

namespace LEON
{

template<typename Container>
class ReverseIterator
{
private:
    using iterator  = ReverseIterator;
    using size_type = typename Container::size_type;

public:
    using iterator_tag    = ::std::random_access_iterator_tag;
    using value_type      = typename Container::value_type;
    using difference_type = typename Container::difference_type;
    using pointer         = typename Container::pointer;
    using reference       = typename Container::reference;

    // 特殊関数
    ReverseIterator(const Container* container,
                    size_type index = 0):
        mContainer(container),
        mIndex(container->mSize - index - 1){}
    ~ReverseIterator()                                 = default;
    ReverseIterator(const ReverseIterator&)            = default;
    ReverseIterator(ReverseIterator&&)                 = default;
    ReverseIterator& operator=(const ReverseIterator&) = default;
    ReverseIterator& operator=(ReverseIterator&&)      = default;

    // random_access_iterator の要件を満たすための関数
    reference operator*()                      const {return mContainer->mData[mIndex];}
    pointer   operator->()                     const {return mContainer->mData + mIndex;}
    reference operator[](difference_type diff) const {return mContainer->mData[mContainer->mSize - diff- 1];}
    iterator& operator++()                           {mIndex--; return *this;}
    iterator  operator++(int)                        {iterator tmp = *this; mIndex--; return tmp;}
    iterator& operator--()                           {mIndex++; return *this;}
    iterator  operator--(int)                        {iterator tmp = *this; mIndex++; return tmp;}
    iterator& operator+=(difference_type diff)       {mIndex -= diff; return *this;}
    iterator& operator-=(difference_type diff)       {mIndex += diff; return *this;}

    friend iterator operator+(const iterator& iter,
                              difference_type diff){iterator tmp(iter); return tmp+=diff;}
    friend iterator operator+(difference_type diff,
                              const iterator& iter){return iter + diff;}
    friend iterator operator-(const iterator& iter,
                              difference_type diff){return iter + diff * -1;}
    friend difference_type operator-(const iterator& lhs,
                                     const iterator& rhs){return &*rhs - &*lhs;}
    friend bool operator==(const iterator& lhs,
                           const iterator& rhs){return &*lhs == &*rhs;}
    friend bool operator!=(const iterator& lhs,
                           const iterator& rhs){return &*lhs != &*rhs;}
    friend bool operator<(const iterator& lhs,
                          const iterator& rhs){return &*lhs > &*rhs;}
    friend bool operator<=(const iterator& lhs,
                           const iterator& rhs){return &*lhs >= &*rhs;}
    friend bool operator>(const iterator& lhs,
                          const iterator& rhs){return &*lhs < &*rhs;}
    friend bool operator>=(const iterator& lhs,
                           const iterator& rhs){return &*lhs <= &*rhs;}
    friend void swap(const iterator& lhs,
                     const iterator& rhs){auto tmp = *lhs; *lhs = *rhs; *rhs = tmp;}

private:
    Container* mContainer; // container
    size_type  mIndex;     // index
};

};