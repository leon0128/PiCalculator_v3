#pragma once

#include "type_traits.hpp"

namespace LEON
{
    // 宣言
    template<typename>
    struct remove_reference;
    template<typename>
    struct add_rvalue_reference;

    /*
    * 転送と移動
    */

    // forward (c++14)
    // 関数テンプレートの引数を転送
    template<typename T>
    constexpr T&& forward(typename remove_reference<T>::type& t) noexcept
        {return static_cast<T&&>(t);}
    template<typename T>
    constexpr T&& forward(typename remove_reference<T>::type&& t) noexcept
        {return static_cast<T&&>(t);}

    // move (c++14)
    // 左辺値を右辺値にキャスト
    template<typename T>
    constexpr typename remove_reference<T>::type&& move(T&& t) noexcept
        {return static_cast<typename remove_reference<T>::type&&>(t);}

    /*
    * 型の値
    */

    // declval (c++11)
    // declval のヘルパー
    template<typename T>
    struct declval_helper
    {
        static const bool stop = false;
        static typename add_rvalue_reference<T>::type delegate();
    };
    // declval (c++11)
    // 指定された型の値を得る (この関数は評価されてはならない)
    template<typename T>
    typename add_rvalue_reference<T>::type declval() noexcept
    {
        static_assert(declval_helper<T>::stop,
                      "incorrect usage of declval.");
        return declval_helper<T>::delegate();
    }
};