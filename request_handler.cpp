

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

	if (c.client.gamemode == 1)
		c.client.GameRecvMsgHandler(req.size, req.data);
	else
		c.client.LogRecvMsgHandler(req.data);
}
