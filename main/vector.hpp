/* 動的配列 vector
* 
* ::std::vector 同様、各要素はメモリ上に連続して配置される
*
*/

#pragma once

#include "allocator.hpp"
#include "allocator_traits.hpp"
#include "iterator.hpp"
#include "reverse_iterator.hpp"

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
    explicit vector(size_type n):
        mData(nullptr),
        mSize(n),
        mCapacity(n * 2),
        mAllocator()
    {
        // デフォルトコンストラクタの呼び出し
        mData = traits::allocate(mAllocator,
                                 mCapacity);
        for(size_type i = 0; i < n; i++)
        {
            traits::construct(mAllocator,
                              mData + i);
        }
    }
        
    

private:
    pointer   mData;           // 先頭アドレス
    size_type mSize;           // 格納されてる要素数
    size_type mCapacity;       // メモリを再確保せずに格納できる最大要素数
    allocator_type mAllocator; // アロケータ
};

};