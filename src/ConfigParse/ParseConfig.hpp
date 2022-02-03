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
	std::string								root;
	std::multimap<std::string, std::string>	cgi;
	bool									cgi_status;

	std::string	getRoot() const { return root; }
	std::string	getPath() const { return path; }
	bool	getStatus() const {return cgi_status; }
	const std::multimap<std::string, std::string>&	getCgi() const { return cgi; }

};

typedef struct	s_server {
	std::string 							index;
	std::string								serverName;
	int										port;
	std::string								errorPages;
	std::string								root;
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

void	strTrim(std::string &value);
#endif
