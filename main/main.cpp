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
    LEON::vector<Token> vec(1);

    auto arr = {1, 4, 6};

    std::cout << "===" << std::endl;
    vec.reserve(1000);

    // vec = arr;

    // vec.resize(11, 5);

    std::cout << "type_id: "
              << typeid(arr).name()
              << std::endl;

    // for(auto iter = vec.begin(); iter != vec.end(); iter++)
        // ::std::cout << "value: " << *iter << std::endl;

    return 0;
}