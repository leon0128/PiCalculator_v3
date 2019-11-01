#include "pointer_traits.hpp"
#include "allocator_traits.hpp"

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
    using allocator_type = std::allocator<long double>;

    allocator_type alloc;

    long double temp = 0.0;

    std::cout << "max_size: ";
    LEON::allocator_traits<allocator_type>::construct<long double>(alloc, &temp, 1.0);
    std::cout << temp << std::endl;

    
    return 0;
}