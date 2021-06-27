#include "mysql_conn.hpp"

std::unique_ptr<MysqlConnector> MysqlConnector::instance = nullptr;
std::once_flag MysqlConnector::flag;

MysqlConnector &MysqlConnector::get_instance(void){
	std::call_once(MysqlConnector::flag, [](){
			MysqlConnector::instance.reset(new MysqlConnector);
			});

	return *MysqlConnector::instance;
}

int MysqlConnector::mysql_conn_init(){
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_options(&mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8");

	if(mysql_real_connect(&mysql, NULL, user_id.c_str(), user_pw.c_str(), db_name.c_str(), 0, NULL ,0) == NULL){
		printf("MySQL Connection Fail() - id : %s, pw : %s, db : %s\n", user_id.c_str(), user_pw.c_str(), db_name.c_str());
		return -1;
	}

	return 1;
}

std::map<std::string, std::string> MysqlConnector::mysql_conn_run_no_result(const char *query){
	std::map<std::string, std::string> result_map;

	int query_status = mysql_query(&mysql, query);
	if(query_status != 0){
		printf("MySQL Query Error - query : %s\n", query);
		result_map.insert(std::make_pair("return_code", "false"));
	}else{
		result_map.insert(std::make_pair("return_code", "true"));
	}

	return result_map;
}

std::map<std::string, std::string> MysqlConnector::mysql_conn_run(const char *query){
	std::map<std::string, std::string> result_map;

	int query_status = mysql_query(&mysql, query);
	if(query_status != 0){
		printf("MySQL Query Error - query : %s\n", query);
	}

	result = mysql_store_result(&mysql);
	if(result==NULL){
		if(!result_map.empty()){
			result_map.clear();
		}

		return result_map;
	}

	std::vector<std::string> field_vec;
	MYSQL_FIELD *field;
	fields = mysql_num_fields(result);

	for(int i=0;i<fields;i++){
		field = mysql_fetch_field(result);
		char *column = field->name;
		field_vec.push_back(std::string(column));
	}

	while(row = mysql_fetch_row(result)){
		for(int i=0; i<fields; i++){
			result_map.insert(std::make_pair(field_vec[i], std::string(row[i])));
		}
	}

	mysql_free_result(result);

	return result_map;
}

int MysqlConnector::mysql_conn_stop(){
	mysql_close(&mysql);

	return 1;
}

int MysqlConnector::mysql_conf_file_read(const char *conf_file){
	FILE *pFile;

	pFile = fopen(conf_file, "r");
	if(pFile==NULL){
		printf("MySQL Conf File Open Error - Not Exist File\n");
		return -1;
	}else{
		while(feof(pFile)==0){
			char str[1024];
			fgets(str, 1024, pFile);

			char *data = NULL, *ptr = NULL, *rptr = NULL;
			data = strdup(str);
			ptr = strtok_r(data, ":", &rptr);
			rptr[strlen(rptr)-1]='\0';

			ptr = trim(ptr);
			rptr = trim(rptr);

			if(strcmp(ptr, "user_id")==0){
				user_id.assign(rptr, strlen(rptr));
			}else if(strcmp(ptr, "user_pass")==0){
				user_pw.assign(rptr, strlen(rptr));
			}else if(strcmp(ptr, "dbname")==0){
				db_name.assign(rptr, strlen(rptr));
			}

			free(data);
		}
	}

	fclose(pFile);

	if(user_id.empty() || user_pw.empty() || db_name.empty()){
		printf("MySQL Conf File Read Error - Invalid value\n");
		return -1;
	}

	printf("MySQL Read Config - id : %s, pw : %s, db : %s\n", user_id.c_str(), user_pw.c_str(), db_name.c_str());

	return 1;
}
