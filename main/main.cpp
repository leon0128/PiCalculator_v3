#include "calculator.hpp"

int main(int argc, char** argv)
{
    MP mp;

    if(argc == 1)
        Calculator::calculate(mp);
    else
        Calculator::calculate(mp, MP::convert(MP(argv[1])));

    MP::output(mp);

    return 0;
}