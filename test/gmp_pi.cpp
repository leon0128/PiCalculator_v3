#include <gmpxx.h>
#include <cmath>
#include <string>
#include <iostream>

mpz_class A("13591409");
mpz_class B("545140134");
mpz_class C("640320");
mpz_class C3over24 = C * C * C / 24;

mpf_class PI;

struct PQT
{
    mpz_class P;
    mpz_class Q;
    mpz_class T;
};

PQT compute(int n1, int n2);

int main(int argc, char** argv)
{
    int digit = 10000;
    if(argc >= 2)
        digit = atoi(argv[1]);

    int count = digit / 14;

    PI = mpf_class(0, digit * log2(10));

    PQT pqt = compute(0, count);

    PI  = sqrt(mpf_class(C));
    PI *= C * pqt.Q;
    PI /= (pqt.T + A * pqt.Q) * 12;

    std::cout.precision(digit + 1);
    std::cout << PI 
              << " ["
              << digit << "]" << std::endl;

    return 0;
}

PQT compute(int n1, int n2)
{
    PQT ans;

    if(n1 + 1 == n2)
    {
        ans.P  = -(2 * n2 - 1);
        ans.P *=  (6 * n2 - 5);
        ans.P *=  (6 * n2 - 1);

        ans.Q  = C3over24 * n2 * n2 * n2;
        
        ans.T  = (A + B * n2) * ans.P;
    }
    else
    {
        int m = (n1 + n2) / 2;

        PQT pqt1 = compute(n1, m);
        PQT pqt2 = compute(m, n2);

        ans.P = pqt1.P * pqt2.P;
        ans.Q = pqt1.Q * pqt2.Q;
        ans.T = pqt1.T * pqt2.Q +
                pqt1.P * pqt2.T;
    }

    return ans;
}
