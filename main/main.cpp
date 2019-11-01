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
    using allocator_type = Token;

    std::cout << "leon_type: "
              << typeid(typename LEON::allocator_traits<allocator_type>::pointer).name()
              << std::endl;

    std::cout << "std_type : "
              << typeid(typename std::allocator_traits<allocator_type>::pointer).name()
              << std::endl;

    return 0;
}