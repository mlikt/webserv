#include "Request.hpp"
vector<string> split (const string &s, char delim) {
	vector<string> result;
	stringstream ss (s);
	string item;

	while (getline (ss, item, delim)) {
		result.push_back (item);
	}

	return result;
}

Request::Request(char *buf) {
	this->request = buf;
	//std::cout << "|" << this->request << "|"<< std::endl;
}

int Request::parseStartLine() {
	size_t occurrence = this->request.find("\r\n", 0);
	if (occurrence == string::npos)
		return -1;
	std::string firstLine = this->request.substr(0, occurrence);
	vector<string>dividedFirstLine = split(firstLine, ' ');
	if (dividedFirstLine.size() != 3)
		return -1;
	if (dividedFirstLine[0] != "GET" && dividedFirstLine[0] != "POST" && dividedFirstLine[0] != "DELETE")
		return -1;
	this->METHOD = dividedFirstLine[0];
	this->URI = dividedFirstLine[1];
	if (dividedFirstLine[2] != "HTTP/1.1")
		return -1;
	this->HTTP_PROTOCOL = dividedFirstLine[2];
	return 1;
}
