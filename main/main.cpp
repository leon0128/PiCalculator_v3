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
    LEON::vector<int> vec;
    // vec.reserve(3);

    Token tok;
    std::cout << "===" << std::endl;

    auto iter = vec.insert(vec.begin(), 5, 5);
    vec.insert(vec.begin(), {1, 4, 6, 8, 0});

    std::cout << "===" << std::endl;

    for(; iter != vec.end(); iter++)
        std::cout << "value: " << *iter << std::endl;

    std::cout << "size: " << vec.size() << std::endl;

    return 0;
}