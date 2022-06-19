#include <iostream>
#include "http/veridic.hpp"

using  std::cout;

int main(int argc, char const *argv[])
{
    Veridic tasty;
    tasty.setUrl("http://example.com/");  //example url, this url not include endpoints etc


    //example fields
    POST fields = {
        "choes-Size=3",
        "cookies-size=4",
        "location=polo_norte",
        "isHuman=false"
    };

    //example headers
    Headers headers = {
        "cookies-size: 4",
        "isHuman: false",
        "location: colombia",
        "deeper: false"
    };

    //example post
    auto res = tasty.post(fields, headers, "/user/verify");
    auto res = tasty.post(fields, headers);
    auto res2 = tasty.pos(fields, "/user/verify");
    auto res3 = tasty.pos(fields);

    cout << res; 

    return 0;
}
