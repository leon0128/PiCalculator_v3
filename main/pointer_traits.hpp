/*
* ::std::pointer_traits を模したもの
* ポインタに対する特殊化は時間があったら実装予定
*/

#pragma once

namespace LEON
{

// SFINAE で型がポインタかどうかを判定
template<typename Ptr>
class pointer_traits
{
private:
    // SFINAE で型特性を取得する関数の作成
    // element_type
    template<typename P>
    static typename P::element_type* get_element_type(typename P::element_type*){return nullptr;}
    template<typename>
    static Ptr get_element_type(...){return nullptr;}
    // difference_type
    template<typename P>
    static typename P::difference_type* get_difference_type(typename P::difference_type*){return nullptr;}
    template<typename>
    static long* get_difference_type(...){return nullptr;}
    // rebind<U>
    template<typename P, typename U>
    static typename P::rebind* get_rebind(typename P::rebind*){return nullptr;}
    template<typename P, typename U>
    static U** get_rebind(...){return nullptr;}

public:
    // メンバ型
    using pointer         = Ptr;
    using element_type    = decltype(*get_element_type<pointer>(nullptr));
    using difference_type = decltype(*get_difference_type<pointer>(nullptr));
    template<typename U>
    using rebind          = decltype(*get_rebind<pointer, U>(nullptr));

    // 静的メンバ関数
    static pointer pointer_to(element_type& v){return pointer::pointer_to(v);}
};

};