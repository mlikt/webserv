#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <dirent.h>

#include "ConnectedNode.hpp"

#define LIMITE_SIZE 8192U

#define PAYLOAD_TOO_LARGE 413
#define REQUEST_URI_TOO_LARGE  414

class ConnectedNode;

class Request {
	private:
		// Полный запрос
		std::string request;
		// Тело запроса для POST
		std::string bufBody;
		
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
		Request():LimitRequestFieldSize(LIMITE_SIZE), recvByteRequest(0) {};
		~Request() {};

		int PutNextChunk(const std::string &chunk, ConnectedNode &node);
		
		int checkReqPath();
		int parseStartLine(void);
};

#endif
