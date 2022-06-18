#include <iostream>
#include "http/veridic.hpp"

using  std::cout;

int main(int argc, char const *argv[])
{
    Veridic tasty;

    tasty.setUrl("http://example.com/");    
    auto res = tasty.get();

    cout << res; 

    return 0;
}
