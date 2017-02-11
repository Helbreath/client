#pragma once


#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/shared_lock_guard.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>
#include "request_handler.h"
#include "streams.h"
#include <boost/asio/ssl.hpp>

class CGame;
class request_handler;



/// Represents a single connection from a client.
class connection
	: public boost::enable_shared_from_this<connection>,
	private boost::noncopyable
{
public:
	/// Construct a connection with the given io_service.
	explicit connection(boost::asio::io_service& io_service,
		CGame & client, request_handler& handler, boost::asio::ssl::context& context);

	/// Get the socket associated with the connection.
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::lowest_layer_type& socket()
    {
        return socket_.lowest_layer();
    }
    //boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket();

	/// Start the first asynchronous operation for the connection.
	void start();

	/// Stop all asynchronous operations associated with the connection.
	void stop();

	void write(const char * data, const uint64_t size);
	void write(StreamWrite & sw);

	/// Handle completion of a read operation.
	void handle_read_header(const boost::system::error_code& e,
		std::size_t bytes_transferred);
	void handle_read(const boost::system::error_code& e,
		std::size_t bytes_transferred);
	/// Handle completion of a write operation.
	void handle_write(const boost::system::error_code& e);

    void handle_handshake(const boost::system::error_code& error);

	/// Socket for the connection.
	//boost::asio::ip::tcp::socket socket_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;

	/// The manager for this connection.
	CGame & client;

	/// The handler used to process the incoming request.
	request_handler& request_handler_;

	/// Buffer for incoming data.
	boost::array<char, 8192> buffer_;//2048

	/// The incoming request.
	request request_;

	int32_t size;

    bool handshake_complete;
};

typedef boost::shared_ptr<connection> connection_ptr;
