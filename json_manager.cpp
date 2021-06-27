
#include "common.hpp"
#include "json_manager.hpp"

JsonManager::JsonManager(){

}

JsonManager::~JsonManager(){

}

int JsonManager::set_data(char *data){
	data = data;
	return 1;
}

std::map<std::string, std::string> JsonManager::parse_json(char *str){
	std::map<std::string, std::string> map;

	char *origin = NULL, *data = NULL, *ptr = NULL, *rptr = NULL;
	origin = strdup(str);
	data = origin;

	data++;
	data[strlen(data)-1]='\0';
	data = trim(data);

	for(ptr = strtok_r(data, ",", &rptr); ptr; ptr = strtok_r(NULL, ",", &rptr)){
		char *column = NULL, *key = NULL, *value = NULL;

		column = strdup(ptr);
		key = strtok_r(column, "=", &value);
		key = trim(key);
		value = trim(value);

		map.insert(std::make_pair(std::string(key), std::string(value)));

		delete column;
	}

	delete origin;

	return map;
}

char *JsonManager::make_json(std::map<std::string, std::string> json_map){
	char *return_data = NULL;
	std::string data;

	if(json_map.size() == 0){
		return NULL;
	}

	data += "{";

	for(auto it = json_map.begin(); it != json_map.end(); it++){
		data = data + it->first + "=" + it->second + ", ";
	}

	data = data.substr(0, data.size()-2);
	data += "}"; 

	return_data = strdup(data.c_str());

	return return_data;
}
