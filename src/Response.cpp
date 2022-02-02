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
	if (staticPageFolder == "/"){
		staticPageFolder = "/index.html";
	}
	std::ifstream input ((staticPageFolder).c_str());
	buf << input.rdbuf();
	std::cout << buf << std::endl;
	this->fullResponse = START + std::to_string(buf.str().length()) + "\r\n\r\n" + buf.str();
}

void Response::FormHeaders(std::string staticPageFolder) {
	this->responseHeaders = "HTTP/1.1 200 OK\r\n"
							"Content-Type: text/html; charset=UTF-8\r\n"
							"Connection: close\r\n"
							"Content-Length: ";

	if (staticPageFolder[staticPageFolder.length() - 1] == '/'){
		staticPageFolder += "index.html";
	}
	std::ifstream *file = new std::ifstream ((staticPageFolder).c_str());
	file->seekg(0, file->end);
	size_t length = file->tellg();
	file->seekg(0, file->beg);
	this->contentLength = length;
	this->file = file;
	this->responseHeaders += std::to_string(this->contentLength) + "\r\n\r\n";
	std::cout << this->responseHeaders << std::endl;
	/*buf << input.rdbuf();
	this->responseHeaders += std::to_string(buf.str().length()) + "\r\n\r\n";
	this->response = this->responseHeaders + buf.str();*/

}

void Response::SetHeaderState(HeaderState state) {
	this->headerState = state;
}
void Response::FormResponseChunk() {
	if (this->headerState == NotSend)
	{
		char *buffer = new char [400];
		file->read(buffer, 400 - this->responseHeaders.length());
		this->chunkBuf = this->responseHeaders + buffer;
		this->contentLength = this->contentLength - 400;
		this->headerState = Sending;
		return;
	}
	this->chunkBuf.clear();
	char *buffer = new char [400];
	file->read(buffer,400);
	this->chunkBuf = buffer;
	this->contentLength -= 400;
	if (this->contentLength <= 0)
		this->headerState = Sending;
}

std::string Response::GetResponse() {
	return this->response;
}

std::string Response::GetChunkBuf() {
	return this->chunkBuf;
}

