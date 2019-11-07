#pragma once

#include "multiple_precision.hpp"

#include <iostream>

class Calculator
{
private:
    Calculator() = delete;

public:
    enum EAlgorithm
    {
        RAMANUJAN,
        LEIBNIZ
    };

    static void calculate(MP& mp, EAlgorithm = RAMANUJAN);

private:
    static void ramanujan(MP& mp);
    static void leibniz(MP& mp);
};