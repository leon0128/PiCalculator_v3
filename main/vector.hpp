/* 動的配列 vector
* 
* ::std::vector 同様、各要素はメモリ上に連続して配置される
*
*
*  template<typename InputIter>
*  vector() について、
*  イテレータ以外もこっちが優先的に使われる問題(未実装)
*/

#pragma once

#include "allocator.hpp"
#include "allocator_traits.hpp"
#include "iterator.hpp"
#include "reverse_iterator.hpp"

#include <type_traits> // is_trivially_destructible
#include <utility>     // move, initializer_list

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
        vector(std::move(other),
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
        if(!::std::is_trivially_destructible<value_type>::value)
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

        if(!::std::is_trivially_destructible<value_type>::value)
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
        
        if(!::std::is_trivially_destructible<value_type>::value)
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
        if(!::std::is_trivially_destructible<value_type>::value)
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
    }

    

private:
    pointer   mData;           // 先頭アドレス
    size_type mSize;           // 格納されてる要素数
    size_type mCapacity;       // メモリを再確保せずに格納できる最大要素数
    allocator_type mAllocator; // アロケータ
};

};