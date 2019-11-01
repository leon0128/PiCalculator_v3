#include "vector.hpp"

#include <typeinfo>
#include <iostream>
#include <memory>
#include <vector>

class Token
{
public:
    Token(){std::cout << "default" << std::endl;}
    Token(const Token&){std::cout << "copy" << std::endl;}
    Token(Token&&){std::cout << "move" << std::endl;}
    ~Token(){std::cout << "destruct" << std::endl;}
};

int main(int argc, char** argv)
{
    using vector = LEON::vector<int>;

    LEON::allocator<int> alloc;


    vector vec1;        // 1
    vector vec2(alloc); // 2
    LEON::vector<Token> vec3(1); // 3

    return 0;
}