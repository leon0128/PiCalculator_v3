#pragma once

// コンパイラのマクロを利用して処理系定義の実装

namespace LEON
{    
    // ptrdiff_t
    // ポインタの差演算の結果を表す型
    using ptrdiff_t = __PTRDIFF_TYPE__;

    // size_t
    // オブジェクトのバイト数を表す型
    using size_t = __SIZE_TYPE__;

    // nullptr_t (c++11)
    // nullptr の型
    using nullptr_t = decltype(nullptr);

    // max_align_t (c++11)
    // どのスカラー型よりも大きいアライメントを必要とする POD型
    /* 
    * この実装があっているかどうか不明、
    * また、メンバ名に 予約語(ダブルアンダースコア) を使用している為、
    * 予期せぬエラーに繋がる恐れがある
    */ 
    using max_align_t
        = struct{long long   __max_align_ll;
                 long double __max_align_ld;};

    // byte (c++17)
    // byte データの別名として定義される型
    enum class byte : unsigned char{};
};

// offsetof
// 構造体型(type) のメンバ(member) へのオフセットを取得するマクロ
#define offsetof(type, member) __builtin_offsetof(type, member)

// NULL
// ヌルポインタ定数に展開されるマクロ
#define NULL __null