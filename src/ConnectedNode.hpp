#ifndef CONNECTNODE_HPP
#define CONNECTNODE_HPP

#include "Request.hpp"
#include "Response.hpp"
class Response;
class Request;

class ConnectedNode
{
	public:
	// Для определения состояния соедененого узла
		enum State 
		{
			Create,
			RecvRequest,
			RecvBodyMessage,
			SendResponse,
			SendBodyMessage,
			ReRecvRequest,
			Closed,
			Error
		};

	private:
		Request		*request;
		Response	*response;

	// Определяет состоянеие соединенеия
		State		state;
	
	// Запрещаем копировать
		ConnectedNode(const ConnectedNode& node);
	public:
		ConnectedNode(const ConnectedNode::State state);
		~ConnectedNode();
		int PutNextChunkRequest(std::string chunk);
		void CreateRequest();
		void CreateResponse();
		State GetConnectState() const ;
		void SetConnectState(const State state);
		void FormResponseTest();
		std::string GetStaticPagePath();
		void FormHeaders();
		std::string GetResponse();
		void SetResponseHeaderState(HeaderState  state);
		void FormResponseChunk();
		std::string GetResponseChunkBuf();


};
#endif