#include <iostream>
#include "http/veridic.hpp"

using  std::cout;

int main(int argc, char const *argv[])
{
    Veridic tasty;

    tasty.setUrl("http://example.com/");    

    GET fields = {
        "name=kevin",
        "mail=kevinlevin@gmail.com",
        "tasty=true"
    };

    auto res = tasty.get(fields, "/user/verify");
    auto res2 = tasty.get(fields);
    auto res3 = tasty.get();

    cout << res; 

    return 0;
}
