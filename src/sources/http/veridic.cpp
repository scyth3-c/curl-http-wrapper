#include "http/veridic.hpp"

Veridic::Veridic() {}
Veridic::Veridic(string tasty_url) : URL(tasty_url) {}

bool Veridic::setUrl(string url) {
    if (!url.empty()) {
        URL = url;
        return true;
    }
        return false;
}

string Veridic::get(string endpoint)
{
    if (URL.empty())
        return BAD_RESULT;
    http = make_shared<HTTP>(URL);

    if (http->getSimple(endpoint != DEFAULT ? endpoint : "") != HTTP_SUCCESS)
        return BAD_RESULT;

    auto response = http->Response();
    http.reset();
    return response;
}

string Veridic::get(GET &fields, string endpoint)
{
    if (URL.empty())
        return BAD_RESULT;
    http = make_shared<HTTP>(URL);

    if (http->getBase(&fields, endpoint != DEFAULT ? endpoint : "") != HTTP_SUCCESS)
        return BAD_RESULT;

    auto response = http->Response();
    http.reset();
    return response;
}

string Veridic::get(GET &fields, Headers& headers, string endpoint)
{
    if (URL.empty())
        return BAD_RESULT;
    http = make_shared<HTTP>(URL);

    if (http->get(fields, headers, endpoint != DEFAULT ? endpoint : "") != HTTP_SUCCESS)
        return BAD_RESULT;

    auto response = http->Response();
    http.reset();
    return response;
}

string Veridic::get(Headers& headers, string endpoint)
{
    if (URL.empty())
        return BAD_RESULT;
    http = make_shared<HTTP>(URL);

    if (http->getHeaders(headers, endpoint != DEFAULT ? endpoint : "") != HTTP_SUCCESS)
    return BAD_RESULT;

    auto response = http->Response();
    http.reset();
    return response;
}


string Veridic::post(POST &fields, string endpoint, string type)
{

    if (URL.empty())
        return BAD_RESULT;
    http = make_shared<HTTP>(URL);

    if (http->post(fields, endpoint != DEFAULT ? endpoint : "", type != "" ? type : "" ) != HTTP_SUCCESS)
        return BAD_RESULT;

    auto response = http->Response();
    http.reset();
    return response;
}

string Veridic::post(POST &fields, Headers &headers, string endpoint, string type)
{

    if (URL.empty())
        return BAD_RESULT;
    if (!headers.initial)
        return BAD_RESULT;

    http = make_shared<HTTP>(URL);

    if (http->post(&fields, headers, endpoint != DEFAULT ? endpoint : "", type != "" ? type : "" ) != HTTP_SUCCESS)
        return BAD_RESULT;

    auto response = http->Response();
    http.reset();
    return response;
}

string Veridic::post(std::string endpoint) {
    if (URL.empty())
        return BAD_RESULT;

    http = make_shared<HTTP>(URL);

    if (http->postSimple(endpoint != DEFAULT ? endpoint : "") != HTTP_SUCCESS)
        return BAD_RESULT;

    auto response = http->Response();
    http.reset();
    return response;
}

string Veridic::post(Headers &headers, std::string endpoint) {
    if (URL.empty())
        return BAD_RESULT;
    if (!headers.initial)
        return BAD_RESULT;

    http = make_shared<HTTP>(URL);

    if (http->postHeaders(headers, endpoint != DEFAULT ? endpoint : "") != HTTP_SUCCESS)
        return BAD_RESULT;

    auto response = http->Response();
    http.reset();
    return response;
}

string Veridic::put(PUT &_fields, string _endpoint) {
 return _endpoint != DEFAULT ? post(_fields, _endpoint, PUT_TYPE) :post(_fields, PUT_TYPE) ;
}

string Veridic::put(PUT &_fields, Headers &_headers, string _endpoint) {
    return _endpoint != DEFAULT ? post(_fields,_headers, _endpoint, PUT_TYPE) :post(_fields, _headers, PUT_TYPE) ;
}
string Veridic::Delete(DELETE &_fields, string _endpoint) {
    return _endpoint != DEFAULT ? post(_fields, _endpoint, DELETE_TYPE) : post(_fields, DELETE_TYPE);
}
string Veridic::Delete(DELETE &_fields, Headers  &_headers, string _endpoint) {
    return _endpoint != DEFAULT ? post(_fields, _headers, _endpoint, DELETE_TYPE) : post(_fields, _headers, DELETE_TYPE);
}
string Veridic::custom(POST &_fields, string type, string _endpoint) {
    return _endpoint != DEFAULT ?post(_fields, _endpoint, type) : post(_fields, type);
}
string Veridic::custom(POST &_fields, Headers &_headers, string type, string _endpoint) {
    return _endpoint != DEFAULT ?post(_fields, _headers , _endpoint, type) : post(_fields, type);
}