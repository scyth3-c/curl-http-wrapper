#include "http/http.hpp"

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