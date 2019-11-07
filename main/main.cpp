#include "multiple_precision.hpp"

int main(int argc, char** argv)
{
    MP a(0);
    MP b("3");
    MP c("1.5");

    MP::print(b / c);

    return 0;
}