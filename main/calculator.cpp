#include "calculator.hpp"

void Calculator::calculate(MP& mp, UINT_64 digit, EAlgorithm algorithm)
{
    MP::MAX_DEPTH = digit; 

    switch(algorithm)
    {
        case(CHUDNOVSKY):
            chudnovsky(mp);
            break;
        case(RAMANUJAN):
            ramanujan(mp);
            break;

        default:
            std::cerr << "The spwcified algorithm does not exist." << std::endl;
            break;
    }
}

void Calculator::chudnovsky(MP& mp)
{
    std::cout << "\r" << "initialize-value" << std::flush;

    UINT_64 count
        = MP::MAX_DEPTH / 14;

    mp = MP(800); // 平方根を計算する際の初期値

    UINT_64 _C = 640320;

    MP A("13591409");
    MP B("545140134");
    MP C(_C);
    MP _C_2(_C / 2);

    MP C3over24(_C * _C * _C / 24);

    MP P(1);
    MP Q(1);
    MP T(0);

    for(UINT_64 i = 1; i <= count; i++)
    {
        std::cout << "\r" << "multiplication-section: " << i << "/" << count << std::flush;

        MP p(2 * i - 1);
        p *= 6 * i - 5;
        p *= 6 * i - 1;

        MP q(i * i * i);
        q *= C3over24;

        MP a(B * i);
        a += A;
        if(i % 2 == 1)
            a.inverse();
        
        P *= p;
        Q *= q;
        T *= q;
        a *= P;
        T += a;
    }
    std::cout << "\r" << "                                    " << std::flush;
    std::cout << "\r" << "root-section: " << std::flush;

    mp = MP("0.5") * (mp + C / mp);
    mp = MP("0.5") * (mp + C / mp);
    for(UINT_64 i = 14; i < MP::MAX_DEPTH; i *= 2)
    {
        std::cout << "\r" << "root-section: " << i << "/" << MP::MAX_DEPTH << std::flush;
        mp = MP("0.5") * (mp +  C / mp);
    }

    std::cout << "\r" << "                                     " << std::flush;
    std::cout << "\r" << "final-section" << std::flush;

    mp *= C;
    mp *= Q;
    Q  *= A;
    Q  += T;
    Q  *= 12;
    mp /= Q;

    std::cout << "                                          " << "\r" << std::flush;
}

void Calculator::ramanujan(MP& mp)
{
    UINT_64 count
        = (MP::MAX_DEPTH >= 1)
            ? (MP::MAX_DEPTH / 6 + 3) : 0;


    mp = MP();

    MP _f_4n(1);
    MP _21460n(0);
    MP _p_882(882);
    MP _p_4(1);
    MP _f_n(1);

    for(UINT_64 i = 0; i < count; i++)
    {
        std::cout << "\r" <<"prog: " << i + 1 << "/" << count << std::flush;

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
    }
    std::cout << "\r";

    mp = 4 / mp;
}