#include "multiple_precision.hpp"
#include <gmp.h>

int main(int argc, char** argv)
{
    MP a(0);
    MP b("0. 000 000 000 5");
    MP c("1 000 000 000");


    MP::print(c - b);
    // MP::print(a -= a * b);

    // mpz_t m;

    // mpz_init(m);
    // mpz_set_str(m, "999999999999999999", 10);

    // for(int i = 0; i < 15; i++)
    // {
    //     // std::cout << "proc: " << i << std::endl;
    //     mpz_mul(m, m, m);
    // }
    // // mpz_out_str(stdout, 10, m);

    // mpz_clear(m);

    return 0;
}