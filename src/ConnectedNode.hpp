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
		void PutNextChunkRequest(std::string chunk);
		void CreateRequest();
		void CreateResponse();
		State GetConnectState() const ;
		void SetConnectState(const State state);
		void FormResponseTest();


};
#endif