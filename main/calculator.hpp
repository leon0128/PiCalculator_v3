#pragma once

#include "multiple_precision.hpp"

#include <iostream>

class Calculator
{
private:
    Calculator() = delete;

public:
    static void calculate(MP& mp, UINT_64 digit = 100);
};