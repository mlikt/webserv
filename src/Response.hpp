#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Request.hpp"
#include "Response.hpp"

class Response {
	private:
		std::string response;
		std::string URI;
	
	public:
		std::string fullResponse;
		Response();
		~Response();
		void formResponse(std::string staticPageFolder);
		bool SetURI(const std::string & URI);
};
#endif
