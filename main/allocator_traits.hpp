/*
* ::std::allocator_traits を模したもの
*/

/*
*  メンバ型の存在確認方法の実装を変更予定(detection idiom)
*  変更済み:
*  difference_type, size_type
*/

// 
/* 
*
* pointer_traits::rebind の呼び出しが良く解らなかった為、
* const_pointer, void_pointer, const_void_pointer
* はデフォルトを仮定義
*
* rebind_alloc<U>, rebind_traits<U> の実装は要調整
*
* ::std::numeric_limits::max() を type_traits を利用して実装？
* ::std::forward の実装？
*
* construct() の関数呼び出しの分岐
*/

#pragma once

#include "pointer_traits.hpp"
#include <type_traits>         // make_unsigned, is_empty, remove_reference ::std::true_type, ::std::false_type
#include <limits>              // numeric_limits::max (時間があれば max() は実装するかも)
#include <utility>             // forward
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
    using pointer = typename ::std::remove_reference<decltype(*get_pointer<allocator_type>(nullptr))>::type;

private:
    using traits = pointer_traits<pointer>;

    // const_pointer
    template<typename A>
    static typename A::const_pointer* get_const_pointer(typename A::const_pointer*){return nullptr;}
    template<typename>
    static DEFAULT_CONST_POINTER* get_const_pointer(...){return nullptr;} 
public:
    using const_pointer = typename ::std::remove_reference<decltype(*get_const_pointer<allocator_type>(nullptr))>::type;

private:
    // void_pointer
    template<typename A>
    static typename A::void_pointer* get_void_pointer(typename A::void_pointer*){return nullptr;}
    template<typename>
    static DEFAULT_VOID_POINTER* get_void_pointer(...){return nullptr;}
public:
    using void_pointer = typename ::std::remove_reference<decltype(*get_void_pointer<allocator_type>(nullptr))>::type;

private:
    // const_void_pointer
    template<typename A>
    static typename A::const_void_pointer* get_const_void_pointer(typename A::const_void_pointer*){return nullptr;}
    template<typename>
    static DEFAULT_CONST_VOID_POINTER* get_const_void_pointer(...){return nullptr;}
public:
    using const_void_pointer = typename ::std::remove_reference<decltype(*get_const_void_pointer<allocator_type>(nullptr))>;

private:
    // difference_type(fix)
    template<typename A, typename P, typename = void>
    struct _difference_type
        {using type = typename pointer_traits<P>::difference_type;};
    template<typename A, typename P>
    struct _difference_type<A, P, ::std::__void_t<typename A::difference_type>>
        {using type = typename A::difference_type;};
public:
    using difference_type = typename _difference_type<allocator_type, pointer>::type;

private:
    // size_type(fix)
    template<typename A, typename D, typename = void>
    struct _size_type : ::std::make_unsigned<D>{};
    template<typename A, typename D>
    struct _size_type<A, D, ::std::__void_t<typename A::size_type>>
        {using type = typename A::size_type;};
public:
    using size_type = typename _size_type<allocator_type, difference_type>::type;

private:
    // propagate_on_contatypename iner_copy_assignment
    template<typename A>
    static typename A::propagate_on_container_copy_assignment* get_propagate_on_container_copy_assignment(typename A::propagate_on_container_copy_assignment*){return nullptr;}
    template<typename>
    static ::std::false_type* get_propagate_on_container_copy_assignment(...){return nullptr;}
public:
    using propagate_on_container_copy_assignment = typename ::std::remove_reference<decltype(*get_propagate_on_container_copy_assignment<allocator_type>(nullptr))>::type;

private:
    // propagate_on_container_move_assignment
    template<typename A>
    static typename A::propagate_on_container_move_assignment* get_propagate_on_container_move_assignment(typename A::propagate_on_container_move_assignment*){return nullptr;}
    template<typename>
    static ::std::false_type* get_propagate_on_container_move_assignment(...){return nullptr;}
public:
    using propagate_on_container_move_assignment = typename ::std::remove_reference<decltype(*get_propagate_on_container_move_assignment<allocator_type>(nullptr))>;

private:
    // propagate_on_container_swap
    template<typename A>
    static typename A::propagate_on_container_swap* get_propagate_on_container_swap(typename A::propagate_on_container_swap*){return nullptr;}
    template<typename>
    static ::std::false_type* get_propagate_on_container_swap(...){return nullptr;}
public:
    using propagate_on_container_swap = typename ::std::remove_reference<decltype(*get_propagate_on_container_swap<allocator_type>(nullptr))>::type;

private:
    // is_always_equal
    using default_is_always_equal = typename ::std::is_empty<allocator_type>::type;
    template<typename A>
    static typename A::is_always_equal* get_is_always_equal(typename A::is_always_equal*){return nullptr;}
    template<typename A>
    static default_is_always_equal* get_is_always_equal(...){return nullptr;}
public:
    using is_always_equal = typename ::std::remove_reference<decltype(*get_is_always_equal<allocator_type>(nullptr))>::type;

private:
    // rebind_alloc<U>
    template<typename A, typename U, typename...>
    static typename A::rebind::other* get_rebind_alloc(typename A::rebind::other*){return nullptr;}
    // template<typename A, typename U, typename... Args>
    // static auto get_rebind_alloc(...)decltype(){return nullptr;}
public:
    template<typename U>
    using rebind_alloc = typename ::std::remove_reference<decltype(*get_rebind_alloc<allocator_type, U>(nullptr))>::type;
    
    // rebind_traits<U>
    template<typename U>
    using rebind_trais = allocator_traits<rebind_alloc<U>>;

private:
    // allocator_type に対応する関数が存在するか確認するための関数
    // allocate(hint)
    template<typename A>
    static pointer _allocate(A& alloc, size_type n, const_void_pointer hint, int)
        {return alloc.allocate(n, hint);}
    template<typename A>
    static pointer _allocate(A& alloc, size_type n, const_void_pointer, ...)
        {return alloc.allocate(n);}
    // max_size
    template<typename A>
    static auto _max_size(A& alloc, int)->decltype(alloc.max_size())
        {return alloc.max_size();}
    template<typename A>
    static size_type _max_size(A&, ...)
        {return ::std::numeric_limits<size_type>::max() / sizeof(value_type);}
    // construct
    // template<typename A, typename T, typename... Args>
    // static void _construct(A& alloc, T* ptr, int, Args&&... args)
    //     {alloc.construct(ptr, ::std::forward<Args>(args)...);}
    // template<typename A, typename T, typename... Args>
    // static void _construct(A& alloc, T* ptr, long, Args&&... args)
    //     {::new(static_cast<void*>(ptr)) T(::std::forward<Args>(args)...);}
    
    // // construct(fix)
    // template<typename T, typename... Args>
    // struct _construct_check
    // {
    //     template<typename A, typename = decltype(::std::declval<A*>()->construct(::std::declval<T*>(), ::std::declval<Args>()...))>
    //     static ::std::true_type check(int);
    //     template<typename>
    //     static ::std::false_type check(...);

    //     using type = decltype(check<allocator_type>(0));
    // };
    // template<typename>
    // struct _construct
    // {
    //     template<typename A, typename T, typename... Args>
    //     static void construct(A&, T* ptr, Args&&... args)
    //         {::new(static_cast<void*>(ptr)) T(::std::forward<Args>(args)...);}
    // };
    // template<typename>
    // struct _construct<void>
    // {
    //     template<typename, typename T, typename... Args>
    //     static void construct(A& alloc, T* ptr, Args&&... args)
    //         {alloc.construct(ptr, ::std::forward<Args>(args)...);}
    // };
    
    // destroy
    template<typename A, typename T>
    static void _destroy(A& alloc, T* ptr, int)
        {alloc.destroy(ptr);}
    template<typename A, typename T>
    static void _destroy(A& alloc, T* ptr, ...)
        {ptr->~T();}
    // select_on_container_copy_construction()
    template<typename A>
    static allocator_type _select_on_container_copy_construction(const A& alloc, int)
        {return alloc.select_on_container_copy_construction();}
    template<typename A>
    static allocator_type _select_on_container_copy_construction(const A& alloc, ...)
        {return alloc;}

public:
    // メンバ関数
    // allocate
    static pointer allocate(allocator_type& alloc,
                            size_type n)
        {return alloc.allocate(n);}
    static pointer allocate(allocator_type& alloc,
                            size_type n,
                            const_void_pointer hint)
        {return _allocate<allocator_type>(alloc, n, hint, 0);}
    // deallocate
    static void deallocate(allocator_type& alloc,
                           pointer ptr,
                           size_type n)
        {alloc.deallocate(ptr, n);}
    // max_size
    static size_type max_size(allocator_type& alloc)                
        {return _max_size<allocator_type>(alloc, 0);}
    static size_type max_size(const allocator_type& alloc) noexcept
        {return _max_size<const allocator_type>(alloc, 0);}
    // construct
    template<typename T, typename... Args>
    static void construct(allocator_type& alloc,
                          T* ptr,
                          Args&&... args)
        {::new(static_cast<void*>(ptr)) T(::std::forward<Args>(args)...);}
    // destroy
    template<typename T>
    static void destroy(allocator_type& alloc,
                        T* ptr)
        {_destroy<allocator_type, T>(alloc, ptr);}
    // select_on_container_copy_construction
    static allocator_type select_on_container_copy_construction(const allocator_type& alloc)
        {return _select_on_container_copy_construction<allocator_type>(alloc, 0);}
};

};