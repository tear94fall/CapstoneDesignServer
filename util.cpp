#include "util.hpp"

char *ltrim(char *s){
	while(isspace(*s)) s++;
	return s;
}

char *rtrim(char *s){
	char* back = s + strlen(s);
	while(isspace(*--back));
	*(back+1) = '\0';
	return s;
}

char *trim(char *s){
	return rtrim(ltrim(s));
}

char *remove_square_bracket(char *s){
	if(s[0]=='['){
		s++;
	}
	if(s[strlen(s)-1]==']'){
		s[strlen(s)-1]='\0';
	}

	return s;
}

template <typename T>
std::string ToString(T type){
	std::stringstream ss;
	ss << type;
	return ss.str();
}

int string_to_int(std::string str){
	std::stringstream ss;
	ss << str;
	int number;
	ss >> number;

	return number;
}

std::string get_date_string(){
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	std::stringstream ss;
	time_t tt = std::chrono::system_clock::to_time_t(now);
	struct tm lt;

	localtime_r(&tt, &lt);
	ss << lt.tm_year + 1900 << "-" << lt.tm_mon + 1 << "-" << lt.tm_mday;

	return ss.str();
}

std::string make_random_number(void){
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> dis(0, 100);

	return std::to_string(dis(gen) % 9);
}

std::string day_between_from_two_days(std::string start, std::string end){
	time_t s_time, d_time;
	struct tm s_tm, d_tm;
	struct tm *s_timeinfo = &s_tm, *d_timeinfo = &d_tm;
	double diff;

	std::string s_year, d_year, s_mon, d_mon, s_day, d_day;

	s_year = start.substr(0, 4);
	s_mon = start.substr(5, 2);
	s_day = start.substr(8, 2);

	d_year = end.substr(0, 4);
	d_mon = end.substr(5, 2);
	d_day = end.substr(8, 2);

	s_timeinfo->tm_year = string_to_int(s_year)-1900;
	s_timeinfo->tm_mon = string_to_int(s_mon)-1;
	s_timeinfo->tm_mday = string_to_int(s_day);
	s_timeinfo->tm_hour = 0;
	s_timeinfo->tm_min = 0;
	s_timeinfo->tm_sec = 0;
	s_timeinfo->tm_isdst = 0;
	s_time = mktime(s_timeinfo);

	d_timeinfo->tm_year = string_to_int(d_year)-1900;
	d_timeinfo->tm_mon = string_to_int(d_mon)-1;
	d_timeinfo->tm_mday = string_to_int(d_day);
	d_timeinfo->tm_hour = 0;
	d_timeinfo->tm_min = 0;
	d_timeinfo->tm_sec = 0;
	d_timeinfo->tm_isdst = 0;
	d_time = mktime(d_timeinfo);

	diff = difftime(d_time, s_time);

	int btw_day = diff / (60 * 60 * 24);

	return std::to_string(btw_day);
}
