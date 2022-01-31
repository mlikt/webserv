#include "Request.hpp"
std::vector<std::string> split (const std::string &s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss (s);
	std::string item;

	while (std::getline (ss, item, delim)) {
		result.push_back (item);
	}

	return result;
}

int Request::PutNextChunk(const std::string &chunk, ConnectedNode &node) 
{
	if (node.GetConnectState() == ConnectedNode::RecvRequest)
	{
		// Внимание!!! Сейчас добавляется не совсем верно
		// возможно chunk идет вместе с телом, верно будет добавлять размерность,
		// после отсечения конца запроса тоесть двойного \r\n\r\n
		this->recvByteRequest += chunk.size();

		if (this->recvByteRequest > this->LimitRequestFieldSize)
		{
			node.SetConnectState(ConnectedNode::Error);
			return (REQUEST_URI_TOO_LARGE);
		}
	}
	else if (node.GetConnectState() == ConnectedNode::RecvBodyMessage)
	{

	}
	
	return (0);
}

int Request::parseStartLine() {
	size_t occurrence = this->request.find("\r\n", 0);
	if (occurrence == std::string::npos)
		return -1;
	std::string firstLine = this->request.substr(0, occurrence);
	std::vector<std::string>dividedFirstLine = split(firstLine, ' ');
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

int Request::checkReqPath() {
	DIR *dir;
	dir = opendir(("./pages" + this->URI).c_str());
	if (!dir) {
		std::cout << "NO DIR" << std::endl;
		return -1;
	}
	this->staticPageFolder = "./pages" + this->URI;
	return 1;
}
