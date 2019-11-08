#pragma once

#include "vector.hpp"

#include <stdexcept> // error
#include <iostream>  // cout, endl, cerr

class MultiplePrecision;
using MP = MultiplePrecision;
using UINT_32 = unsigned int;
using UINT_64 = unsigned long long int;
using  INT_64 = long long int;

class MultiplePrecision
{
private:
    static const UINT_64 CARRY; // 繰り上がりのボーダー

public:
    static       UINT_64 MAX_DEPTH;  // 小数の最大深度(桁数)
    static const  INT_64 INT_64_MAX; // UINT_64 の最大値()

    // 特殊関数
    MultiplePrecision();
    MultiplePrecision(INT_64 value);
    MultiplePrecision(UINT_64 value);
    MultiplePrecision(int value);
    MultiplePrecision(long value);
    MultiplePrecision(const char* value);
    MultiplePrecision(const MP& other);
    MultiplePrecision(MP&& other);
    ~MultiplePrecision();
    MP& operator=(const MP& other);
    MP& operator=(MP&& other);

    // 演算子
    MP& operator+=(const MP& other);
    MP& operator-=(const MP& other);
    MP& operator*=(const MP& other);
    MP& operator/=(const MP& other);
    
    friend MP operator+(const MP& lhs,
                        const MP& rhs);
    friend MP operator-(const MP& lhs,
                        const MP& rhs);
    friend MP operator*(const MP& lhs,
                        const MP& rhs);
    friend MP operator/(const MP& lhs,
                        const MP& rhs);
    
    friend bool operator==(const MP& lhs,
                           const MP& rhs);
    friend bool operator!=(const MP& lhs,
                           const MP& rhs);
    friend bool operator< (const MP& lhs,
                           const MP& rhs);
    friend bool operator<=(const MP& lhs,
                           const MP& rhs);
    friend bool operator> (const MP& lhs,
                           const MP& rhs);
    friend bool operator>=(const MP& lhs,
                           const MP& rhs);

    // 階乗
    static MP factorial(INT_64 count);
    // 累乗(指数は正の正数のみ)
    static MP power(const MP& radix, UINT_64 index);
    // 絶対値で大きい方を返す
    static const MP& absoluteMax(const MP& lhs, const MP& rhs);
    static       MP& absoluteMax(      MP& lhs,       MP& rhs);
    // UINT_64 を取得する
    static UINT_64 convert(const MP& mp);

    // 出力
    static void output(const MP& mp);

private:
    // 第一引数に第二引数と第三引数の加算結果を格納
    // 演算結果の符号は lhs の値が使用される
    static void addition(MP& dst,
                         const MP& lhs, const MP& rhs);
    // 第一引数に第二引数と第三引数の減算結果を格納
    // 演算結果の符号は |lhs| > |rhs| なら lhs の値が使用される
    static void subtraction(MP& dst,
                            const MP& lhs, const MP& rhs);
    // 第一引数に第二引数と第三引数の乗算結果を格納
    static void multiplication(MP& dst,
                               const MP& lhs, const MP& rhs);
    // 第一引数に第二引数と第三引数の除算結果を格納
    static void division(MP& dst,
                         const MP& lhs, const MP& rhs);
    // 除算で使用する桁合わせ用のヘルパー関数
    static void digitAlignment(  INT_64& digit, 
                                UINT_64& dividend,
                               const MP& difference);

    // 最上位が 0 の場合、その要素を削除
    // 最下位が 0 の場合、その要素の削除
    void shrink();
    // 数値の開始地点の取得 (1.0 == 0, 0.1 = -1)
    INT_64 offset() const; 

    // output の関数
    static void simpleOutput (const MP& mp);

    LEON::vector<UINT_32> mIntegerPart; // 整数部(1 つの要素に 9桁, 10進)
    LEON::vector<UINT_32> mDecimalPart; // 小数部(1 つの要素に 9桁, 10進)

    bool mIsPositive; // 正数かどうか
};