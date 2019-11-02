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
    using vector = LEON::vector<Token>;

    LEON::allocator<Token> alloc;

    vector src(1);
    vector dst(src);

    // Token tok;
    // LEON::vector<Token> vec(1, tok);

    // vector vec1;        // 1
    // vector vec2(alloc); // 2
    // vector vec3(1);     // 3
    // vector vec4(1, 1);  // 4
    // vector vec5(vec.begin(), vec.end());

    return 0;
}