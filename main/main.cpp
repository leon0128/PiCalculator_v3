#include "multiple_precision.hpp"
#include "calculator.hpp"

int main(int argc, char** argv)
{
    MP mp;

    Calculator::calculate(mp);

    MP::print(mp);

    return 0;
}