#include <iostream>
#include "http/veridic.hpp"

using  std::cout;

int main(int argc, char const *argv[])
{
    Veridic tasty;
    tasty.setUrl("http://example.com/");  //example url, this url not include endpoints etc


    //example fields
    POST fields = {
       "id=my_unique_id"
    };

    //example headers
    Headers headers = {
       "location: colombia",
        "deeper: false"
    };

    //example delete
    auto res = tasty.delete(fields, headers, "/user/verify");
    auto res = tasty.delete(fields, headers);
    auto res2 = tasty.delete(fields, "/user/verify");
    auto res3 = tasty.delete(fields);

    cout << res; 

    return 0;
}