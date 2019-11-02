#include "multiple_precision.hpp"

#include <vector>

int main(int argc, char** argv)
{
    std::cout << "UINT_32: " << sizeof(UINT_32) << std::endl
              << "UINT_64: " << sizeof(UINT_64) << std::endl
              << " INT_64: " << sizeof(INT_64)  << std::endl;

    return 0;
}