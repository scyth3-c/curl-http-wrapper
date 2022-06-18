#pragma once

#ifndef VERIDIC_HPP
#define VERIDIC_HPP

#include <iostream>
#include <memory.h>
#include <string>

#include "utils/utils.hpp"
#include "http/http.hpp"

#define PLAIN_TEXT "Content-Type: text/plain"
#define N "\n"
#define NN "\n\n"
#define C std::cout

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::vector;

class Veridic
{

private:
    shared_ptr<HTTP> http = nullptr;
    string URL{};

public:
    Veridic();
    Veridic(string);
    bool setUrl(string);

    string get(string opcional_endpoint = DEFAULT);
    string get(GET &, string opcional_endpostring = DEFAULT);

    string post(POST &, string endpoint = DEFAULT, string type = DEFAULT);
    string post(POST &, Headers &, string endpoint = DEFAULT, string type = DEFAULT);

    string put(PUT &, string endpoint = DEFAULT);
    string put(PUT &, Headers &, string endpoint = DEFAULT);

    string Delete(DELETE &, string endpoint = DEFAULT);
    string Delete(DELETE &, Headers &, string endpoint = DEFAULT);

    string custom(POST &field, string type, string endpoint = DEFAULT);
    string custom(POST &fields, Headers &headers, string type, string endpoint = DEFAULT);
};

#endif // !VERIDIC_HPP