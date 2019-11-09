#pragma once

#include "multiple_precision.hpp"

#include <iostream> // cout, endl, cerr, flush

class Calculator
{
private:
    Calculator() = delete;

public:
    enum EAlgorithm
    {
        CHUDNOVSKY = 0,
        RAMANUJAN  = 1
    };

    static void calculate(MP& mp, UINT_64 digit = 100,
                          EAlgorithm algorithm = CHUDNOVSKY);

private:
    // アルゴリズム
    static void chudnovsky(MP& mp);    
    static void ramanujan(MP& mp);

};