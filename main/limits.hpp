/*
* 以下の型の特殊化は後で実装
* char8_t, char16_t, char32_t, wchat_t,
* float, double, long double
*/

#pragma once

#include "climits.hpp"

namespace LEON
{
    // float_round_style
    // 浮動小数点数 の丸めのスタイルに関数列挙値
    enum float_round_style
    {
        round_indeterminate       = -1, // 丸めスタイルが確定しない
        round_toward_zero         = 0,  // 0 に向かって丸められる
        round_to_nearest          = 1,  // 最も近い表現可能な値に丸められる
        round_toward_infinity     = 2,  // 正の無限表現に向かって丸められる
        round_toward_neg_infinity = 3   // 負の無限表現に向かって丸められる
    };

    // float_denorm_style
    // 非正規化数 のサポートに関する情報の為の列挙値
    enum float_denorm_style
    {
        denorm_indeterminate = -1, // 値型が非正規化数を許可するかを判定できない
        denorm_absent        = 0,  // 値型が非正規化数を許可しない
        denorm_present       = 1   // 値型が非正規化数を許可する
    };


    // 特殊化されていないクラスのメンバ変数を定義した基底クラス
    // 必要な時だけ派生クラスで再定義
    class numeric_limits_base
    {
    public:
        // 特殊化されているか
        static constexpr bool is_specialized = false;

        // digits (c++11)
        // 基数 radix において 表示できる桁数を示す
        static constexpr int digits = 0;
        // digits10 (c++11)
        // 10進数において表現できる桁数を示す
        static constexpr int digits10 = 0;
        // max_digits10 (c++11)
        // 浮動小数点数 の10進数表現における正確さを損なわない最大の桁数
        static constexpr int max_digits10 = 0;
        // is_signed (c++11)
        // 型が 符号付き型 なら true そうでなければ false
        static constexpr bool is_signed = false;
        // is_integer (c++11)
        // 型が 整数型 なら true そうでなければ false
        static constexpr bool is_integer = false;
        // is_exact (c++11)
        // 型が 正確な表現 を持つ場合 true そうでなければ false
        static constexpr bool is_exact = false;
        // radix (c++11)
        // digits を表現する基数を示す
        static constexpr int radix = 0;
        // min_exponent (c++11)
        // 浮動小数点数型 において 型の指数下限値を示す
        static constexpr int min_exponent = 0;
        // min_exponent10 (c++11)
        // 浮動小数点数型 において 型の10進指数下限値を示す
        static constexpr int min_exponent10 = 0;
        // max_exponent (c++11)
        // 浮動小数点数型 において 型の指数上限値を示す
        static constexpr int max_exponent = 0;
        // max_exponent10 (c++11)
        // 浮動小数点数型 において 型の10新指数上限値を示す
        static constexpr int max_exponent10 = 0;
        // has_infinity (c++11)
        // 浮動小数点数型 において 型が正の無限表現を持っているか
        static constexpr bool has_infinity = false;
        // has_quiet_NaN (c++11)
        // 浮動小数点数型 において 型がシグナルを投げない NaN を持っているか
        static constexpr bool has_quiet_NaN = false;
        // has_signaling_NaN (c++11)
        // 浮動小数点数型 において 型がシグナルを投げる NaN を持っているか
        static constexpr bool has_signaling_NaN = false;
        // has_denorn (c++11)
        // 浮動小数点数型 において 型の非正規化数のサポート状況を判定
        static constexpr float_denorm_style has_denorm = denorm_absent;
        // has_denorm_loss (c++11)
        // 浮動小数点数型 において 精度の損失が非正規化数によるものかを判定
        static constexpr bool has_denorm_loss = false;
        // is_iec559 (c++11)
        // 浮動小数点数型 において 型が IEC559 (IEEE754) に準拠しているか
        static constexpr bool is_iec559 = false;
        // is_bounded (c++11)
        // 型の値のなす集合が有限かを判定
        static constexpr bool is_bounded = false;
        // is_modulo (c++11)
        // 加算、減算、乗算における数学的な値と、その型の値との間に [max() - min() + 1] を法として
        // 常に合同関係があるか
        static constexpr bool is_modulo = false;
        // traps (c++11)
        // 算術演算によって トラップが発せうする型かどうか
        static constexpr bool traps = false;
        // tinyness_before (c++11)
        // 浮動小数点数型 において 型が丸めが行われる前に小さな値になることを検出できるか
        static constexpr bool tinyness_before = false;
        // round_style (c++11)
        // 浮動小数点数 の丸めスタイルの列挙値
        static constexpr float_round_style round_style = round_toward_zero;
    };

    // 特殊化されていない型
    template<typename T>
    class numeric_limits:
        public numeric_limits_base
    {
    public:
        // min (c++11)
        // 型の最小値取得 (浮動小数点数の場合、非ゼロの正の値の最小値)
        static constexpr T min() noexcept {return T();}
        // max (c++11)
        // 型の最大値取得
        static constexpr T max() noexcept {return T();}
        // lowest (c++11)
        // 型の最小値取得
        static constexpr T lowest() noexcept {return T();}
        // epsilon (c++11)
        // 機械イプシロン を取得する
        static constexpr T epsilon() noexcept {return T();}
        // round_error (c++11)
        // 最大の丸め誤差取得
        static constexpr T round_error() noexcept {return T();}
        // infinity (c++11)
        // 浮動小数点数型 における 正の無限表現を取得
        static constexpr T infinity() noexcept {return T();}
        // quiet_NaN (c++11)
        // 浮動小数点数型 において シグナルを発生させない NaN を取得
        static constexpr T quiet_NaN() noexcept {return T();}
        // signaling_NaN (c++11)
        // 浮動小数点数型 において シグナルを発生させる NaN を取得
        static constexpr T signaling_NaN() noexcept {return T();}
        // denorm_min (c++11)
        // 浮動小数点数型 において 最小の正の非正規化数を取得
        static constexpr T denorm_min() noexcept {return T();}
    };

    // cv 修飾 を許容するための特殊化
    template<typename T>
    class numeric_limits<const T>:
        public numeric_limits<T>{};
    template<typename T>
    class numeric_limits<volatile T>:
        public numeric_limits<T>{};
    template<typename T>
    class numeric_limits<const volatile T>:
        public numeric_limits<T>{};

    // bool 特殊化
    template<>
    class numeric_limits<bool>:
        public numeric_limits_base
    {
    public:
        static constexpr bool is_specialized = true;

        static constexpr int  digits     = 1;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact   = true;
        static constexpr int  radix      = 2;
        static constexpr bool is_bounded = true;
        static constexpr bool traps      = true;

        static constexpr bool min()           noexcept {return false;}
        static constexpr bool max()           noexcept {return true;}
        static constexpr bool lowest()        noexcept {return false;}
        static constexpr bool epsilon()       noexcept {return false;}
        static constexpr bool round_error()   noexcept {return false;}
        static constexpr bool infinity()      noexcept {return false;}
        static constexpr bool quiet_NaN()     noexcept {return false;}
        static constexpr bool signaling_NaN() noexcept {return false;}
        static constexpr bool denorm_min()    noexcept {return false;}
    };

    // char 特殊化
    template<>
    class numeric_limits<char>:
        public numeric_limits_base
    {
    public:
        static constexpr bool is_specialized = true;

        static constexpr int  digits     = (CHAR_MAX != UCHAR_MAX) ? 7 : 8;
        static constexpr int  digits10   = 2;
        static constexpr bool is_signed  = (CHAR_MAX != UCHAR_MAX) ? true : false;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact   = true;
        static constexpr int  radix      = 2;
        static constexpr bool is_bounded = true;
        static constexpr bool traps      = true;

        static constexpr char min()           noexcept {return CHAR_MIN;}
        static constexpr char max()           noexcept {return CHAR_MAX;}
        static constexpr char lowest()        noexcept {return CHAR_MIN;}
        static constexpr char epsilon()       noexcept {return 0;}
        static constexpr char round_error()   noexcept {return 0;}
        static constexpr char infinity()      noexcept {return static_cast<char>(0);}
        static constexpr char quiet_NaN()     noexcept {return static_cast<char>(0);}
        static constexpr char signaling_NaN() noexcept {return static_cast<char>(0);}
        static constexpr char denorm_min()    noexcept {return static_cast<char>(0);}
    };

    // signed char の特殊化
    template<>
    class numeric_limits<signed char>:
        public numeric_limits_base
    {
    public:
        static constexpr bool is_specialized = true;

        static constexpr int  digits     = 7;
        static constexpr int  digits10   = 2;
        static constexpr bool is_signed  = true;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact   = true;
        static constexpr int  radix      = 2;
        static constexpr bool is_bounded = true;
        static constexpr bool traps      = true;

        static constexpr signed char min()           noexcept {return SCHAR_MIN;}
        static constexpr signed char max()           noexcept {return SCHAR_MAX;}
        static constexpr signed char lowest()        noexcept {return SCHAR_MIN;}
        static constexpr signed char epsilon()       noexcept {return 0;}
        static constexpr signed char round_error()   noexcept {return 0;}
        static constexpr signed char infinity()      noexcept {return static_cast<signed char>(0);}
        static constexpr signed char quiet_NaN()     noexcept {return static_cast<signed char>(0);}
        static constexpr signed char signaling_NaN() noexcept {return static_cast<signed char>(0);}
        static constexpr signed char denorm_min()    noexcept {return static_cast<signed char>(0);}
    };

    // unsigned char の特殊化
    template<>
    class numeric_limits<unsigned char>:
        public numeric_limits_base
    {
    public:
        static constexpr bool is_specialized = true;

        static constexpr int  digits     = 8;
        static constexpr int  digits10   = 2;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact   = true;
        static constexpr int  radix      = 2;
        static constexpr bool is_bounded = true;
        static constexpr bool traps      = true;
        
        static constexpr unsigned char min()           noexcept {return 0;}
        static constexpr unsigned char max()           noexcept {return UCHAR_MAX;}
        static constexpr unsigned char lowest()        noexcept {return 0;}
        static constexpr unsigned char epsilon()       noexcept {return 0;}
        static constexpr unsigned char round_error()   noexcept {return 0;}
        static constexpr unsigned char infinity()      noexcept {return static_cast<unsigned char>(0);}
        static constexpr unsigned char quiet_NaN()     noexcept {return static_cast<unsigned char>(0);}
        static constexpr unsigned char signaling_NaN() noexcept {return static_cast<unsigned char>(0);}
        static constexpr unsigned char denorm_min()    noexcept {return static_cast<unsigned char>(0);}
    };

    // short の特殊化
    template<>
    class numeric_limits<short>:
        public numeric_limits_base
    {
    public:
        static constexpr bool is_specialized = true;

        static constexpr int  digits     = 15;
        static constexpr int  digits10   = 4;
        static constexpr bool is_signed  = true;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact   = true;
        static constexpr int  radix      = 2;
        static constexpr bool is_bounded = true;
        static constexpr bool traps      = true;

        static constexpr short min()           noexcept {return SHRT_MIN;}
        static constexpr short max()           noexcept {return SHRT_MAX;}
        static constexpr short lowest()        noexcept {return SHRT_MIN;}
        static constexpr short epsilon()       noexcept {return 0;}
        static constexpr short round_error()   noexcept {return 0;}
        static constexpr short infinity()      noexcept {return static_cast<short>(0);}
        static constexpr short quiet_NaN()     noexcept {return static_cast<short>(0);}
        static constexpr short signaling_NaN() noexcept {return static_cast<short>(0);}
        static constexpr short denorm_min()    noexcept {return static_cast<short>(0);}
    };

    // unsigned short の特殊化
    template<>
    class numeric_limits<unsigned short>:
        public numeric_limits_base
    {
    public:
        static constexpr bool is_specialized = true;

        static constexpr int  digits     = 16;
        static constexpr int  digits10   = 4;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact   = true;
        static constexpr int  radix      = 2;
        static constexpr bool is_bounded = true;
        static constexpr bool traps      = true;

        static constexpr unsigned short min()           noexcept {return 0;}
        static constexpr unsigned short max()           noexcept {return USHRT_MAX;}
        static constexpr unsigned short lowest()        noexcept {return 0;}
        static constexpr unsigned short epsilon()       noexcept {return 0;}
        static constexpr unsigned short round_error()   noexcept {return 0;}
        static constexpr unsigned short infinity()      noexcept {return static_cast<unsigned short>(0);}
        static constexpr unsigned short quiet_NaN()     noexcept {return static_cast<unsigned short>(0);}
        static constexpr unsigned short signaling_NaN() noexcept {return static_cast<unsigned short>(0);}
        static constexpr unsigned short denorm_min()    noexcept {return static_cast<unsigned short>(0);}
    };

    // int の特殊化
    template<>
    class numeric_limits<int>:
        public numeric_limits_base
    {
    public:
        static constexpr bool is_specialized = true;

        static constexpr int  digits     = 31;
        static constexpr int  digits10   = 9;
        static constexpr bool is_signed  = true;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact   = true;
        static constexpr int  radix      = 2;
        static constexpr bool is_bounded = true;
        static constexpr bool traps      = true;

        static constexpr int min()           noexcept {return INT_MIN;}
        static constexpr int max()           noexcept {return INT_MAX;}
        static constexpr int lowest()        noexcept {return INT_MIN;}
        static constexpr int epsilon()       noexcept {return 0;}
        static constexpr int round_error()   noexcept {return 0;}
        static constexpr int infinity()      noexcept {return static_cast<int>(0);}
        static constexpr int quiet_NaN()     noexcept {return static_cast<int>(0);}
        static constexpr int signaling_NaN() noexcept {return static_cast<int>(0);}
        static constexpr int denorm_min()    noexcept {return static_cast<int>(0);}
    };

    // unsigned int の特殊化
    template<>
    class numeric_limits<unsigned int>:
        public numeric_limits_base
    {
    public:
        static constexpr bool is_specialized = true;

        static constexpr int  digits     = 32;
        static constexpr int  digits10   = 9;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact   = true;
        static constexpr int  radix      = 2;
        static constexpr bool is_bounded = true;
        static constexpr bool traps      = true;

        static constexpr unsigned int min()           noexcept {return 0u;}
        static constexpr unsigned int max()           noexcept {return UINT_MAX;}
        static constexpr unsigned int lowest()        noexcept {return 0u;}
        static constexpr unsigned int epsilon()       noexcept {return 0u;}
        static constexpr unsigned int round_error()   noexcept {return 0u;}
        static constexpr unsigned int infinity()      noexcept {return static_cast<unsigned int>(0);}
        static constexpr unsigned int quiet_NaN()     noexcept {return static_cast<unsigned int>(0);}
        static constexpr unsigned int signaling_NaN() noexcept {return static_cast<unsigned int>(0);}
        static constexpr unsigned int denorm_min()    noexcept {return static_cast<unsigned int>(0);}
    };

    // long の特殊化
    template<>
    class numeric_limits<long>:
        public numeric_limits_base
    {
    public:
        static constexpr bool is_specialized = true;

        static constexpr int  digits     = (INT_MAX != LONG_MAX) ? 63 : 31;
        static constexpr int  digits10   = (INT_MAX != LONG_MAX) ? 18 : 9;
        static constexpr bool is_signed  = true;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact   = true;
        static constexpr int  radix      = 2;
        static constexpr bool is_bounded = true;
        static constexpr bool traps      = true;

        static constexpr long min()           noexcept {return LONG_MIN;}
        static constexpr long max()           noexcept {return LONG_MAX;}
        static constexpr long lowest()        noexcept {return LONG_MIN;}
        static constexpr long epsilon()       noexcept {return 0l;}
        static constexpr long round_error()   noexcept {return 0l;}
        static constexpr long infinity()      noexcept {return static_cast<long>(0);}
        static constexpr long quiet_NaN()     noexcept {return static_cast<long>(0);}
        static constexpr long signaling_NaN() noexcept {return static_cast<long>(0);}
        static constexpr long denorm_min()    noexcept {return static_cast<long>(0);}
    };

    // unsigned long の特殊化
    template<>
    class numeric_limits<unsigned long>:
        public numeric_limits_base
    {
    public:
        static constexpr bool is_specialized = true;

        static constexpr int  digits     = (INT_MAX != LONG_MAX) ? 64 : 32;
        static constexpr int  digits10   = (INT_MAX != LONG_MAX) ? 19 : 9;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact   = true;
        static constexpr int  radix      = 2;
        static constexpr bool is_bounded = true;
        static constexpr bool traps      = true;

        static constexpr unsigned long min()           noexcept {return 0ul;}
        static constexpr unsigned long max()           noexcept {return ULONG_MAX;}
        static constexpr unsigned long lowest()        noexcept {return 0ul;}
        static constexpr unsigned long epsilon()       noexcept {return 0ul;}
        static constexpr unsigned long round_error()   noexcept {return 0ul;}
        static constexpr unsigned long infinity()      noexcept {return static_cast<unsigned long>(0);}
        static constexpr unsigned long quiet_NaN()     noexcept {return static_cast<unsigned long>(0);}
        static constexpr unsigned long signaling_NaN() noexcept {return static_cast<unsigned long>(0);}
        static constexpr unsigned long denorm_min()    noexcept {return static_cast<unsigned long>(0);}
    };

    // long long の特殊化
    template<>
    class numeric_limits<long long>:
        public numeric_limits_base
    {
    public:
        static constexpr bool is_specialized = true;

        static constexpr int  digits     = 63;
        static constexpr int  digits10   = 18;
        static constexpr bool is_signed  = true;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact   = true;
        static constexpr int  radix      = 2;
        static constexpr bool is_bounded = true;
        static constexpr bool traps      = true;

        static constexpr long long min()           noexcept {return LLONG_MIN;}
        static constexpr long long max()           noexcept {return LLONG_MAX;}
        static constexpr long long lowest()        noexcept {return LLONG_MIN;}
        static constexpr long long epsilon()       noexcept {return 0ll;}
        static constexpr long long round_error()   noexcept {return 0ll;}
        static constexpr long long infinity()      noexcept {return static_cast<long long>(0);}
        static constexpr long long quiet_NaN()     noexcept {return static_cast<long long>(0);}
        static constexpr long long signaling_NaN() noexcept {return static_cast<long long>(0);}
        static constexpr long long denorm_min()    noexcept {return static_cast<long long>(0);}
    };

    // unsigned long long の特殊化
    template<>
    class numeric_limits<unsigned long long>:
        public numeric_limits_base
    {
    public:
        static constexpr bool is_specialized = true;

        static constexpr int  digits     = 64;
        static constexpr int  digits10   = 19;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact   = true;
        static constexpr int  radix      = 2;
        static constexpr bool is_bounded = true;
        static constexpr bool traps      = true;

        static constexpr unsigned long long min()           noexcept {return 0ull;}
        static constexpr unsigned long long max()           noexcept {return ULLONG_MAX;}
        static constexpr unsigned long long lowest()        noexcept {return 0ull;}
        static constexpr unsigned long long epsilon()       noexcept {return 0ull;}
        static constexpr unsigned long long round_error()   noexcept {return 0ull;}
        static constexpr unsigned long long infinity()      noexcept {return static_cast<unsigned long long>(0);}
        static constexpr unsigned long long quiet_NaN()     noexcept {return static_cast<unsigned long long>(0);}
        static constexpr unsigned long long signaling_NaN() noexcept {return static_cast<unsigned long long>(0);}
        static constexpr unsigned long long denorm_min()    noexcept {return static_cast<unsigned long long>(0);}
    };
};