#include "command.hpp"

// Echo
std::map<std::string, std::string> EchoCommand::execute_command(std::map<std::string, std::string> &data){
	return echo_command(data);
}

std::map<std::string, std::string> EchoCommand::echo_command(std::map<std::string, std::string> &data){
	return data;
}

// Login
std::map<std::string, std::string> LoginCommand::execute_command(std::map<std::string, std::string> &data){
	return login_command(data);
}

std::map<std::string, std::string> LoginCommand::login_command(std::map<std::string, std::string> &data){
	std::map<std::string, std::string> result_map;
	
	std::string userid = data["user_id"];
	std::string userpw = data["user_pw"];

	if(userid.empty() || userpw.empty()){
		result_map.insert(std::make_pair("return_code", "false"));
		return result_map;
	}

	std::string query = SELECT + PW_COLUMN + FROM + MEMBER_TABLE + WHERE + ID_COLUMN + EQUAL + QUOT + userid + QUOT + SEMICOL;
	result_map = MYSQL_CONN.mysql_conn_run(query.c_str());
	result_map.insert(std::make_pair("request_number", data["request_number"]));

	std::string passwd = result_map["passwd"];
	
	if(passwd.empty()){
		result_map.clear();
		result_map.insert(std::make_pair("return_code", "true"));

		return result_map;
	}

	if(passwd == userpw){
		result_map.insert(std::make_pair("return_code", "true"));
	}else{
		result_map.clear();
		result_map.insert(std::make_pair("return_code", "false"));
	}

	return result_map;
}

// IdCheck
std::map<std::string, std::string> IdCheckCommand::execute_command(std::map<std::string, std::string> &data){
	return id_check_command(data);
}

std::map<std::string, std::string> IdCheckCommand::id_check_command(std::map<std::string, std::string> &data){
	std::map<std::string, std::string> result_map;
	
	std::string userid = data["user_id"];

	if(userid.empty()){
		result_map.insert(std::make_pair("return_code", "false"));
		return result_map;
	}

	std::string query = SELECT + COUNT_ALL + FROM + MEMBER_TABLE + WHERE + ID_COLUMN + EQUAL + QUOT + userid + QUOT + SEMICOL;
	result_map = MYSQL_CONN.mysql_conn_run(query.c_str());
	result_map.insert(std::make_pair("request_number", data["request_number"]));

	std::string id_count = result_map["COUNT(*)"];

	if(id_count.empty()){
		result_map.clear();
		result_map.insert(std::make_pair("return_code", "false"));

		return result_map;
	}

	if(id_count!="0"){
		result_map.insert(std::make_pair("return_code", "true"));
	}else{
		result_map.clear();
		result_map.insert(std::make_pair("return_code", "false"));
	}

	return result_map;
}

// CreateAccount
std::map<std::string, std::string> CreateAccountCommand::execute_command(std::map<std::string, std::string> &data){
	return create_account_command(data);
}

std::map<std::string, std::string> CreateAccountCommand::create_account_command(std::map<std::string, std::string> &data){
	std::map<std::string, std::string> result_map;
	
	std::string id = data["user_id"];
	std::string pw = data["user_pw"];
	std::string name = data["user_name"];
	std::string tel = data["user_tel"];
	std::string date = get_date_string();

	std::string query = INSERT + INTO + MEMBER_TABLE + \
		LPARENS + \
		ID_COLUMN + COMMA + PW_COLUMN + COMMA + NAME_COLUMN + COMMA + TEL_COLUMN + COMMA + LAST_DRIVE_DATE_COLUMN + \
		RPARENS + \
		WS + \
		VALUES + \
		LPARENS + \
		DQUOT + id + DQUOT + COMMA + \
		DQUOT + pw + DQUOT + COMMA + \
		DQUOT + name + DQUOT + COMMA + \
		DQUOT + tel + DQUOT + COMMA + \
		DQUOT + date + DQUOT + \
		RPARENS + \
		SEMICOL;

	result_map = MYSQL_CONN.mysql_conn_run_no_result(query.c_str());
	result_map.insert(std::make_pair("request_number", data["request_number"]));

	return result_map;
}

// GetCapchatTestCase
std::map<std::string, std::string> GetCaptchaTestCaseCommand::execute_command(std::map<std::string, std::string> &data) {
	return get_captcha_test_case_command(data);
}

std::map<std::string, std::string> GetCaptchaTestCaseCommand::get_captcha_test_case_command(std::map<std::string, std::string> &data){
	std::map<std::string, std::string> result_map;
	
	std::string number = make_random_number();

	std::string query = SELECT + CAPTCHA_ANSWER + FROM + CAPTCHA_TABLE + WHERE + CAPTCHA_NUM + EQUAL + DQUOT + number + DQUOT + SEMICOL;

	result_map = MYSQL_CONN.mysql_conn_run(query.c_str());
	result_map.insert(std::make_pair("request_number", data["request_number"]));

	return result_map;
}

// GetLastDriveDate
std::map<std::string, std::string> GetLastDriveDateCommand::execute_command(std::map<std::string, std::string> &data){
	return get_last_drive_date_command(data);
}

std::map<std::string, std::string> GetLastDriveDateCommand::get_last_drive_date_command(std::map<std::string, std::string> &data){
	std::map<std::string, std::string> result_map;
	
	std::string userid = data["user_id"];

	if(userid.empty()){
		result_map.insert(std::make_pair("return_code", "false"));
		return result_map;
	}

	std::string query = SELECT + LAST_DRIVE_DATE_COLUMN + FROM + MEMBER_TABLE + WHERE + ID_COLUMN + EQUAL + QUOT + userid + QUOT + SEMICOL;
	result_map = MYSQL_CONN.mysql_conn_run(query.c_str());
	result_map.insert(std::make_pair("request_number", data["request_number"]));

	std::string last_drive_date = result_map["last_drive_date"];
	
	if(last_drive_date.empty()){
		result_map.clear();
		result_map.insert(std::make_pair("return_code", "false"));

		return result_map;
	}
	
	std::string date = get_date_string();
	std::string days = day_between_from_two_days(last_drive_date, date);

	result_map.insert(std::make_pair("last_drive_date", days));

	return result_map;
}

// GetCapchat2TestCase
std::map<std::string, std::string> GetCaptcha2TestCaseCommand::execute_command(std::map<std::string, std::string> &data) {
	return get_captcha2_test_case_command(data);
}

std::map<std::string, std::string> GetCaptcha2TestCaseCommand::get_captcha2_test_case_command(std::map<std::string, std::string> &data){
	std::map<std::string, std::string> result_map;
	
	std::string captcha2_number = data["captcha2_number"];
	std::string captcha2_answer = data["captcha2_answer"];

	std::string query = SELECT + CAPTCHA_ANSWER + FROM + CAPTCHA_TABLE2 + WHERE + CAPTCHA_NUM + EQUAL + DQUOT + captcha2_number + DQUOT + SEMICOL;

	result_map = MYSQL_CONN.mysql_conn_run(query.c_str());
	result_map.insert(std::make_pair("request_number", data["request_number"]));

	std::string answer = result_map["captcha_answer"];

	if(answer == captcha2_answer){
		result_map.insert(std::make_pair("return_code", "true"));
	}

	return result_map;
}

// GetUserInfo
std::map<std::string, std::string> GetUserInfoCommand::execute_command(std::map<std::string, std::string> &data){
	return get_user_info_command(data);
}

std::map<std::string, std::string> GetUserInfoCommand::get_user_info_command(std::map<std::string, std::string> &data){
	std::map<std::string, std::string> result_map;
	std::map<std::string, std::string>::iterator result_map_iter;
	
	std::string userid = data["userid"];

	std::string query = SELECT + \
			    	DRIVE_CNT_COLUMN + COMMA + \
				LAST_DRIVE_DATE_COLUMN + COMMA + \
				SLEEP_DETECT_CNT_COLUMN + COMMA + \
				ALCOHOL_DETECT_CNT_COLUMN + \
				FROM + MEMBER_TABLE + WHERE + ID_COLUMN + EQUAL + QUOT + userid + QUOT + SEMICOL;
	result_map = MYSQL_CONN.mysql_conn_run(query.c_str());

	std::string last_drive_date = result_map["last_drive_date"];
	last_drive_date.replace(last_drive_date.find("-"), 1, "/");
	last_drive_date.replace(last_drive_date.find("-"), 1, "/");

	std::string drive_cnt = result_map["drive_cnt"];
	std::string sleep_detect_cnt = result_map["sleep_detect_cnt"];
	std::string alcohol_detect_cnt = result_map["alcohol_detect_cnt"];

	result_map.insert(std::make_pair("request_number", data["request_number"]));
	result_map.insert(std::make_pair("drive_info", drive_cnt + "-" + sleep_detect_cnt + "-" + alcohol_detect_cnt + "-" + last_drive_date));

	return result_map;
}

// GetAllUserInfo
std::map<std::string, std::string> GetAllUserInfoCommand::execute_command(std::map<std::string, std::string> &data){
	return get_all_user_info_command(data);
}

std::map<std::string, std::string> GetAllUserInfoCommand::get_all_user_info_command(std::map<std::string, std::string> &data){
	std::map<std::string, std::string> result_map;
	std::map<std::string, std::string>::iterator result_map_iter;
	
	std::string userid = data["userid"];

	std::string query = SELECT + ALL + FROM + MEMBER_TABLE + WHERE + ID_COLUMN + EQUAL + QUOT + userid + QUOT + SEMICOL;
	result_map = MYSQL_CONN.mysql_conn_run(query.c_str());

	std::string user_id = result_map["id"];
	std::string user_pwd = result_map["passwd"];
	std::string user_name = result_map["name"];
	std::string user_tel = result_map["tel"];

	result_map.insert(std::make_pair("user_info", user_id + "-" + user_pwd + "-" + user_name + "-" + user_tel));
	result_map.insert(std::make_pair("request_number", data["request_number"]));

	return result_map;
}

// UpdateUserInfo
std::map<std::string, std::string> UpdateUserInfoCommand::execute_command(std::map<std::string, std::string> &data){
	return update_user_info_command(data);
}

std::map<std::string, std::string> UpdateUserInfoCommand::update_user_info_command(std::map<std::string, std::string> &data){
	std::map<std::string, std::string> result_map;
	std::map<std::string, std::string>::iterator result_map_iter;
	
	std::string user_id = data["userid"];
	std::string user_pwd = data["userpassword"];
	std::string user_name = data["username"];
	std::string user_tel = data["usertel"];

	std::string query = SELECT + PRIMARY_KEY + FROM + MEMBER_TABLE + WHERE + ID_COLUMN + EQUAL + QUOT + user_id + QUOT + SEMICOL;
	result_map = MYSQL_CONN.mysql_conn_run(query.c_str());

	std::string primary_key = result_map["idx"];
	
	query = UPDATE + MEMBER_TABLE + SET + \
		PW_COLUMN + EQUAL + DQUOT + user_pwd + DQUOT + COMMA + \
		NAME_COLUMN + EQUAL + DQUOT + user_name + DQUOT + COMMA + \
		TEL_COLUMN + EQUAL + DQUOT + user_tel + DQUOT + \
		WHERE + PRIMARY_KEY + EQUAL + DQUOT + primary_key + DQUOT + SEMICOL;

	result_map = MYSQL_CONN.mysql_conn_run_no_result(query.c_str());
	result_map.insert(std::make_pair("request_number", data["request_number"]));

	return result_map;
}

std::map<std::string, std::string> UpdateLastDriveDateCommand::execute_command(std::map<std::string, std::string> &data){
	return update_last_drive_date_command(data);
}

std::map<std::string, std::string> UpdateLastDriveDateCommand::update_last_drive_date_command(std::map<std::string, std::string> &data){
	std::map<std::string, std::string> result_map;
	std::map<std::string, std::string>::iterator result_map_iter;
	
	std::string user_id = data["userid"];

	std::string query = SELECT + \
		PRIMARY_KEY + COMMA + DRIVE_CNT_COLUMN + COMMA + LAST_DRIVE_DATE_COLUMN + \
		FROM + MEMBER_TABLE + WHERE + ID_COLUMN + EQUAL + QUOT + user_id + QUOT + SEMICOL;
	result_map = MYSQL_CONN.mysql_conn_run(query.c_str());

	std::string primary_key = result_map["idx"];
	std::string drive_cnt = result_map["drive_cnt"];
	std::string last_drive_date = result_map["last_drive_date"];

	int cnt_drive = string_to_int(drive_cnt) + 1;
	drive_cnt.clear();
	drive_cnt = std::to_string(cnt_drive);
	std::string current_date = get_date_string();

	query = UPDATE + MEMBER_TABLE + SET + DRIVE_CNT_COLUMN + EQUAL + DQUOT + drive_cnt + DQUOT;
	if(current_date != last_drive_date){
		query+=COMMA + LAST_DRIVE_DATE_COLUMN + EQUAL + DQUOT + current_date + DQUOT;
	}
	query+=WHERE + PRIMARY_KEY + EQUAL + DQUOT + primary_key + DQUOT + SEMICOL;

	result_map = MYSQL_CONN.mysql_conn_run_no_result(query.c_str());
	result_map.insert(std::make_pair("request_number", data["request_number"]));

	return result_map;
}

std::map<std::string, std::string> UpdateAlcoholCountCommand::execute_command(std::map<std::string, std::string> &data){
	return update_alcohol_count_command(data);
}

std::map<std::string, std::string> UpdateAlcoholCountCommand::update_alcohol_count_command(std::map<std::string, std::string> &data){
	std::map<std::string, std::string> result_map;
	std::map<std::string, std::string>::iterator result_map_iter;
	
	std::string user_id = data["userid"];

	std::string query = SELECT + \
		PRIMARY_KEY + COMMA + ALCOHOL_DETECT_CNT_COLUMN + \
		FROM + MEMBER_TABLE + WHERE + ID_COLUMN + EQUAL + QUOT + user_id + QUOT + SEMICOL;
	result_map = MYSQL_CONN.mysql_conn_run(query.c_str());

	std::string primary_key = result_map["idx"];
	std::string alcohol_drive_cnt = result_map["alcohol_detect_cnt"];

	int cnt_alcohol_drive = string_to_int(alcohol_drive_cnt) + 1;
	alcohol_drive_cnt.clear();
	alcohol_drive_cnt = std::to_string(cnt_alcohol_drive);

	query = UPDATE + MEMBER_TABLE + SET + ALCOHOL_DETECT_CNT_COLUMN + EQUAL + DQUOT + alcohol_drive_cnt + DQUOT + \
		WHERE + PRIMARY_KEY + EQUAL + DQUOT + primary_key + DQUOT + SEMICOL;

	result_map = MYSQL_CONN.mysql_conn_run_no_result(query.c_str());
	result_map.insert(std::make_pair("request_number", data["request_number"]));

	return result_map;
}

std::map<std::string, std::string> FindUserIdCommand::execute_command(std::map<std::string, std::string> &data){
	return find_user_id_command(data);
}

std::map<std::string, std::string> FindUserIdCommand::find_user_id_command(std::map<std::string, std::string> &data){
	std::map<std::string, std::string> result_map;
	
	std::string username = data["username"];
	std::string usertel = data["usertel"];

	std::string query = SELECT + ID_COLUMN + FROM + MEMBER_TABLE + WHERE + \
		NAME_COLUMN + EQUAL + QUOT + username + QUOT + AND + \
		TEL_COLUMN + EQUAL + QUOT + usertel + QUOT + SEMICOL;
	result_map = MYSQL_CONN.mysql_conn_run(query.c_str());

	std::string user_id = result_map["id"];
	
	if(user_id.empty()){
		result_map.clear();
		result_map.insert(std::make_pair("return_code", "false"));

		return result_map;
	}

	result_map.clear();
	result_map.insert(std::make_pair("userid", user_id));
	result_map.insert(std::make_pair("request_number", data["request_number"]));

	return result_map;
}

std::map<std::string, std::string> FindUserPasswdCommand::execute_command(std::map<std::string, std::string> &data){
	return find_user_passwd_command(data);
}

std::map<std::string, std::string> FindUserPasswdCommand::find_user_passwd_command(std::map<std::string, std::string> &data){
	std::map<std::string, std::string> result_map;

	std::string userid = data["userid"];
	std::string username = data["username"];

	std::string query = SELECT + PW_COLUMN + FROM + MEMBER_TABLE + WHERE + \
		ID_COLUMN + EQUAL + QUOT + userid + QUOT + AND + \
		NAME_COLUMN + EQUAL + QUOT + username + QUOT + SEMICOL;
	result_map = MYSQL_CONN.mysql_conn_run(query.c_str());

	std::string user_pw = result_map["passwd"];
	
	if(user_pw.empty()){
		result_map.clear();
		result_map.insert(std::make_pair("return_code", "false"));

		return result_map;
	}

	result_map.clear();
	result_map.insert(std::make_pair("userpw", user_pw));
	result_map.insert(std::make_pair("request_number", data["request_number"]));

	return result_map;
}
