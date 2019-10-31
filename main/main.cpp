#include "pointer_traits.hpp"
#include "allocator_traits.hpp"

#include <typeinfo>
#include <iostream>
#include <memory>
#include <vector>

int main(int argc, char** argv)
{
    using allocator_type = std::allocator<int>;

    std::cout << "leon_type: "
              << typeid(typename LEON::allocator_traits<allocator_type>::size_type).name()
              << std::endl;

    std::cout << "std_type : "
              << typeid(typename std::allocator_traits<allocator_type>::size_type).name()
              << std::endl;

    return 0;
}