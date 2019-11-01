/*
* ::std::allocator_traits を模したもの
* 
* difference_type を 符号なしに変換する方法が解らなかったため,
* <type_traits> を include
*
* pointer_traits::rebind の呼び出しが良く解らなかった為、
* const_pointer, void_pointer, const_void_pointer
* はデフォルトを仮定義
*
* :
* 以下の実行結果は, std::, LEON:: 共に期待した値が得られた。
* typeid(typename pointer_traits<pointer>::rebind<const value_type>)
*
* rebind_alloc<U>, rebind_traits<U> の実装は要調整
*/

#pragma once

#include "pointer_traits.hpp"
#include <type_traits> // make_unsigned, is_empty ::std::true_type, ::std::false_type

namespace LEON
{

#define DEFAULT_CONST_POINTER      const value_type*
#define DEFAULT_VOID_POINTER       void*
#define DEFAULT_CONST_VOID_POINTER const void*

template<typename Alloc>
class allocator_traits
{
public:
    using allocator_type = Alloc;
    using value_type     = typename allocator_type::value_type;

private:
    // SFINAE で型特性を取得する関数の作成
    // pointer
    template<typename A>
    static typename A::pointer* get_pointer(typename A::pointer*){return nullptr;}
    template<typename>
    static value_type** get_pointer(...){return nullptr;}

public:
    using pointer = decltype(*get_pointer<allocator_type>(nullptr));

private:
    using traits = pointer_traits<pointer>;

    // const_pointer
    template<typename A>
    static typename A::const_pointer* get_const_pointer(typename A::const_pointer*){return nullptr;}
    template<typename>
    static DEFAULT_CONST_POINTER* get_const_pointer(...){return nullptr;} 
public:
    using const_pointer = decltype(*get_const_pointer<allocator_type>(nullptr));

private:
    // void_pointer
    template<typename A>
    static typename A::void_pointer* get_void_pointer(typename A::void_pointer*){return nullptr;}
    template<typename>
    static DEFAULT_VOID_POINTER* get_void_pointer(...){return nullptr;}
public:
    using void_pointer = decltype(*get_void_pointer<allocator_type>(nullptr));

private:
    // const_void_pointer
    template<typename A>
    static typename A::const_void_pointer* get_const_void_pointer(typename A::const_void_pointer*){return nullptr;}
    template<typename>
    static DEFAULT_CONST_VOID_POINTER* get_const_void_pointer(...){return nullptr;}
public:
    using const_void_pointer = decltype(*get_const_void_pointer<allocator_type>(nullptr));

private:
    // difference_type
    template<typename A>
    static typename A::difference_type* get_difference_type(typename A::difference_type*){return nullptr;}
    template<typename A>
    static typename A::traits::difference_type* get_difference_type(typename A::traits::difference_type*){return nullptr;}
public:
    using difference_type = decltype(*get_difference_type<allocator_type>(nullptr));

private:
    // ::std::make_unsigned を使うために difference_type を実体化
    static difference_type diff_t;

    // size_type
    template<typename A>
    static typename A::size_type* get_size_type(typename A::size_type*){return nullptr;}
    template<typename>
    static typename ::std::make_unsigned<decltype(diff_t)>* get_size_type(...){return nullptr;}
public:
    using size_type = decltype(*get_size_type<allocator_type>(nullptr));

private:
    // propagate_on_contatypename iner_copy_assignment
    template<typename A>
    static typename A::propagate_on_container_copy_assignment* get_propagate_on_container_copy_assignment(typename A::propagate_on_container_copy_assignment*){return nullptr;}
    template<typename>
    static ::std::false_type* get_propagate_on_container_copy_assignment(...){return nullptr;}
public:
    using propagate_on_container_copy_assignment = decltype(*get_propagate_on_container_copy_assignment<allocator_type>(nullptr));

private:
    // propagate_on_container_move_assignment
    template<typename A>
    static typename A::propagate_on_container_move_assignment* get_propagate_on_container_move_assignment(typename A::propagate_on_container_move_assignment*){return nullptr;}
    template<typename>
    static ::std::false_type* get_propagate_on_container_move_assignment(...){return nullptr;}
public:
    using propagate_on_container_move_assignment = decltype(*get_propagate_on_container_move_assignment<allocator_type>(nullptr));

private:
    // propagate_on_container_swap
    template<typename A>
    static typename A::propagate_on_container_swap* get_propagate_on_container_swap(typename A::propagate_on_container_swap*){return nullptr;}
    template<typename>
    static ::std::false_type* get_propagate_on_container_swap(...){return nullptr;}
public:
    using propagate_on_container_swap = decltype(*get_propagate_on_container_swap<allocator_type>(nullptr));

private:
    // is_always_equal
    using default_is_always_equal = typename ::std::is_empty<allocator_type>::type;
    template<typename A>
    static typename A::is_always_equal* get_is_always_equal(typename A::is_always_equal*){return nullptr;}
    template<typename A>
    static default_is_always_equal* get_is_always_equal(...){return nullptr;}
public:
    using is_always_equal = decltype(*get_is_always_equal<allocator_type>(nullptr));

private:
    // rebind_alloc<U>
    template<typename A, typename U>
    static typename A::rebind::other* get_rebind_alloc(typename A::rebind::other*){return nullptr;}
    // template<typename A, typename U, typename... Args>
    // static A<U, Args>* get_rebind_alloc(...){return nullptr;}
public:
    template<typename U>
    using rebind_alloc = decltype(*get_rebind_alloc<allocator_type, U>(nullptr));
    
    // rebind_traits<U>
    template<typename U>
    using rebind_trais = allocator_traits<rebind_alloc<U>>;
};

};