#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include "common.hpp"
#include "command.hpp"
#include "json_manager.hpp"
#include "mysql_conn.hpp"
#include "data_mapper.hpp"

class Socket{
	public:
		Socket();
		~Socket();
		int sockfd;
		struct sockaddr_in sin;

		int init_socket(int port_number);
};

class Server : public Socket{
	public:
		Server();
		Server(int port);
		~Server();

		int epfd;
		char buffer[4096];
		enum { MAX_EVENTS = 32 };
		struct epoll_event ev;
		struct epoll_event events[MAX_EVENTS];

		JsonManager *json_manager;
		DataMapper *data_mapper;

		std::map<int, Command *> command_map;

		int init_server(void);
		int run_server(void);
		int stop_server(void);
		int init_command(void);
		int set_nonblocking(int sock_fd);
		int add_new_client(int client_fd);
		int respones_to_client(int client_fd);
		std::map<std::string, std::string> process_transaction(std::map<std::string, std::string> &data);
		char *make_response(std::map<std::string, std::string> request_map);
};

int start_server(int argc, char *argv[]);
int parse_args(int argc, char *argv[]);

#endif
