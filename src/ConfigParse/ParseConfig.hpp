#ifndef WEBSERV_PARSECONFIG_HPP
#define WEBSERV_PARSECONFIG_HPP

#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <cstdint>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

struct Location {
	std::string								path;
	std::map<std::string, bool>				methods;
	std::string								root;
	std::multimap<std::string, std::string>	cgi;
	std::string								index;
	std::string								autoIndex;
	int										limitClientBodySize;
	std::pair<int, std::string>				redirect;

	std::string	getRoot() const { return root; }
	const std::multimap<std::string, std::string>&	getCgi() const { return cgi; }
	std::string	getIndex() const { return index; }
	std::string	getAutoIndex() const { return autoIndex; }
	int	getLimit() const { return limitClientBodySize; }
	std::map<std::string, bool>	getMethods() const { return methods; }
	const std::pair<int, std::string>&	getRedire() const { return redirect; }
};

typedef struct	s_server {
	std::string								serverName;
	std::string								hostName;
	int										port;
	std::string 							logFile;
	std::map<int, std::string>				errorPages;
	std::multimap<std::string, Location>	locations;
}	t_server;


class ErrorException : public std::exception {

public :

	ErrorException(const char *msg) : errorMsg(msg), status(0) {}
	ErrorException(int st, const char *msg) : errorMsg(msg), status(st) {}
	int getStatus() const {return status;}
	virtual const char* what(void) const throw () { return (this->errorMsg); }

private:

	const char*	errorMsg;
	const int	status;
};

#endif //WEBSERV_PARSECONFIG_HPP
