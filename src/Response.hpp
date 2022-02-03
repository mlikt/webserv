#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "HeaderState.hpp"
#include "Request.hpp"



class Response {
	private:
		std::string response;
		std::string URI;
		std::string responseHeaders;
		std::string responseBody;
		size_t contentLength;
		std::ifstream *file;
		std::string chunkBuf;
		HeaderState headerState;
	public:
		std::string fullResponse;
		Response();
		~Response();
		void formResponse(std::string staticPageFolder);
		bool SetURI(const std::string & URI);
		void FormHeaders(std::string staticPageFolder);
		std::string GetResponse();
		void FormResponseChunk();
		void SetHeaderState(HeaderState state);
		std::string GetChunkBuf();
};
#endif
