#include "multiple_precision.hpp"

#include <vector>

int main(int argc, char** argv)
{
    MP a;
    MP b(100000000);

    for(int i = 0; i < 100000000; i++)
    {
        if(i % 10000000000 == 0)
            std::cout << "proc: " << i << std::endl;
        a+=b;
    }
    MP::print(a);



    return 0;
}