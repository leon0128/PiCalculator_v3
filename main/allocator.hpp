/*
* ::LEON::vector で使うための allocator
* 最低限の機能のみ
* c++17 で非推奨となった関数は未実装、メンバ型は未定義
* (LEON::allocator_traits で実装したものを使用)
*/

#pragma once

#include <cstdlib>     // malloc(), free()
#include <type_traits> // true_type

namespace LEON
{

template<typename T>
class allocator
{
public:
    using value_type      = T;
    using is_always_equal = ::std::true_type;
    using propagate_on_container_move_assignment = ::std::true_type;

    // コンストラクタ
    allocator()                 noexcept {}
    allocator(const allocator&) noexcept {}
    template<typename U>
    allocator(const allocator<U>&) noexcept {}
    // デストラクタ
    ~allocator(){}
    // 代入演算子
    allocator& operator=(const allocator&) = default;

    // メモリ確保
    value_type* allocate(unsigned long long int n)
        {return static_cast<value_type*>(::std::malloc(n * sizeof(value_type)));}
    // メモリ解放
    void deallocate(value_type* ptr,
                    unsigned long long int n)
        {::std::free(ptr);}
};

};