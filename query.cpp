#include "query.hpp"

std::string WS				= " ";
std::string SELECT  			= "SELECT" + WS;
std::string UPDATE			= "UPDATE" + WS;
std::string INSERT			= "INSERT" + WS;
std::string SET				= "SET" + WS;
std::string ALL				= "*" + WS;
std::string INTO			= "INTO" + WS;
std::string FROM			= "FROM" + WS;
std::string VALUES			= "VALUES" + WS;
std::string WHERE			= "WHERE" + WS;
std::string COUNT			= "COUNT" + WS;
std::string COUNT_ALL			= "COUNT(*)" + WS;
std::string AND				= "AND" + WS;

std::string EQUAL			= "=";
std::string QUOT			= "\'";
std::string DQUOT			= "\"";
std::string COMMA			= "," + WS;
std::string SEMICOL			= ";";
std::string LPARENS			= "(";
std::string RPARENS			= ")";

std::string MEMBER_TABLE		= "member" + WS;
std::string PRIMARY_KEY			= "idx" + WS;
std::string ID_COLUMN			= "id" + WS;
std::string PW_COLUMN			= "passwd" + WS;
std::string NAME_COLUMN			= "name" + WS;
std::string TEL_COLUMN			= "tel" + WS;
std::string DRIVE_CNT_COLUMN		= "drive_cnt" + WS;
std::string LAST_DRIVE_DATE_COLUMN	= "last_drive_date" + WS;
std::string SLEEP_DETECT_CNT_COLUMN	= "sleep_detect_cnt" + WS;
std::string ALCOHOL_DETECT_CNT_COLUMN	= "alcohol_detect_cnt" + WS;

std::string CAPTCHA_TABLE		= "captcha" + WS;
std::string CAPTCHA_TABLE2		= "captcha2" + WS;
std::string CAPTCHA_NUM			= "captcha_num" + WS;
std::string CAPTCHA_ANSWER		= "captcha_answer" + WS;
