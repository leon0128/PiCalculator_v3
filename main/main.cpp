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
    LEON::vector<float> vec{1, 4, 5, 7, 9, 1, 4, 4};

    auto arr = {1, 4, 6};

    // vec = arr;

    std::cout << "type_id: "
              << typeid(arr).name()
              << std::endl;

    for(auto iter = vec.begin(); iter != vec.end(); iter++)
        ::std::cout << "value: " << *iter << std::endl;

    return 0;
}