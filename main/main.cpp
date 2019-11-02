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
    Token& operator=(const Token&){std::cout << "=C" << std::endl; return *this;}
    Token& operator=(Token&&){std::cout << "=M" << std::endl; return *this;}
};

int main(int argc, char** argv)
{
    LEON::vector<Token> vec(3);
    std::vector<Token> vec2(1);
    // vec.reserve(3);

    std::cout << "vec : " << vec.data() << std::endl;
    std::cout << "vec2: " << vec2.data() << std::endl;

    std::initializer_list<Token> a{};

    Token tok;
    std::cout << "===" << std::endl;

    vec.assign(a);
    
    std::cout << "===" << std::endl;

    std::cout << "vec : " << vec.capacity() << std::endl;
    std::cout << "vec2: " << vec2.capacity() << std::endl;

    // for(; iter != vec.end(); iter++)
    //     std::cout << "value: " << *iter << std::endl;

    std::cout << "size: " << vec.size() << std::endl;

    return 0;
}