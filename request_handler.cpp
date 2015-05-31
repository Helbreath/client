

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

	c.client.GameRecvMsgHandler(req.size, req.data);
	//c.client->PutMsgQueue(c.client->socketpipe, req.data, req.size);
}
