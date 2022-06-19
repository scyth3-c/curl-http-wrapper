#include <iostream>
#include "http/veridic.hpp"

using  std::cout;

int main(int argc, char const *argv[])
{
    Veridic tasty;
    tasty.setUrl("http://example.com/");  //example url, this url not include endpoints etc


    //example fields
    POST fields = {
        "name=cookies",
        "id=11"
    };

    //example headers
    Headers headers = {
        "location: colombia",
        "deeper: false"
    };

    //example put
    auto res = tasty.put(fields, headers, "/user/verify");
    auto res = tasty.put(fields, headers);
    auto res2 = tasty.put(fields, "/user/verify");
    auto res3 = tasty.put(fields);

    cout << res; 

    return 0;
}
