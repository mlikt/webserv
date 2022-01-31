#include "ParseConfig.hpp"

void InitServerStruct(t_server &t) {
	t.serverName = "";
	t.hostName = "";
	t.port = 0;
	t.logFile = "";
}

t_server StartParsingBuffer(std::string &buf) {
	t_server server;
	InitServerStruct(server);

	std::string elementServer;
	size_t positionNextServer;

	if ()
	return (server);
}

int StartParseConfig(char *fileName) {
	std::vector<t_server> ServerStruct;

	std::ifstream	parsFile(fileName);
	std::string		buf;
	std::string		bufFile;

	if(parsFile.is_open()) {
		std::cout << "Parser start ebnvrt)))\n" << std::endl;
		while(std::getline(parsFile, buf)) {
			bufFile += buf + "\n";
		}
		while (bufFile.find("server:") != std::string::npos) {
			t_server serv = StartParsingBuffer(bufFile);
			ServerStruct.push_back(serv);
		}
	}
	else if (!parsFile.is_open()) {
		std::cout << "Error open file\n" << std::endl;
		return (1);
	}
	parsFile.close();
	return(0);
}

int main(int argc, char **argv) {
	if(argc == 2) {
		StartParseConfig(argv[1]);
	}
	else {
		std::cout << "ARG error\n" << std::endl;
		return(1);
	}
	return(0);
}