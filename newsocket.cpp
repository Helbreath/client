
#include "newsocket.h"
#include <vector>
#include <boost/bind.hpp>
#include "../Game.h"
#include "request_handler.h"
#include "../streams.h"

connection::connection(boost::asio::io_service& io_service,
	CGame& client, request_handler& handler)
	: socket_(io_service),
	client(client),
	request_handler_(handler)
{
	size = 0;
}

boost::asio::ip::tcp::socket& connection::socket()
{
	return socket_;
}

void connection::start()
{
	boost::asio::async_read(socket_, boost::asio::buffer(buffer_, 3), boost::bind(&connection::handle_read_header, shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void connection::stop()
{
	socket_.close();
	if (client._socket)
		client._socket.reset();
}

void connection::write(const char * data, const uint64_t size)
{
	std::lock_guard<std::mutex> lock(mutsocket);
	char ckey[1] = { 0 };
	char csize[2];
	try {
		uint16_t tsize = uint16_t(size) + 3;
		*(int16_t*)csize = tsize;
		std::array<boost::asio::const_buffer, 3> buffers = {
			boost::asio::buffer(ckey, 1),
			boost::asio::buffer(csize, 2),
			boost::asio::buffer(data, size)
		};
		boost::asio::write(socket_, buffers);
	}
	catch (std::exception& e)
	{
		printf("asio::write_some() exception: %s", e.what());
	}
}

void connection::write(StreamWrite & sw)
{
	std::lock_guard<std::mutex> lock(mutsocket);
	char ckey[1] = { 0 };
	char csize[2];
	try {
		uint16_t tsize = uint16_t(sw.size) + 3;
		*(int16_t*)csize = tsize;
		std::array<boost::asio::const_buffer, 3> buffers = {
			boost::asio::buffer(ckey, 1),
			boost::asio::buffer(csize, 2),
			boost::asio::buffer(sw.data, sw.size)
		};
		boost::asio::write(socket_, buffers);
	}
	catch (std::exception& e)
	{
		printf("asio::write_some() exception: %s", e.what());
	}
}

void connection::handle_read_header(const boost::system::error_code& e,
	std::size_t bytes_transferred)
{
	std::lock_guard<std::mutex> lock(mutsocket);
	//printf("bytes_transferred head: %d\n", bytes_transferred);
	if (!e)
	{
		if (bytes_transferred == 3)
		{
			//TODO: helbreath's ghetto xor key needs to go. first bytes = size or gtfo
			size = *(int16_t*)(((char*)buffer_.data()) + 1);
			if (size > 2048)//temporary set size .. shouldn't really be more than this anyway
			{
				printf("Invalid packet size : %d", size);
				client.stop(shared_from_this());
				return;
			}
			boost::asio::async_read(socket_, boost::asio::buffer(buffer_, size - 3), boost::bind(&connection::handle_read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}
	}
	else if (e != boost::asio::error::operation_aborted)
	{
		client.stop(shared_from_this());
		return;
	}

}

void connection::handle_read(const boost::system::error_code& e,
	std::size_t bytes_transferred)
{
	std::lock_guard<std::mutex> lock(mutsocket);
	//printf("bytes_transferred body: %d - expected: %d\n", bytes_transferred, size);
	if (!e)
	{
		if (bytes_transferred != size - 3)
		{
			printf("Did not receive proper amount of bytes : rcv: %d needed: %d", bytes_transferred, size);
			client.stop(shared_from_this());
			return;
		}
		//printf("uid("XI64")\n", uid);
		// read object size
		if ((size > 2048) || (size <= 0))
		{
			//ERROR - object too large - close connection
			printf("Invalid packet size : %?d", size);
			client.stop(shared_from_this());
			return;
		}

		// parse packet
		request_.size = size;
		request_.data = buffer_.data();
		request_.connection = this;
		try {
			request_handler_.handle_request(request_);
		}
		catch (std::exception& e)
		{
			printf("handle_request() exception: %s", e.what());
		}

		boost::asio::async_read(socket_, boost::asio::buffer(buffer_, 3), boost::bind(&connection::handle_read_header, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	else if (e != boost::asio::error::operation_aborted)
	{
		client.stop(shared_from_this());
		return;
	}
}

void connection::handle_write(const boost::system::error_code& e)
{
	std::lock_guard<std::mutex> lock(mutsocket);
	if (!e)
	{
		// 		// Initiate graceful connection closure.
		// 		asio::error_code ignored_ec;
		// 		socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
	}

	if (e != boost::asio::error::operation_aborted)
	{
		client.stop(shared_from_this());
	}
}


