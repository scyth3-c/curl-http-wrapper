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

    Headers headers = {
            "Authorization: Bearer your_access_token",
            "Cache-Control: no-cache",
            "User-Agent: http-curl/1.0"
    };

    auto Combo = tasty.get(fields, "/user/verify");
    auto Fidls = tasty.get(fields);
    auto Simple = tasty.get();

    auto Headers = tasty.get(headers);
    auto FieldHeaders = tasty.get(fields, headers);

    cout << res;

    return 0;
}
