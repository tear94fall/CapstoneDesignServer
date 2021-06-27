#ifndef __MYSQL_CONN_HPP__
#define __MYSQL_CONN_HPP__

#include "common.hpp"
#include "query.hpp"
#include "util.hpp"

/*
 * .conf file format
 *
 * user_id:"user id"
 * user_pass:"user password"
 * dbname:"data base"
 */

/*
 * db table
 *
 * +----+------+-----+------+
 * | id | name | age | date |
 * +----+------+-----+------+
 * | 1 | name | age | date |
 * +----+------+-----+------+
 */

class MysqlConnector final {
	private:
		MysqlConnector() = default;

		static std::unique_ptr<MysqlConnector> instance;
		static std::once_flag flag;

	public:
		MysqlConnector(const MysqlConnector &) = delete;
		MysqlConnector(MysqlConnector &&) = delete;
		MysqlConnector &operator=(const MysqlConnector &) = delete;
		MysqlConnector &operator=(MysqlConnector &&) = delete;

		static MysqlConnector &get_instance(void);

		std::string user_id;
		std::string user_pw;
		std::string db_name;

		MYSQL mysql;
		MYSQL_RES *result;
		MYSQL_ROW row;
		int fields;

		int mysql_conn_init();
		std::map<std::string, std::string> mysql_conn_run(const char *query);
		std::map<std::string, std::string> mysql_conn_run_no_result(const char *query);
		int mysql_conn_stop();

		int mysql_conf_file_read(const char *conf_file);
};

#define MYSQL_CONN MysqlConnector::get_instance()

#endif
