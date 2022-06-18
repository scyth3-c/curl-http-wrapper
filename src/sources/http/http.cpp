#include "http/http.hpp"

HTTP::HTTP(string url) : URL(url)
{
	curl = std::make_shared<CURL *>(curl_easy_init());
	if (!curl)
	{
		screen(std::cout, UTILS_ERROR, SYSTEM_DECORATOR);
	}
}

HTTP::HTTP() {}

HTTP::~HTTP()
{
	curl.reset();
}

// callback function, this extract response of querys
size_t HTTP::callback(void *buffer, size_t size, size_t nmemb, string *data)
{
	size_t rsize = size * nmemb;
	try
	{
		data->append(static_cast<const char *>(buffer), rsize);
	}
	catch (std::bad_alloc &e)
	{
		screen(std::clog, UTILS_ERROR, SYSTEM_DECORATOR, &e);
	}
	return rsize;
}

// base http get
int HTTP::get(string endpoint)
{

	string temp_url{};

	if (endpoint != DEFAULT)
	{
		temp_url = URL + endpoint;
	}
	else
	{
		temp_url = URL;
	}

	curl_easy_setopt(*curl, CURLOPT_URL, temp_url.c_str());
	curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, HTTP::callback);
	curl_easy_setopt(*curl, CURLOPT_WRITEDATA, &response);
	CURLcode perfom = curl_easy_perform(*curl);
	performs.push_back(perfom);

	try
	{
		if (perfom != CURLE_OK)
			screen(std::clog, UTILS_WARNING, CURL_DECORATOR, perfom);
	}
	catch (std::exception &error)
	{
		screen(std::cout, UTILS_ERROR, &error);
	}
	curl_easy_cleanup(*curl);
	return HTTP_SUCCESS;
}

int HTTP::get(GET &fields, string endpoint)
{
	string temp_url{};

	if (endpoint != DEFAULT)
	{
		temp_url = URL + endpoint + "?" + fields.transform();
	}
	else
	{
		temp_url = URL;
	}

	curl_easy_setopt(*curl, CURLOPT_URL, temp_url.c_str());
	curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, HTTP::callback);
	curl_easy_setopt(*curl, CURLOPT_WRITEDATA, &response);
	CURLcode perfom = curl_easy_perform(*curl);
	performs.push_back(perfom);

	try
	{
		if (perfom != CURLE_OK)
			screen(std::clog, UTILS_WARNING, CURL_DECORATOR, perfom);
	}
	catch (std::exception &error)
	{
		return CURL_ERROR;
		screen(std::cout, UTILS_ERROR, &error);
	}
	curl_easy_cleanup(*curl);
	return HTTP_SUCCESS;
}

// base http post
int HTTP::post(POST &fields, string endpoint, string type)
{
	string temp_url{};

	if (endpoint != DEFAULT)
	{
		temp_url = URL + endpoint;
	}
	else
	{
		temp_url = URL;
	}

	auto postFields = fields.transform();
	curl_easy_setopt(*curl, CURLOPT_URL, temp_url.c_str());
	curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, HTTP::callback);
	curl_easy_setopt(*curl, CURLOPT_POSTFIELDS, postFields.c_str());
	curl_easy_setopt(*curl, CURLOPT_WRITEDATA, &response);

	if (type != DEFAULT)
	{
		curl_easy_setopt(*curl, CURLOPT_CUSTOMREQUEST, type.c_str());
	}

	CURLcode perfom = curl_easy_perform(*curl);
	performs.push_back(perfom);

	try
	{
		if (perfom != CURLE_OK)
			screen(std::clog, UTILS_WARNING, CURL_DECORATOR, perfom);
	}
	catch (std::exception &error)
	{
		return CURL_ERROR;
		screen(std::cout, UTILS_ERROR, &error);
	}
	curl_easy_cleanup(*curl);
	return HTTP_SUCCESS;
}

int HTTP::post(POST &fields, Headers &headers, string endpoint, string type)
{
	string temp_url{};

	if (endpoint != DEFAULT)
	{
		temp_url = URL + endpoint;
	}
	else
	{
		temp_url = URL;
	}
	auto postFields = fields.transform();
	curl_easy_setopt(*curl, CURLOPT_URL, temp_url.c_str());
	curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, HTTP::callback);
	curl_easy_setopt(*curl, CURLOPT_POSTFIELDS, postFields.c_str());
	curl_easy_setopt(*curl, CURLOPT_WRITEDATA, &response);

	if (type != DEFAULT)
		curl_easy_setopt(*curl, CURLOPT_CUSTOMREQUEST, type.c_str());

	curl_easy_setopt(*curl, CURLOPT_HTTPHEADER, makeHeaders(headers.aloc));

	CURLcode perfom = curl_easy_perform(*curl);
	performs.push_back(perfom);

	try
	{
		if (perfom != CURLE_OK)
			screen(std::clog, UTILS_WARNING, CURL_DECORATOR, perfom);
	}
	catch (std::exception &error)
	{
		return CURL_ERROR;
		screen(std::cout, UTILS_ERROR, &error);
	}
	curl_easy_cleanup(*curl);
	return HTTP_SUCCESS;
}

int HTTP::put(PUT &_fields, string _endpoint)
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

int HTTP::put(PUT &_fields, Headers &_headers, string _endpoint)
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

int HTTP::Delete(DELETE &_fields, string _endpoint)
{

	if (_endpoint != DEFAULT)
	{
		return post(_fields, _endpoint, DELETE_TYPE);
	}
	else
	{
		return post(_fields, DELETE_TYPE);
	}
}
int HTTP::Delete(DELETE &_fields, Headers &_headers, string _endpoint)
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

int HTTP::custom(POST &_fields, string type, string _endpoint)
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

int HTTP::custom(POST &_fields, Headers &_headers, string type, string _endpoint)
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

int HTTP::setHeaders(Headers &headers)
{

	if (!headers.initial)
		return UTILS_USER_ERROR;
	http_headers = headers.aloc;
	headerify = true;
	return UTILS_SUCCESS;
}

curl_slist *HTTP::makeHeaders(std::vector<string> headers)
{
	struct curl_slist *list = NULL;
	if (headers.empty())
		return list;
	for (auto &it : headers)
	{
		list = curl_slist_append(list, it.c_str());
	}
	return list;
}

void HTTP::setUrl(string _url)
{
	URL = _url;
}

// extract response of the querys
string HTTP::Response()
{
	return without(response, char(34));
}

// remove separator send of the API
string HTTP::without(string target, char key)
{
	for (auto &it : target)
	{
		if (it == key)
		{
			it = char(00);
		}
	}
	return target;
}

// extract the list of result of querys
string HTTP::genPerfomList()
{
	int map{};
	for (auto &it : performs)
	{
		map += it;
	}
	return std::to_string(map);
}