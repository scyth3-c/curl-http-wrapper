#ifndef UTILS_HPP
#define UTILS_HPP

#include<iostream>

constexpr auto UTILS_ERROR = "-[error critico]-";
constexpr auto UTILS_WARNING = "-[advertencia]-";
constexpr auto UTILS_SUCCESS = 0;

constexpr auto SYSTEM_DECORATOR = "-[BAD SYSTEM RESPONSE]-";
constexpr auto CURL_DECORATOR = "-[BAD CURL RESPONSE]-";
constexpr auto BAD_RESULT = "-[BAD_USE]-";

constexpr auto HTTP_ERROR = -1;
constexpr auto CURL_ERROR = -2;
constexpr auto HTTP_SUCCESS = 0;

constexpr auto  UTILS_USER_ERROR = 3;

#define API "https://radiant-dusk-88409.herokuapp.com/"

template<class... C>
auto screen(std::ostream& out, C const &...conten) -> void {
    ((out<<conten), ...);
}


#endif // UTILS_HPP!