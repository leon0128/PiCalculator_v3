#pragma once

// 具体的な値は実装依存
// 規定で定められている値を使用

/*
* 最小値 最大値
*/

// SCHAR_MIN, SCHAR_MAX
// signed char の最小値、最大値
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127

// UCHAR_MAX
// unsigned char の最大値
#define UCHAR_MAX 255

// CHAR_MIN, CHAR_MAX
// char の最小値、最大値
#ifdef __CHAR_UNSIGNED__
    #define CHAR_MIN 0
    #define CHAR_MAX 255
#else
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127
#endif

// SHRT_MIN, SHRT_MAX
// short の最小値、最大値
#define SHRT_MIN (-32768)
#define SHRT_MAX 32767

// USHRT_MAX
// unsigned short の最大値
#define USHRT_MAX 65535

// INT_MIN, INT_MAX
// int の最小値、最大値
#define INT_MIN (-2147483647 - 1)
#define INT_MAX 2147483647

// UINT_MAX
// unsigned int の最大値
#define UINT_MAX 4294967295u

// LONG_MIN, LONG_MAX, ULONG_MAX
// long, unsigned long の最小値、最大値
#if defined __x86_64__
    #define  LONG_MIN (-9223372036854775807l - 1l)
    #define  LONG_MAX 9223372036854775807l
    #define ULONG_MAX 18446744073709551615ul
#else
    #define  LONG_MIN (-2147483648l)
    #define  LONG_MAX 2147483647l
    #define ULONG_MAX 4294967295ul
#endif

// LLONG_MIN, LLONG_MAX (c++11)
// long long の最小値、最大値
#define LLONG_MIN (-9223372036854775807ll - 1l)
#define LLONG_MAX 9223372036854775807ll 

// ULLONG_MAX (c++11)
// unsigned long long の最大値
#define ULLONG_MAX 18446744073709551615ull

/*
* その他
*/

// CHAR_BIT
// 1 バイトのビット数
#define CHAR_BIT 8

// MB_LEN_MAX
// サポートされている全てのロケールのマルチバイト文字のうち、最大のバイト数
#define MB_LEN_MAX 16