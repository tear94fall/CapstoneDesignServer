#ifndef __COMMNAD_HPP__
#define __COMMAND_HPP__

#include "common.hpp"
#include "mysql_conn.hpp"

#define CMD_ECHO			2
#define CMD_LOGIN			4
#define CMD_ID_CHECK			6
#define CMD_CREATE_ACCOUNT		8
#define CMD_GET_CAPTCHA			10
#define CMD_GET_LAST_DRIVE_DATE		12
#define CMD_GET_CAPTCHA2		14
#define CMD_GET_USER_INFO		16
#define CMD_GET_ALL_USER_INFO		18
#define CMD_UPDATE_USER_INFO		20
#define CMD_UPDATE_LAST_DRIVE_DATE	22
#define CMD_UPDATE_ALCOHOL_COUNT	24
#define CMD_FIND_USER_ID		26
#define CMD_FIND_USER_PASSWD 		28

class Command {
	public:
		virtual std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) = 0;
};

// Echo
class EchoCommand : public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> echo_command(std::map<std::string, std::string> &data);
};

// Login
class LoginCommand : public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> login_command(std::map<std::string, std::string> &data);
};

// IdCheck
class IdCheckCommand : public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> id_check_command(std::map<std::string, std::string> &data);
};

// CreateAccount
class CreateAccountCommand : public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> create_account_command(std::map<std::string, std::string> &data);
};

// GetCaptchaTestCase
class GetCaptchaTestCaseCommand : public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> get_captcha_test_case_command(std::map<std::string, std::string> &data);
};

// GetLastDriveDate
class GetLastDriveDateCommand: public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> get_last_drive_date_command(std::map<std::string, std::string> &data);
};

// GetCaptcha2TestCase
class GetCaptcha2TestCaseCommand : public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> get_captcha2_test_case_command(std::map<std::string, std::string> &data);
};

// GetUserInfo
class GetUserInfoCommand : public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> get_user_info_command(std::map<std::string, std::string> &data);
};

// GetAllUserInfo
class GetAllUserInfoCommand : public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> get_all_user_info_command(std::map<std::string, std::string> &data);
};

// UpdateUserInfo
class UpdateUserInfoCommand : public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> update_user_info_command(std::map<std::string, std::string> &data);
};

// UpdateLastDriveDate
class UpdateLastDriveDateCommand : public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> update_last_drive_date_command(std::map<std::string, std::string> &data);
};

// UpdateAlcoholCount
class UpdateAlcoholCountCommand : public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> update_alcohol_count_command(std::map<std::string, std::string> &data);
};

// FindUserId
class FindUserIdCommand : public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> find_user_id_command(std::map<std::string, std::string> &data);
};

// FindUserPasswd
class FindUserPasswdCommand : public Command {
	public:
		std::map<std::string, std::string> execute_command(std::map<std::string, std::string> &data) override;
		std::map<std::string, std::string> find_user_passwd_command(std::map<std::string, std::string> &data);
};

#endif
