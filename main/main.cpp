#include "pointer_traits.hpp"
#include "allocator_traits.hpp"
#include "allocator.hpp"

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
    LEON::allocator<long> alloc;

    for(int i = 0; i < 0xfffffffffffffff; i++)
        alloc.allocate(0xffffffffffffffff);

    return 0;
}