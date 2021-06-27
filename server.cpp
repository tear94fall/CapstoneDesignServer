#include "server.hpp"

int start_server(int argc, char *argv[]){
	int port = parse_args(argc, argv);
	if(port == -1)
		port = 8888;

	std::unique_ptr<Server> server(new Server(port));

	if(server->init_server() == -1){ return -1; }
	if(server->run_server() == -1){ return -1; }
	if(server->stop_server() == -1){ return -1; }

	return 0;
}

int parse_args(int argc, char *argv[]){
	int port_number = -1;

	if(argc==2){
		port_number = atoi(argv[1]);
		if(port_number < 0 || port_number > 65536){
			printf("Invalid Port Number : %s\n", argv[1]);
			port_number = -1;
		}
	}else if(argc>2){
		printf("Invalid args...\n");
	}

	return port_number;
}

// Socket
Socket::Socket(){
	memset(&sin, 0x00, sizeof(sin));
}

Socket::~Socket(){
	close(sockfd);
}

int Socket::init_socket(int port_number){
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port_number);

	if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0){
		return -1;
	}

	if(bind(sockfd, (struct sockaddr*) &sin, sizeof(sin))<0){
		return -1;
	}

	const int BACKLOG = 256;

	if(listen(sockfd, BACKLOG)<0){
		return -1;
	}

	int opt_val = true;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));
	setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &opt_val, sizeof(opt_val));
	setsockopt(sockfd, IPPROTO_TCP, TCP_QUICKACK, &opt_val, sizeof(opt_val));

	return 1;
}

// Server
Server::Server(){
	init_socket(8888);
}

Server::Server(int port){
	init_socket(port);
}

Server::~Server(){
	close(epfd);
}

int Server::init_server(void){
	init_command();
	json_manager = new JsonManager();
	data_mapper = new DataMapper();

	int result = 0;
	result = MYSQL_CONN.mysql_conf_file_read("db.conf");
	if(result == -1){
		return -1;
	}

	result = MYSQL_CONN.mysql_conn_init();
	if(result == -1){
		return -1;
	}

	set_nonblocking(sockfd);

	epfd = epoll_create(MAX_EVENTS);
	if(epfd<0){
		return -1;
	}

	memset(&ev, 0x00, sizeof(ev));
	ev.events=EPOLLIN;
	ev.data.fd = sockfd;

	epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

	return 1;
}

int Server::run_server(void){
	while(1){
		int nfd = epoll_wait(epfd, events, MAX_EVENTS, -1);

		for(int i=0;i<nfd;i++){
			int cli_fd = events[i].data.fd;
			if(cli_fd == sockfd){
				add_new_client(cli_fd);
			}else{
				respones_to_client(cli_fd);
			}
		}
	}

	return 1;
}

int Server::stop_server(void){
	MYSQL_CONN.mysql_conn_stop();
	return 1;
}

int Server::init_command(void){
	command_map.insert(std::pair<int, Command *>(CMD_ECHO, new EchoCommand()));
	command_map.insert(std::pair<int, Command *>(CMD_LOGIN, new LoginCommand()));
	command_map.insert(std::pair<int, Command *>(CMD_ID_CHECK, new IdCheckCommand()));
	command_map.insert(std::pair<int, Command *>(CMD_CREATE_ACCOUNT, new CreateAccountCommand()));
	command_map.insert(std::pair<int, Command *>(CMD_GET_CAPTCHA, new GetCaptchaTestCaseCommand()));
	command_map.insert(std::pair<int, Command *>(CMD_GET_LAST_DRIVE_DATE, new GetLastDriveDateCommand()));
	command_map.insert(std::pair<int, Command *>(CMD_GET_CAPTCHA2, new GetCaptcha2TestCaseCommand()));
	command_map.insert(std::pair<int, Command *>(CMD_GET_USER_INFO, new GetUserInfoCommand()));
	command_map.insert(std::pair<int, Command *>(CMD_GET_ALL_USER_INFO, new GetAllUserInfoCommand()));
	command_map.insert(std::pair<int, Command *>(CMD_UPDATE_USER_INFO, new UpdateUserInfoCommand()));
	command_map.insert(std::pair<int, Command *>(CMD_UPDATE_LAST_DRIVE_DATE, new UpdateLastDriveDateCommand()));
	command_map.insert(std::pair<int, Command *>(CMD_UPDATE_ALCOHOL_COUNT, new UpdateAlcoholCountCommand()));
	command_map.insert(std::pair<int, Command *>(CMD_FIND_USER_ID, new FindUserIdCommand()));
	command_map.insert(std::pair<int, Command *>(CMD_FIND_USER_PASSWD, new FindUserPasswdCommand()));

	return 1;
}

int Server::set_nonblocking(int sock_fd){
	int flag = fcntl(sock_fd, F_GETFL, 0);
	fcntl(sock_fd, F_SETFL, flag | O_NONBLOCK);

	return 1;
}

int Server::add_new_client(int client_fd){
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	int client_sock = accept(sockfd, (struct sockaddr *) &client_addr, &client_addr_len);
	if(client_sock < 0){
		return -1;
	}

	set_nonblocking(client_sock);
	memset(&ev, 0x00, sizeof(ev));
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = client_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &ev);

	return 1;
}

int Server::respones_to_client(int client_fd){
	int rc = 1;
	memset(buffer, 0x00, sizeof(buffer));
	int n = read(client_fd, buffer, sizeof(buffer));
	if(n<=0){
		epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, &ev);
		close(client_fd);
		rc = -1;
	}else{
		char *recv_data = NULL, *send_data = NULL;
		std::map<std::string, std::string> recv_map, result_map;

		recv_data = trim(buffer);
		recv_data = remove_square_bracket(recv_data);
		recv_map = json_manager->parse_json(recv_data);

		result_map = process_transaction(recv_map);
		send_data = make_response(result_map);
		write(client_fd, send_data, strlen(send_data));

		free(send_data);
		recv_map.clear();
		result_map.clear();
	}

	return rc;
}

std::map<std::string, std::string> Server::process_transaction(std::map<std::string, std::string> &data){
	std::map<std::string, std::string>::iterator data_iter;
	std::string command_number;
	std::stringstream number;
	int cmd_num;

	data_iter = data.find("request_number");
	if(data_iter != data.end()){
		command_number = data_iter->second;
		// data.erase(data_iter++);
	}

	cmd_num = string_to_int(command_number);

	std::map<std::string, std::string> result_map;
	std::map<int, Command *>::iterator command_iter;
	command_iter = command_map.find(cmd_num);
	if(command_iter != command_map.end()){
		result_map = command_iter->second->execute_command(data);
	}

	return result_map;
}

char *Server::make_response(std::map<std::string, std::string> request_map){
	std::map<std::string, std::string>::iterator request_map_iter;
	std::string command_number;
	int cmd_num = 0;

	char *response_data = NULL;
	std::string data;

	request_map_iter = request_map.find("request_number");
	if(request_map_iter != request_map.end()){
		command_number = request_map_iter->second;
		cmd_num = string_to_int(command_number);
	}

	if(request_map.size() == 0 || cmd_num == 0){
		response_data = strdup("false");
		return response_data;
	}

	switch(cmd_num){
		case CMD_ECHO:
			data = json_manager->make_json(request_map);
			break;
		case CMD_LOGIN:
		case CMD_ID_CHECK:
		case CMD_CREATE_ACCOUNT:
		case CMD_GET_CAPTCHA2:
		case CMD_UPDATE_USER_INFO:
		case CMD_UPDATE_LAST_DRIVE_DATE:
			data = request_map["return_code"];
			break;
		case CMD_GET_CAPTCHA:
			data = request_map["captcha_answer"];
			break;
		case CMD_GET_LAST_DRIVE_DATE:
			data = request_map["last_drive_date"];
			break;
		case CMD_GET_USER_INFO:
			data = request_map["drive_info"];
			break;
		case CMD_GET_ALL_USER_INFO:
			data = request_map["user_info"];
			break;
		case CMD_FIND_USER_ID:
			data = request_map["userid"];
			break;
		case CMD_FIND_USER_PASSWD:
			data = request_map["userpw"];
			break;
		default:
			data = strdup("false");
			break;
	}

	response_data = strdup(data.c_str());
	return response_data;
}
