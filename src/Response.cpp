#include "Response.hpp"

Response::Response() {
	this->response = "";
}
Response::~Response() {}

void Response::formResponse(std::string staticPageFolder) {
	std::string START =
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html; charset=UTF-8\r\n"
			"Connection: close\r\n"
			"Content-Length: ";
	std::ostringstream buf;
	std::ifstream input ((staticPageFolder + "/index.html").c_str());
	buf << input.rdbuf();
	this->fullResponse = START + std::to_string(buf.str().length()) + "\r\n\r\n" + buf.str();
}

