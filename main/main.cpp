#include "multiple_precision.hpp"
#include "calculator.hpp"

int main(int argc, char** argv)
{
    // MP mp;

    // Calculator::calculate(mp);

    // MP::print(mp);

    MP a(4);
    MP b("1.5");

    MP::print(a / b);

    return 0;
}