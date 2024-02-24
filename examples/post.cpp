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
    auto justHeaders = tasty.post(headers);

    auto Combo = tasty.post(fields, headers, "/user/verify");
    auto fieldHeaders = tasty.post(fields, headers);

    auto FieldEndpoint = tasty.post(fields, "/user/verify");
    auto Fields = tasty.post(fields);

    auto Simple = tasty.post();
    auto JustEndpoint = tasty.post("/endpoint");

    
    cout << Combo;

    return 0;
}
