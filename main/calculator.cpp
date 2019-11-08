#include "calculator.hpp"

void Calculator::calculate(MP& mp, UINT_64 digit)
{
    MP::MAX_DEPTH = digit; 
    UINT_64 count
        = (digit >= 1)
            ? (digit / 6 + 3) : 0;


    mp = MP();

    MP _f_4n(1);
    MP _21460n(0);
    MP _p_882(882);
    MP _p_4(1);
    MP _f_n(1);

    for(UINT_64 i = 0; i < count; i++)
    {
        MP dividend
            = (i % 2 == 0)
                ? 1 : -1;
        dividend *= _f_4n;
        dividend *= 1123 + _21460n;

        MP divisor
            = _p_882;
        divisor *= MP::power(_p_4 * _f_n, 4);

        mp += dividend / divisor;

        if(i + 1 < count)
        {
            for(UINT_64 j = 0; j < 4; j++)
                _f_4n *= i * 4 + j + 1;

            _21460n += 21460;
            _p_882  *= 882 * 882;
            _p_4    *= 4;
            _f_n    *= i + 1;
        }

        std::cout << "count: " << i + 1 << "/" << count << std::endl;
    }

    mp = 4 / mp;
}