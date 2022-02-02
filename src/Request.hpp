#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <map>

#include "headers.hpp"
#include "ConnectedNode.hpp"
#define LIMIT_SIZE 8192U
class Response;
class ConnectedNode;
#define PAYLOAD_TOO_LARGE 413
#define REQUEST_URI_TOO_LARGE  414


class Request {
	private:
		// Полный запрос
		std::string request;

		//Полученные заголовки
		std::string reqHeaders;
		std::map<std::string, std::string> headers;

		// Тело запроса для POST
		std::string bufBody;
		std::string reminder;
		
		std::string METHOD;
		std::string URI;
		std::string HTTP_PROTOCOL;

		const size_t LimitRequestFieldSize;
		size_t recvByteRequest;
		
		size_t recvByteBodyMessage;
		size_t ContentLenght;
		// Директория в которой находятся ресурсы
		std::string staticPageFolder;
	public:
		Request():LimitRequestFieldSize(LIMIT_SIZE), recvByteRequest(0) {};
		~Request() {};

		int PutNextChunk(std::string &chunk, ConnectedNode &node);
		int checkReqPath();
		int parseStartLine(void);
		std::string GetStaticPageFolder() {return this->staticPageFolder;}
		void FormAllHeaders();
};


#endif
