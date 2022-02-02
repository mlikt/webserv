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
int ConnectedNode::PutNextChunkRequest(std::string chunk)
{
	return this->request->PutNextChunk(chunk, *this);
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

std::string ConnectedNode::GetStaticPagePath() {
	return this->request->GetStaticPageFolder();
}

void ConnectedNode::FormHeaders() {
	this->response->FormHeaders(this->GetStaticPagePath());
}

std::string ConnectedNode::GetResponse() {
	return this->response->GetResponse();
}

void ConnectedNode::SetResponseHeaderState(HeaderState  state) {
	this->response->SetHeaderState(state);
}

void ConnectedNode::FormResponseChunk() {
	this->response->FormResponseChunk();
}

std::string ConnectedNode::GetResponseChunkBuf(){
	return this->response->GetChunkBuf();
}

