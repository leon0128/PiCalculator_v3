/* 動的配列 vector
* 
* ::std::vector 同様、各要素はメモリ上に連続して配置される
* bool 特殊化 は行っていない
*
*  template を使用した overload が正しく動作しない
*  優先度の問題？
*  
*  template<typename InputIter>
*  vector() について、
*  イテレータ以外もこっちが優先的に使われる問題(未実装)
*  initializer_list を使用した際に正しく処理されない
*
*  copyが禁止されているオブジェクトのムーブが正しく処理されない
*
*  コンストラクタで大きさを指定した時にlength_error か、
*  capacity がオーバーフローのチェックを追加
*
*/

#pragma once

#include "allocator.hpp"
#include "allocator_traits.hpp"
#include "iterator.hpp"
#include "reverse_iterator.hpp"
#include "type_traits.hpp"
#include "utility.hpp"

#include <initializer_list> // initializer_list
#include <iostream>
#include <stdexcept>

namespace LEON
{

template<typename>
class Iterator;
template<typename>
class ReverseIterator;

template<typename T, typename Allocator = allocator<T>>
class vector
{
private:
    // コンテナを操作できるイテレータ
    friend Iterator<vector>;
    friend ReverseIterator<vector>;

    using traits = allocator_traits<Allocator>;

public:
    using value_type             = typename traits::value_type;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = typename traits::pointer;
    using const_pointer          = typename traits::const_pointer;
    using difference_type        = typename traits::difference_type;
    using size_type              = typename traits::size_type;
    using iterator               = Iterator<vector>;
    using const_iterator         = const iterator;
    using reverse_iterator       = ReverseIterator<vector>;
    using const_reverse_iterator = const reverse_iterator;
    using allocator_type         = typename traits::allocator_type;

    // コンストラクタ
    vector():
        mData(nullptr),
        mSize(0),
        mCapacity(0),
        mAllocator(){}
    explicit vector(const allocator_type& alloc):
        mData(nullptr),
        mSize(0),
        mCapacity(0),
        mAllocator(alloc){}
    explicit vector(size_type n,
                    const allocator_type& alloc = allocator_type()):
        mData(nullptr),
        mSize(n),
        mCapacity(n * 2),
        mAllocator(alloc)
    {
        mData = traits::allocate(mAllocator,
                                 mCapacity);
        for(size_type i = 0; i < n; i++)
        {
            traits::construct(mAllocator,
                              mData + i);
        }
    }
    vector(size_type n,
           const_reference value,
           const allocator_type& alloc = allocator_type()):
        mData(nullptr),
        mSize(n),
        mCapacity(n * 2),
        mAllocator(alloc)
    {
        mData = traits::allocate(mAllocator,
                                 mCapacity);
        for(size_type i = 0; i < n; i++)
        {
            traits::construct(mAllocator,
                              mData + i,
                              value);
        }
    }
    vector(const vector& other):
        vector(other,
               traits::select_on_container_copy_construction(mAllocator)){}
    vector(vector&& other):
        vector(move(other),
               traits::select_on_container_copy_construction(mAllocator)){}
    vector(const vector& other,
           const allocator_type& alloc):
        mData(nullptr),
        mSize(other.mSize),
        mCapacity(other.mCapacity),
        mAllocator(alloc)
    {
        mData = traits::allocate(mAllocator,
                                 mCapacity);
        for(size_type i = 0; i < other.mSize; i++)
        {
            traits::construct(mAllocator,
                              mData + i,
                              *(other.mData + i));
        }
    }
    vector(vector&& other,
           const allocator_type& alloc):
        mData(other.mData),
        mSize(other.mSize),
        mCapacity(other.mCapacity),
        mAllocator(alloc)
    {
        other.mData = nullptr;
        other.mSize = 0;
        other.mCapacity = 0;
    }
    vector(::std::initializer_list<value_type> list,
           const allocator_type& alloc):
        mData(nullptr),
        mSize(list.size()),
        mCapacity(list.size() * 2),
        mAllocator(alloc)
    {
        mData = traits::allocate(mAllocator,
                                 mCapacity);
        for(size_type i = 0; i < mSize; i++)
        {
            traits::construct(mAllocator,
                              mData + i,
                              *(list.begin() + i));
        }
    }
    // デストラクタ
    ~vector()
    {
        if(!is_trivially_destructible<value_type>::value)
        {
            for(size_type i = 0; i < mSize; i++)
            {
                traits::destroy(mAllocator,
                                mData + i);
            }
        }
        traits::deallocate(mAllocator,
                           mData,
                           mCapacity);
    }
    // 代入演算子
    vector& operator=(const vector& other)
    {
        if(this == &other)
            return *this;

        if(!is_trivially_destructible<value_type>::value)
        {
            for(size_type i = 0; i < mSize; i++)
            {
                traits::destroy(mAllocator,
                                mData + i);
            }
        }
        traits::deallocate(mAllocator,
                           mData,
                           mCapacity);
        
        mSize = other.mSize;
        mCapacity = other.mCapacity;
        mAllocator = traits::select_on_container_copy_construction(other.mAllocator);
        
        mData = traits::allocate(mAllocator,
                                 mCapacity);
        for(size_type i = 0; i < mSize; i++)
        {
            traits::construct(mAllocator,
                              mData + i,
                              *(other.mData + i));
        }

        return *this;
    }
    vector& operator=(vector&& other)
    {
        if(this == &other)
            return *this;
        
        if(!is_trivially_destructible<value_type>::value)
        {
            for(size_type i = 0; i < mSize; i++)
            {
                traits::destroy(mAllocator,
                                mData + i);
            }
        }
        traits::deallocate(mAllocator,
                           mData,
                           mCapacity);
        
        mData = other.mData;
        mSize = other.mSize;
        mCapacity = other.mCapacity;
        mAllocator = traits::select_on_container_copy_construction(other.mAllocator);
    
        return *this;
    }
    vector& operator=(::std::initializer_list<value_type> list)
    {
        if(!is_trivially_destructible<value_type>::value)
        {
            for(size_type i = 0; i < mSize; i++)
            {
                traits::destroy(mAllocator,
                                mData + i);
            }
        }
        traits::deallocate(mAllocator,
                           mData,
                           mCapacity);
        
        mSize = list.size();
        mCapacity = list.size() * 2;
        mAllocator = traits::select_on_container_copy_construction(mAllocator);

        mData = traits::allocate(mAllocator,
                                 mCapacity);
        for(size_type i = 0; i < mSize; i++)
        {
            traits::construct(mAllocator,
                              mData + i,
                              *(list.begin() + i));
        }

        return *this;
    }

    // イテレータ
    iterator               begin()         noexcept {return iterator(this);}
    const_iterator         begin()   const noexcept {return iterator(this);}
    iterator               end()           noexcept {return iterator(this, mSize);}
    const_iterator         end()     const noexcept {return iterator(this, mSize);}
    const_iterator         cbegin()  const noexcept {return iterator(this);}
    const_iterator         cend()    const noexcept {return iterator(this, mSize);}
    reverse_iterator       rbegin()        noexcept {return reverse_iterator(this);}
    const_reverse_iterator rbegin()  const noexcept {return reverse_iterator(this);}
    reverse_iterator       rend()          noexcept {return reverse_iterator(this, mSize);}
    const_reverse_iterator rend()    const noexcept {return reverse_iterator(this, mSize);}
    const_reverse_iterator crbegin() const noexcept {return reverse_iterator(this);}
    const_reverse_iterator crend()   const noexcept {return reverse_iterator(this, mSize);}

    // 領域
    size_type size()     const noexcept {return mSize;}
    size_type max_size() const noexcept {return traits::max_size(mAllocator);}
    size_type capacity() const noexcept {return mCapacity;}
    bool      empty()    const noexcept {return (mSize == 0) ? true : false;}
    // resize
    void resize(size_type sz, const value_type& value)
    {
        size_type diff = (mSize > sz) ? (mSize - sz) : (sz - mSize);
        
        if(mSize > sz)
        {
            for(size_type i = 0; i < diff; i++)
                pop_back();
        }
        else if(mSize < sz)
        {
            if(sz > mCapacity)
                reserve(sz);

            for(size_type i = 0; i < diff; i++)
            {
                traits::construct(mAllocator,
                                  mData + mSize + i,
                                  value);
            }
            mSize = sz;
        }
    }
    void resize(size_type sz)
        {resize(sz, value_type());}
    // reserve
    void reserve(size_type sz)
    {
        if(sz <= mCapacity)
            return;

        if(sz > max_size())
            throw ::std::length_error("The maximum number of elementes that can be allocated has been exceeded.");
        
        pointer lastData = mData;
        mData = traits::allocate(mAllocator,
                                 sz);
        
        if(is_copy_constructible<value_type>::value)
        {
            for(size_type i = 0; i < mSize; i++)
            {
                traits::construct(mAllocator,
                                  mData + i,
                                  *(lastData + i));
            }
        }
        else
        {
            for(size_type i = 0; i < mSize; i++)
            {
                traits::construct(mAllocator,
                                  mData + i,
                                  move(*(lastData + i)));
            }
        }

        if(!is_trivially_destructible<value_type>::value)
        {
            for(size_type i = 0; i < mSize; i++)
            {
                traits::destroy(mAllocator,
                                lastData + i);
            }
        }
        traits::deallocate(mAllocator,
                           lastData,
                           mCapacity);
        
        mCapacity = sz;
    }
    // shrink_to_fit
    void shrink_to_fit()
    {
        pointer lastData = mData;
        mData = traits::allocate(mAllocator,
                                 mSize);
        
        if(is_copy_constructible<value_type>::value)
        {
            for(size_type i = 0; i < mSize; i++)
            {
                traits::construct(mAllocator,
                                  mData + i,
                                  *(lastData + i));
            }
        }
        else
        {
            for(size_type i = 0; i < mSize; i++)
            {
                traits::construct(mAllocator,
                                  mData + i,
                                  move(*(lastData + i)));
            }
        }

        if(!is_trivially_destructible<value_type>::value)
        {
            for(size_type i = 0; i < mSize; i++)
            {
                traits::destroy(mAllocator,
                                lastData + i);
            }
        }
        traits::deallocate(mAllocator,
                           lastData,
                           mCapacity);
        
        mCapacity = mSize;
    }

    // 要素アクセス
    reference       operator[](size_type n)                {return mData[n];}
    const_reference operator[](size_type n) const          {return mData[n];}
    pointer         data()                        noexcept {return mData;}
    const_pointer   data()                  const noexcept {return mData;}
    reference       front()                                {return mData[0];}
    const_reference front()                 const          {return mData[0];}
    reference       back()                                 {return mData[mSize - 1];}
    const_reference back()                  const          {return mData[mSize - 1];}
    // at
    reference at(size_type n)
    {
        if(n >= mSize)
            throw ::std::out_of_range("Invalid index.");
        return mData[n];
    }
    const_reference at(size_type n) const
    {
        if(n >= mSize)
            throw ::std::out_of_range("Invalid index.");
        return mData[n];
    }

    // コンテナの変更
    // assign
    void assign(size_type n, const value_type& value)
    {
        clear();

        size_type sz = (n * 2 >= n) ? n * 2 : max_size();
        mData = traits::allocate(mAllocator,
                                 sz);
        
        
        for(size_type i = 0; i < n; i++)
        {
            traits::construct(mAllocator,
                              mData + i,
                              value);
        }

        mSize = n;
        mCapacity = sz;
    }
    void assign(::std::initializer_list<value_type> list)
    {
        clear();

        size_type sz = (list.size() * 2 >= list.size()) ? list.size() * 2 : max_size();
        mData = traits::allocate(mAllocator,
                                 sz);
        
        
        for(size_type i = 0; i < list.size(); i++)
        {
            traits::construct(mAllocator,
                              mData + i,
                              *(list.begin() + i));
        }

        mSize = list.size();
        mCapacity = sz;
    }
    // push_back
    void push_back(const value_type& value)
    {
        if(mSize >= mCapacity)
            reserve((mSize * 2 >= mSize) ? (mSize + 1) * 2 : max_size());

        traits::construct(mAllocator,
                          mData + mSize,
                          value);
        mSize++;
    }
    void push_back(value_type&& value)
    {
        if(mSize >= mCapacity)
            reserve((mSize * 2 >= mSize) ? (mSize + 1) * 2 : max_size());

        traits::construct(mAllocator,
                          mData + mSize,
                          move(value));
        mSize++;
    }
    // emplace_back
    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        if(mSize >= mCapacity)
            reserve((mSize * 2 >= mSize) ? (mSize + 1) * 2 : max_size());

        traits::construct(mAllocator,
                          mData + mSize,
                          forward<Args>(args)...);
        mSize++;
    }
    // pop_back
    void pop_back()
    {
        if(!empty())
        {
            if(!is_trivially_destructible<value_type>::value)
            {
                traits::destroy(mAllocator,
                                mData + mSize - 1);
            }
            mSize--;
        }
    }
    // insert
    iterator insert(const_iterator pos,
                    const value_type& value)
        {insert(pos, 1, value);}
    iterator insert(const_iterator pos,
                    value_type&& value)
    {
        if(mSize >= mCapacity)
            reserve((mSize * 2 >= mSize) ? (mSize + 1) * 2 : max_size());

        for(size_type i = mSize; i > pos.mIndex; i--)
        {
            traits::construct(mAllocator,
                              mData + i,
                              move(*(mData + i - 1)));
            if(!is_trivially_destructible<value_type>::value)
            {
                traits::destroy(mAllocator,
                                mData + i - 1);
            }
        }
        
        traits::construct(mAllocator,
                          mData + pos.mIndex,
                          move(value));
        
        mSize++;
        return iterator(this, pos.mIndex);
    }
    iterator insert(const_iterator pos,
                    size_type n,
                    const value_type& value)
    {
        if(mSize + n - 1 >= mCapacity)
            reserve(((mSize + n - 1) * 2 >= mSize) ? (mSize + n) * 2 : max_size());

        for(size_type i = mSize; i > pos.mIndex; i--)
        {
            traits::construct(mAllocator,
                              mData + i + n - 1,
                              *(mData + i - 1));
            if(!is_trivially_destructible<value_type>::value)
            {
                traits::destroy(mAllocator,
                                mData + i - 1);
            }
        }
        
        for(size_type i = 0; i < n; i++)
        {
            traits::construct(mAllocator,
                              mData + pos.mIndex + i,
                              value);
        }

        mSize += n;
        return iterator(this, pos.mIndex);
    }
    iterator insert(const_iterator pos,
                    ::std::initializer_list<value_type> list)
    {
        size_type n = list.size();
        if(mSize + n - 1 >= mCapacity)
            reserve(((mSize + n - 1) * 2 >= mSize) ? (mSize + n) * 2 : max_size());


        for(size_type i = mSize; i > pos.mIndex; i--)
        {
            traits::construct(mAllocator,
                              mData + i + n - 1,
                              *(mData + i - 1));
            if(!is_trivially_destructible<value_type>::value)
            {
                traits::destroy(mAllocator,
                                mData + i - 1);
            }
        }
        
        for(size_type i = 0; i < n; i++)
        {
            traits::construct(mAllocator,
                              mData + pos.mIndex + i,
                              *(list.begin() + i));
        }

        mSize += n;
        return iterator(this, pos.mIndex);
    }
    // emplace
    template<typename... Args>
    iterator emplace(const_iterator pos,
                     Args&&... args)
    {
        if(mSize >= mCapacity)
            reserve((mSize * 2 >= mSize) ? (mSize + 1) * 2 : max_size());

        for(size_type i = mSize; i > pos.mIndex; i--)
        {
            traits::construct(mAllocator,
                              mData + i,
                              move(*(mData + i - 1)));
            if(!is_trivially_destructible<value_type>::value)
            {
                traits::destroy(mAllocator,
                                mData + i - 1);
            }
        }

        traits::construct(mAllocator,
                          mData + pos.mIndex,
                          forward<Args>(args)...);

        mSize++;
        return iterator(this, pos.mIndex);
    }
    // erase
    iterator erase(const_iterator pos)
        {return erase(pos, pos + 1);}
    iterator erase(const_iterator first,
                   const_iterator last)
    {
        if(!is_trivially_destructible<value_type>::value)
        {
            for(auto iter = first; iter != last; iter++)
            {
                traits::destroy(mAllocator,
                                &*iter);
            }
        }

        auto n = last - first;
        if(is_move_assignable<value_type>::value)
        {
            for(size_type i = last.mIndex; i < mSize; i++)
                *(mData + i - n) = move(*(mData + i));
        }
        else
        {
            for(size_type i = last.mIndex; i < mSize; i++)
                *(mData + i - n) = *(mData + i);
        }

        mSize -= last - first;
        return iterator(this, last.mIndex);
    }
    // swap
    void swap(vector& other)
    {
        pointer   ptr = mData;
        size_type sz  = mSize;
        size_type cap = mCapacity;
        allocator_type all 
            = traits::select_on_container_copy_construction(mAllocator);
        
        mData = other.mData;
        mSize = other.mSize;
        mCapacity = other.mCapacity;
        mAllocator
            = traits::select_on_container_copy_construction(mAllocator);
        
        other.mData = ptr;
        other.mSize = sz;
        other.mCapacity  = cap;
        other.mAllocator = all;
    }
    // clear
    void clear()
    {
        if(!is_trivially_destructible<value_type>::value)
        {
            for(size_type i = 0; i < mSize; i++)
            {
                traits::destroy(mAllocator,
                                mData + i);
            }
        }

        traits::deallocate(mAllocator,
                           mData,
                           mCapacity);
        mData = nullptr;
        mSize = 0;
        mCapacity = 0;
    }

    // アロケータ
    allocator_type get_allocator() const noexcept
        {return traits::select_on_container_copy_construction(mAllocator);}

private:
    pointer   mData;           // 先頭アドレス
    size_type mSize;           // 格納されてる要素数
    size_type mCapacity;       // メモリを再確保せずに格納できる最大要素数
    allocator_type mAllocator; // アロケータ
};

// 比較演算子
template<typename _T, typename _A>
bool operator==(const vector<_T, _A>& lhs,
                const vector<_T, _A>& rhs)
{
    if(lhs.size() != rhs.size())
        return false;
    
    for(auto i = 0; i < lhs.size(); i++)
    {
        if(*lhs[i] != *rhs[i])
            return false;
    }

    return true;
}
template<typename _T, typename _A>
bool operator!=(const vector<_T, _A>& lhs,
                const vector<_T, _A>& rhs)
    {return !(lhs == rhs);}
template<typename _T, typename _A>
bool operator<(const vector<_T, _A>& lhs,
               const vector<_T, _A>& rhs)
{
    if(lhs.size() < rhs.size())
        return true;
    
    if(lhs.empty())
        return false;

    if(lhs.back() < rhs.back())
        return true;
    else
        return false;
}
template<typename _T, typename _A>
bool operator<=(const vector<_T, _A>& lhs,
                const vector<_T, _A>& rhs)
{
    if(lhs.size() <= rhs.size())
        return true;
    
    if(lhs.empty())
        return true;

    if(lhs.back() <= rhs.back())
        return true;
    else
        return false;
}
template<typename _T, typename _A>
bool operator>(const vector<_T, _A>& lhs,
               const vector<_T, _A>& rhs)
{
    if(lhs.size() > rhs.size())
        return true;
    
    if(lhs.empty())
        return false;

    if(lhs.back() > rhs.back())
        return true;
    else
        return false;
}
template<typename _T, typename _A>
bool operator>=(const vector<_T, _A>& lhs,
                const vector<_T, _A>& rhs)
{
    if(lhs.size() >= rhs.size())
        return true;
    
    if(lhs.empty())
        return true;

    if(lhs.back() >= rhs.back())
        return true;
    else
        return false;
}
// swap
template<typename _T, typename _A>
void swap(vector<_T, _A>& lhs,
          vector<_T, _A>& rhs)
    {lhs.swap(rhs);}

};