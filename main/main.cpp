#include "multiple_precision.hpp"

int main(int argc, char** argv)
{
    MP a(-MP::INT_64_MAX);
    MP b("1298512908432098412095120984210239874520935829385721395230998234109841230-9823098234.000 000 000  000 000 000  000 000 000  000 000 000  1");

    MP c = b - b;

    MP::print(c);

    return 0;
}