#include "vector.hpp"

#include <typeinfo>
#include <iostream>
#include <memory>
#include <vector>

class Token
{
public:
    using value_type = int;
    using pointer = void*;
    using difference_type = long;

};

int main(int argc, char** argv)
{
    LEON::vector<int> vec;

    return 0;
}