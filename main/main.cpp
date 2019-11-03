#include "multiple_precision.hpp"

#include <vector>

int main(int argc, char** argv)
{
    MP a;
    MP b(100);
    a += b;

    MP::print(a += b);



    return 0;
}