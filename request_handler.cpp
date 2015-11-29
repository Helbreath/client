

#include "request_handler.h"
#include "newsocket.h"
#include "Game.h"

request_handler::request_handler()
{
}

void request_handler::handle_request(const request& req)
{
	connection & c = *req.connection;
	//client->lastpackettime = timestamp;

	shared_ptr<CGame::MsgQueueEntry> msg(new CGame::MsgQueueEntry);
	msg->data = new char[req.size];
	memcpy(msg->data, req.data, req.size);
	msg->size = req.size;
	c.client.PutMsgQueue(msg, c.client.socketpipe);
}
