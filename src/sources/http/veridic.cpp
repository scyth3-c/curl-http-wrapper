#include "http/veridic.hpp"

Veridic::Veridic() {}
Veridic::Veridic(string tasty_url) : URL(tasty_url) {}

bool Veridic::setUrl(string url)
{
    if (!url.empty())
    {
        URL = url;
        return true;
    }
    else
    {
        return false;
    }
}

string Veridic::get(string endpoint)
{
    if (URL.empty())
        return BAD_RESULT;
    http = make_shared<HTTP>(URL);

    if (endpoint != DEFAULT)
    {
        if (http->get(endpoint) != HTTP_SUCCESS)
            return BAD_RESULT;
    }
    else
    {
        if (http->get() != HTTP_SUCCESS)
            return BAD_RESULT;
    }
    auto response = http->Response();
    http.reset();
    return response;
}

string Veridic::get(GET &fields, string endpoint)
{
    if (URL.empty())
        return BAD_RESULT;
    http = make_shared<HTTP>(URL);

    if (endpoint != DEFAULT)
    {
        if (http->get(fields, endpoint) != HTTP_SUCCESS)
            return BAD_RESULT;
    }
    else
    {
        if (http->get(fields) != HTTP_SUCCESS)
            return BAD_RESULT;
    }
    auto response = http->Response();
    http.reset();
    return response;
}

string Veridic::post(POST &fields, string endpoint, string type)
{

    if (URL.empty())
        return BAD_RESULT;
    http = make_shared<HTTP>(URL);

    if (endpoint != DEFAULT)
    {
        if (http->post(fields, endpoint) != HTTP_SUCCESS)
            return BAD_RESULT;
    }
    else if (type != DEFAULT && endpoint != DEFAULT)
    {

        if (http->post(fields, endpoint, type) != HTTP_SUCCESS)
            return BAD_RESULT;
    }
    else
    {
        if (http->post(fields) != HTTP_SUCCESS)
            return BAD_RESULT;
    }

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

    if (endpoint != DEFAULT)
    {

        http->post(fields, headers, endpoint);
    }
    else if (type != DEFAULT && endpoint != DEFAULT)
    {

        http->post(fields, headers, endpoint, type);
    }
    else
    {
        http->post(fields, headers);
    }

    auto response = http->Response();
    http.reset();
    return response;
}

string Veridic::put(PUT &_fields, string _endpoint)
{
    if (_endpoint != DEFAULT)
    {
        return post(_fields, _endpoint, PUT_TYPE);
    }
    else
    {
        return post(_fields, PUT_TYPE);
    }
}

string Veridic::put(PUT &_fields, Headers &_headers, string _endpoint)
{
    if (_endpoint != DEFAULT)
    {
        return post(_fields, _headers, _endpoint, PUT_TYPE);
    }
    else
    {
        return post(_fields, _headers, PUT_TYPE);
    }
}

string Veridic::Delete(PUT &_fields, string _endpoint)
{
    if (_endpoint != DEFAULT)
    {
        return post(_fields, _endpoint, PUT_TYPE);
    }
    else
    {
        return post(_fields, PUT_TYPE);
    }
}

string Veridic::Delete(PUT &_fields, Headers &_headers, string _endpoint)
{
    if (_endpoint != DEFAULT)
    {
        return post(_fields, _headers, _endpoint, DELETE_TYPE);
    }
    else
    {
        return post(_fields, _headers, DELETE_TYPE);
    }
}

string Veridic::custom(PUT &_fields, string type, string _endpoint)
{
    if (_endpoint != DEFAULT)
    {
        return post(_fields, _endpoint, type);
    }
    else
    {
        return post(_fields, type);
    }
}

string Veridic::custom(PUT &_fields, Headers &_headers, string type, string _endpoint)
{
    if (_endpoint != DEFAULT)
    {
        return post(_fields, _headers, _endpoint, type);
    }
    else
    {
        return post(_fields, _headers, type);
    }
}

