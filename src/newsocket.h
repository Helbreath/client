#pragma once

#include <asio/ssl.hpp>
#include <asio/ip/tcp.hpp>
#include <array>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include "request_handler.h"
#include "streams.h"

class CGame;
class request_handler;

/// Represents a single connection from a client.
class connection : public std::enable_shared_from_this<connection>
{
public:
    /// Construct a connection with the given io_service.
    explicit connection(asio::io_context & io_service,
        CGame & client, request_handler & handler, asio::ssl::context & context);

    /// Get the socket associated with the connection.
    asio::ssl::stream<asio::ip::tcp::socket>::lowest_layer_type & socket()
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
    void handle_read_header(const asio::error_code & e,
        std::size_t bytes_transferred);
    void handle_read(const asio::error_code & e,
        std::size_t bytes_transferred);
    /// Handle completion of a write operation.
    void handle_write(const asio::error_code & e);

    void handle_handshake(const asio::error_code & error);

    /// Socket for the connection.
    //boost::asio::ip::tcp::socket socket_;
    asio::ssl::stream<asio::ip::tcp::socket> socket_;

    /// The manager for this connection.
    CGame & client;

    /// The handler used to process the incoming request.
    request_handler & request_handler_;

    /// Buffer for incoming data.
    std::array<char, 8192> buffer_; //2048

    /// The incoming request.
    request request_;

    int32_t size;

    bool handshake_complete;
};

using connection_ptr = std::shared_ptr<connection>;
