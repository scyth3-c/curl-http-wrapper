#include <iostream>
#include "http/veridic.hpp"

using  std::cout;

int main(int argc, char const *argv[]) {
    Veridic tasty;

    tasty.setUrl("http://example.com/");

    Headers headers {
            "Authorization: Bearer your_access_token",
            "Cache-Control: no-cache",
            "User-Agent: http-curl/1.0"
    };

    auto res = tasty.get(headers);

    cout << res; 

    return 0;
}
