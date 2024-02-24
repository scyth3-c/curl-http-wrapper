
// Archivo: http.hpp

#pragma once

#ifndef HTTP_H
#define HTTP_H

#define CURL_STATICLIB
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <memory>
#include <vector>


#define PUT_TYPE "PUT"
#define POST_t "POST"
#define DELETE_TYPE "DELETE"
#define DEFAULT "00"

using std::string;



template<typename A,class...LIST>
struct headers_t {

    std::vector<LIST...> list;
    explicit headers_t(std::initializer_list<LIST...> arguments) : list(arguments) {
          if (list.size() > 0){
              for(auto &it : list) {
                  std::cout << it << std::endl;
                  aloc.push_back(it);
              }
              initial = true;
          }
    }
	bool initial{false};
	std::vector<A> aloc{};
};
typedef headers_t<string,string> Headers;



template <typename A, class... LIST>
struct FIELDS
{
    std::vector<LIST...> list;
   FIELDS(std::initializer_list<LIST...> arguments) : list(arguments) {}

    string http_params{};
    string transform()
    {
        int it = 0;
        for (size_t i = 0; i < list.size(); i++)
        {	 
			if(it == 0) {
				http_params += list[i] + "&";
			} else if(it==2 && i) {
				http_params+="&"+list[i];
				it = 0;
			} else {
				http_params += list[i];
			}
        }
		http_params[http_params.length()-1] = char(00);
        return http_params;
    }
};

typedef FIELDS<string, string> 	POST;
typedef FIELDS<string, string> 	PUT;
typedef FIELDS<string, string> 	GET;
typedef FIELDS<string, string>  DELETE;


class HTTP
{
private:
	std::shared_ptr<CURL *> curl = nullptr;
	
	std::vector<string> http_headers;
	std::vector<CURLcode> performs;
	unsigned int queryIndice = 0;
	string URL{};
	string response{};
	bool headerify{false};

public:
	HTTP(string);
	HTTP();
	~HTTP();
	static size_t callback(void *, size_t, size_t, string *);

	int getSimple(string endpoint_= DEFAULT);
	int getBase(GET*, string endpoint= DEFAULT, bool headers = false, Headers* base = nullptr);
    int get(GET& get, Headers & headers, string endpoint = DEFAULT);
    int getHeaders(Headers &headers, string endpoint = DEFAULT);

    int postSimple(string endpoint = DEFAULT);
    int postHeaders(Headers& headers, string end = DEFAULT);
	int post(POST &, string endpoint = DEFAULT, string type = DEFAULT);
	int post(POST *, Headers &, string endpoint = DEFAULT, string type = DEFAULT);


	int put(PUT &, string endpoint=DEFAULT);
	int put(PUT &, Headers &, string endpoint = DEFAULT);

	int Delete(DELETE &, string endpoint = DEFAULT);
	int Delete(DELETE &, Headers &, string endpoint = DEFAULT);

	int custom(POST &field, string type, string endpoint = DEFAULT);
	int custom(POST &fields, Headers &headers, string type, string endpoint = DEFAULT);


	curl_slist* makeHeaders(std::vector<string>);
    void setUrl(string);
	int setHeaders(Headers&);

	string Response();
	string without(string, char);
	string genPerfomList();
};

#endif // !HTTP_H
// Archivo: veridic.hpp

#pragma once

#ifndef VERIDIC_HPP
#define VERIDIC_HPP

#include <iostream>
#include <memory.h>
#include <string>


#define PLAIN_TEXT "Content-Type: text/plain"

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::vector;

class Veridic {

private:
    shared_ptr<HTTP> http = nullptr;
    string URL{};

public:
    Veridic();
    Veridic(string);
    bool setUrl(string);

    string get(string endpoint = DEFAULT);
    string get(Headers &, string endpoint = DEFAULT);
    string get(GET &, string endpoint = DEFAULT);
    string get(GET &, Headers &, string endpoint = DEFAULT);


    string post(string endpoint = DEFAULT);
    string post(Headers &, string endpoint = DEFAULT);
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
// Archivo: utils.hpp

#ifndef UTILS_HPP
#define UTILS_HPP

#include<iostream>

constexpr auto UTILS_ERROR = "-[critic error]-";
constexpr auto UTILS_WARNING = "-[warning]-";
constexpr auto UTILS_SUCCESS = 0;

constexpr auto SYSTEM_DECORATOR = "-[BAD SYSTEM RESPONSE]-";
constexpr auto CURL_DECORATOR = "-[BAD CURL RESPONSE]-";
constexpr auto BAD_RESULT = "-[BAD_USE]-";

constexpr auto HTTP_ERROR = -1;
constexpr auto CURL_ERROR = -2;
constexpr auto HTTP_SUCCESS = 0;

constexpr auto  UTILS_USER_ERROR = 3;

template<class... C>
auto screen(std::ostream& out, C const &...content) -> void {
    ((out<<content), ...);
}


#endif // UTILS_HPP!
// Archivo: http.cpp


HTTP::HTTP(string url) : URL(url) {
	curl = std::make_shared<CURL *>(curl_easy_init());
	if (!curl) {
		screen(std::cout, UTILS_ERROR, SYSTEM_DECORATOR);
	}
}

HTTP::HTTP() {}
HTTP::~HTTP() {curl.reset();}

// callback function, this extract response of querys
size_t HTTP::callback(void *buffer, size_t size, size_t nmemb, string *data) {
	size_t rsize = size * nmemb;
	try {
		data->append(static_cast<const char *>(buffer), rsize);
	}
	catch (std::bad_alloc &e) {
		screen(std::clog, UTILS_ERROR, SYSTEM_DECORATOR, &e);
	}
	return rsize;
}



// base http get
int HTTP::getSimple(std::string endpoint) {

    string temp_url = endpoint != DEFAULT ? URL +endpoint : URL;

    curl_easy_setopt(*curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(*curl, CURLOPT_URL, temp_url.c_str());
	curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, HTTP::callback);
	curl_easy_setopt(*curl, CURLOPT_WRITEDATA, &response);

	CURLcode perfom = curl_easy_perform(*curl);
	performs.push_back(perfom);

	try {
		if (perfom != CURLE_OK)
			screen(std::clog, UTILS_WARNING, CURL_DECORATOR, perfom);
	}
	catch (std::exception &error) {
		screen(std::cout, UTILS_ERROR, &error);
	}
	curl_easy_cleanup(*curl);
	return HTTP_SUCCESS;
}

int HTTP::getBase(GET *fields, std::string endpoint, bool isHead, Headers *headers) {

    string temp_url = endpoint != DEFAULT ? URL +endpoint : URL;
    struct curl_slist *headersList = nullptr;

	if(fields != nullptr) {
        temp_url += "?" + fields->transform();
        fields = nullptr;
	}

    curl_easy_setopt(*curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(*curl, CURLOPT_URL, temp_url.c_str());
	curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, HTTP::callback);
	curl_easy_setopt(*curl, CURLOPT_WRITEDATA, &response);

    if (isHead) {
      headersList = makeHeaders(headers->aloc);
      if (headersList != nullptr){
          curl_easy_setopt(*curl, CURLOPT_HTTPHEADER, headersList );
      }
    }

	CURLcode perfom = curl_easy_perform(*curl);
	performs.push_back(perfom);

    if (headersList != nullptr){
        curl_slist_free_all(headersList);
        headers = nullptr;
    }

	try {
		if (perfom != CURLE_OK)
			screen(std::clog, UTILS_WARNING, CURL_DECORATOR, perfom);
	}
	catch (std::exception &error){
        screen(std::cout, UTILS_ERROR, &error);
		return CURL_ERROR;
	}
	curl_easy_cleanup(*curl);
	return HTTP_SUCCESS;
}

int HTTP::get(GET &fields, Headers &headers, std::string endpoint)  {
    return HTTP::getBase(&fields, endpoint, true, &headers);
}
int HTTP::getHeaders(Headers &headers, string endpoint) {
    return HTTP::getBase(nullptr, endpoint, true, &headers);
}

// base http post
int HTTP::post(POST &fields, string endpoint, string type) {

    string temp_url = endpoint != DEFAULT ? URL +endpoint : URL;

	auto postFields = fields.transform();

    curl_easy_setopt(*curl, CURLOPT_URL, temp_url.c_str());
	curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, HTTP::callback);
	curl_easy_setopt(*curl, CURLOPT_POSTFIELDS, postFields.c_str());
	curl_easy_setopt(*curl, CURLOPT_WRITEDATA, &response);

	if (type != DEFAULT) {
		curl_easy_setopt(*curl, CURLOPT_CUSTOMREQUEST, type.c_str());
	} else{
        curl_easy_setopt(*curl, CURLOPT_CUSTOMREQUEST, (const char*)POST_t);
    }

	CURLcode perfom = curl_easy_perform(*curl);
	performs.push_back(perfom);

	try {
		if (perfom != CURLE_OK)
			screen(std::clog, UTILS_WARNING, CURL_DECORATOR, perfom);
	}
	catch (std::exception &error) {
        screen(std::cout, UTILS_ERROR, &error);
		return CURL_ERROR;
	}
	curl_easy_cleanup(*curl);
	return HTTP_SUCCESS;
}

int HTTP::post(POST *fields, Headers &headers, string endpoint, string type) {

    string temp_url = endpoint != DEFAULT ? URL +endpoint : URL;
    struct curl_slist *headersList = nullptr;

	curl_easy_setopt(*curl, CURLOPT_URL, temp_url.c_str());
	curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, HTTP::callback);
	curl_easy_setopt(*curl, CURLOPT_WRITEDATA, &response);

    if(fields != nullptr) {
        auto postFields = fields->transform();
        curl_easy_setopt(*curl, CURLOPT_POSTFIELDS, postFields.c_str());
        fields = nullptr;
    }

    if (type != DEFAULT){
        curl_easy_setopt(*curl, CURLOPT_CUSTOMREQUEST, type.c_str());
    }

    std::cout << headers.list.size() <<std::endl;

    headersList = makeHeaders(headers.aloc);
    curl_easy_setopt(*curl, CURLOPT_HTTPHEADER, headersList );

	CURLcode perfom = curl_easy_perform(*curl);
	performs.push_back(perfom);

    curl_slist_free_all(headersList);

	try {
		if (perfom != CURLE_OK)
			screen(std::clog, UTILS_WARNING, CURL_DECORATOR, perfom);
	}
	catch (std::exception &error) {
        screen(std::cout, UTILS_ERROR, &error);
		return CURL_ERROR;
	}
	curl_easy_cleanup(*curl);
	return HTTP_SUCCESS;
}

int HTTP::postHeaders(Headers &headers, std::string end) {
    return HTTP::post(nullptr, headers, end, "POST");
}

int HTTP::postSimple(std::string endpoint) {
    string temp_url = endpoint != DEFAULT ? URL +endpoint : URL;

    curl_easy_setopt(*curl, CURLOPT_URL, temp_url.c_str());
    curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, HTTP::callback);
    curl_easy_setopt(*curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(*curl, CURLOPT_CUSTOMREQUEST, (const char*)POST_t);

    CURLcode perfom = curl_easy_perform(*curl);
    performs.push_back(perfom);

    try {
        if (perfom != CURLE_OK)
            screen(std::clog, UTILS_WARNING, CURL_DECORATOR, perfom);
    }
    catch (std::exception &error) {
        screen(std::cout, UTILS_ERROR, &error);
        return CURL_ERROR;
    }
    curl_easy_cleanup(*curl);
    return HTTP_SUCCESS;
}

int HTTP::put(PUT &_fields, string _endpoint) {
    return (_endpoint != DEFAULT) ? post(_fields, _endpoint, PUT_TYPE) : post(_fields, PUT_TYPE);
}

int HTTP::put(PUT &_fields, Headers &_headers, string _endpoint) {
    return (_endpoint != DEFAULT) ? post(&_fields, _headers, _endpoint, PUT_TYPE) : post(&_fields, _headers, PUT_TYPE);
}

int HTTP::Delete(DELETE &_fields, string _endpoint) {
    return (_endpoint != DEFAULT) ? post(_fields, _endpoint, DELETE_TYPE) : post(_fields, DELETE_TYPE);
}

int HTTP::Delete(DELETE &_fields, Headers &_headers, string _endpoint) {
    return (_endpoint != DEFAULT) ? post(&_fields, _headers, _endpoint, DELETE_TYPE) : post(&_fields, _headers, DELETE_TYPE);
}

int HTTP::custom(POST &_fields, string type, string _endpoint) {
    return (_endpoint != DEFAULT) ? post(_fields, _endpoint, type) : post(_fields, type);
}

int HTTP::custom(POST &_fields, Headers &_headers, string type, string _endpoint) {
    return (_endpoint != DEFAULT) ? post(&_fields, _headers, _endpoint, type) : post(&_fields, _headers, type);
}

int HTTP::setHeaders(Headers &headers)
{

	if (!headers.initial)
		return UTILS_USER_ERROR;
	http_headers = headers.aloc;
	headerify = true;
	return UTILS_SUCCESS;
}

curl_slist *HTTP::makeHeaders(std::vector<string> headers) {
        struct curl_slist *list = nullptr;

        if (headers.empty())
            return list;

        for (auto &it : headers) {
            std::cout << it << std::endl;
            list = curl_slist_append(list, it.c_str());
        }
        return list;
}

void HTTP::setUrl(string _url){
	URL = _url;
}

// extract response of the querys
string HTTP::Response()
{
	return without(response, char(34));
}

// remove separator send of the API
string HTTP::without(string target, char key) {
	for (auto &it : target) {
		if (it == key)
			it = char(00);
	}
	return target;
}

// extract the list of result of querys
string HTTP::genPerfomList(){
	int map{};
	for (auto &it : performs){
		map += it;
	}
	return std::to_string(map);
}
// Archivo: veridic.cpp


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