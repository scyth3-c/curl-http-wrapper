#pragma once

#ifndef HTTP_H
#define HTTP_H

#define CURL_STATICLIB
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "utils/utils.hpp"

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