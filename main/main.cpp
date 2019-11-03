#include "multiple_precision.hpp"

#include <vector>

int main(int argc, char** argv)
{
    MP(0);
    MP mp(MP::INT_64_MAX);

    MP::print(mp);


    return 0;
}