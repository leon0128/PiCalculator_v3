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
    using allocator_type = std::allocator<float>;


    allocator_type alloc;

    // std::cout << typeid(typename std::allocator_traits<allocator_type>::size_type).name()
    //           << std::endl;
    // std::cout << typeid(typename LEON::allocator_traits<allocator_type>::size_type).name()
    //           << std::endl;

    // int i = 1;
    std::allocator_traits<allocator_type>::allocate(alloc, 1);
    LEON::allocator_traits<allocator_type>::allocate(alloc, 1);
    // alloc.allocate(1);

    // std::allocator_traits<allocator_type>::allocate

    return 0;
}