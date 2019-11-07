#include "multiple_precision.hpp"

int main(int argc, char** argv)
{
    MP a(0);
    MP b("984987897219802109");
    MP c("980098298024198021");

    MP::print(b - c);

    return 0;
}