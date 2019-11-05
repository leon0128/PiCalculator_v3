#include "multiple_precision.hpp"

int main(int argc, char** argv)
{
    MP a(-MP::INT_64_MAX);
    MP b("0.000 000 000  000 000 000  000 000 000  000 000 000  1");

    MP::print(b);

    MP::print(a * b);

    return 0;
}