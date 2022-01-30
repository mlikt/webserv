#include "ParseConfig.hpp"

t_server parsBuf(std::string &buf) {
	t_server server;
	//init struct
	return (server);
}

int StartParseConfig(char *fileName) {
	std::vector<t_server> ServerStruct;

	std::ifstream	parsFile(fileName);
	std::string		buf;
	std::string		bufFile;

	if(parsFile.is_open()) {
		std::cout << "Parser start ebnvrt\n" << std::endl;
		while(std::getline(parsFile, buf)) {
			bufFile += buf + "\n";
		}
		while (bufFile.find("server:") != std::string::npos) {
			t_server serv = parsBuf(bufFile);
			servers.push_back(serv);
		}
	}
	else if (!parsFile.is_open()) {
		return (1);
	}
	parsFile.close();
	return(0);
}