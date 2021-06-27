#ifndef __JSON_MANAGER_HPP__
#define __JSON_MANAGER_HPP__

#include "util.hpp"

class JsonManager{
	public:
		char *data;    

		JsonManager();
		~JsonManager();

		int set_data(char *data);
		std::map<std::string, std::string> parse_json(char *str);
		char *make_json(std::map<std::string, std::string> data);
};

#endif
