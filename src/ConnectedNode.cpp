#include "ConnectedNode.hpp"

ConnectedNode::ConnectedNode(const ConnectedNode::State state)
:request(nullptr), response(nullptr), state(state)
{}

ConnectedNode::~ConnectedNode()
{
	delete request;
	delete response;
}

void ConnectedNode::CreateRequest()
{
	if (this->request == nullptr && state == ConnectedNode::Create)
	{
		this->request = new Request();
		state = ConnectedNode::RecvRequest;
	}
}

void ConnectedNode::CreateResponse()
{
	this->response = new Response();
}
void ConnectedNode::PutNextChunkRequest(const std::string &chunk)
{
	this->request->PutNextChunk(chunk, *this);
}

ConnectedNode::State ConnectedNode::GetConnectState() const 
{
	return (state);
}

void ConnectedNode::SetConnectState(const State state)
{
	this->state = state;
}

void ConnectedNode::FormResponseTest() {
	this->response->formResponse(this->request->GetStaticPageFolder());
}

