
#pragma once

#include <string>
#include <vector>
#include <stdint.h>

struct request
{
	int32_t size;
	std::string cmd;
	std::string uri;
	char * data;
	class connection * connection;
};

/// The common handler for all incoming requests.
class request_handler
{
public:
	/// Construct with a directory containing files to be served.
	explicit request_handler();

	/// Handle a request and produce a reply.
	void handle_request(const request& req);
};
