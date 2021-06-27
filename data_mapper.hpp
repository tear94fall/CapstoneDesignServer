#ifndef __DATA_HPP__
#define __DATA_HPP__

#include "common.hpp"

// DataMapper
class DataMapper{
	public:
		DataMapper();
		~DataMapper();

		void init_data_map(void);
		void *data_mapper(int cmd, std::map<std::string, std::string> data_map);
};

// [{request_number=2, echo_test_key=echo_test_value}]
// [{user_pw=1234, request_number=4, user_id=root}]


class Data{
	int command_number;
};

// Login
class Login : public Data{
	std::string id;
	std::string passwd;
};

// Member
class Member : public Data{
	public:
		std::string id;
		std::string passwd;
		std::string name;
		std::string email;
		std::string phone;
};

#endif
