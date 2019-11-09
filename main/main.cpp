#include "calculator.hpp"

int main(int argc, char** argv)
{
    MP mp;

    if(argc == 1)
        Calculator::calculate(mp);
    else
        Calculator::calculate(mp, MP::convert(MP(argv[1])),
                              Calculator::CHUDNOVSKY);

    MP::output(mp, true);

    return 0;
}