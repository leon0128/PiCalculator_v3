#include "multiple_precision.hpp"
#include "calculator.hpp"

int main(int argc, char** argv)
{
    MP mp;

    Calculator::calculate(mp);

    MP::output(mp);

    return 0;
}