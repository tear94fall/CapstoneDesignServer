#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include "common.hpp"

char *ltrim(char *s);
char *rtrim(char *s);
char *trim(char *s);
char *remove_square_bracket(char *s);

template <typename T>
std::string ToString(T type);

int string_to_int(std::string str);
std::string get_date_string();
std::string make_random_number(void);
std::string day_between_from_two_days(std::string start, std::string end);

#endif
