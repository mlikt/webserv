#include "ParseConfig.hpp"

void ParsingServerNode(t_server &server, std::string &serverNode) {

}

void InitServerStruct(t_server &t) {
	t.serverName = "";
	t.port = 0;
}

size_t FindNextPosition(const std::string &buf) {
	size_t pos, from = 0;
	size_t i = 0;

	while(i < 2) {
		pos = buf.find("server:", from);
		if (std::string::npos == pos) {
			break ;
		}
		from = pos + 1;
		i++;
	}
	return (pos);
}

t_server StartParsingBuffer(std::string &buf) {
	t_server server;
	InitServerStruct(server);

	std::string serverNode;
	size_t positionNextServer = 0;

	if ((positionNextServer = FindNextPosition(buf))) {
		serverNode = buf.substr(0, positionNextServer);
//		std::cout << "server node\n" <<  serverNode << std::endl;
		ParsingServerNode(server, serverNode);
		buf.erase(0, positionNextServer);
	}
	else {
		serverNode = buf.substr(0, buf.length());
		buf.erase(0, buf.length());
	}
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