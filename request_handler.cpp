

#include "request_handler.h"
#include "newsocket.h"
#include "Game.h"

request_handler::request_handler()
{
}

void request_handler::handle_request(const request& req)
{
	try {
		connection & c = *req.connection;
		//client->lastpackettime = timestamp;

		if (c.client.gamemode == 1)
		{
			std::lock_guard<std::mutex> lock(c.client.socketmut);
			c.client.PutMsgQueue(c.client.socketpipe, req.data, req.size);
			//c.client.GameRecvMsgHandler(req.size, req.data);
		}
		else
			c.client.LogResponseHandler(req.size, req.data);
	}
	catch (std::exception& e)
	{
		printf("handle_request() exception: %s", e.what());
	}
}
