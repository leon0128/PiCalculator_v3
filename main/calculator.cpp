#include "calculator.hpp"

void Calculator::calculate(MP& mp, EAlgorithm algorithm)
{
    switch(algorithm)
    {
        case(RAMANUJAN):
            ramanujan(mp);
            break;
        case(LEIBNIZ):
            leibniz(mp);
            break;

        default:
            std::cerr << "err: algorithm is not implemented." << std::endl;
    }
}

void Calculator::ramanujan(MP& mp)
{
    const MP _M_1(-1);

    const MP _4(4);
    const MP _882(882);
    const MP _1123(1123);
    const MP _21460(21460);

    mp = MP();

    for(UINT_64 i = 0; i < 20; i++)
    {
        MP dividend
            = MP::power(_M_1, i) *
              MP::factorial(4 * i) * 
              (_1123 + (_21460 * MP(i)));

        MP divisor
            = MP::power(_882, 2 * i + 1) *
              MP::power(MP::power(_4, i) * MP::factorial(i), 4);

        mp += dividend / divisor;
    }

    mp = _4 / mp;
}

void Calculator::leibniz(MP& mp)
{
    mp = MP();

    const MP _4(4);
    const MP _M_1(-1);
    const MP _1(1);
    const MP _2(2);

    for(UINT_64 i = 0; i < 100; i++)
    {
        MP dividend
            = (i % 2 == 0)
                ? _1 : _M_1;

        MP divisor
            = _2 * MP(i) + _1;
        
        mp += dividend / divisor;
    }

    mp *= _4;
}