#include "calculator.hpp"

void Calculator::calculate(MP& mp, EAlgorithm algorithm)
{
    switch(algorithm)
    {
        case(RAMANUJAN):
            ramanujan(mp);
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

    for(UINT_64 i = 0; i < 2; i++)
    {
        MP temp
            = MP::power(MP::power(_4, i) * MP::factorial(i),
                        4);

        MP dividend
            = _4                         *
              MP::power(_882, 2 * i + 1) *
              temp;

        MP divisor
            = MP::power(_M_1, i)   *
              MP::factorial(4 * i) *
              (_1123 + _21460 * MP(i));

        MP res = dividend / divisor;
        // mp += 
    }
}