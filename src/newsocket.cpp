
#include "newsocket.h"
#include <vector>
#include <functional>
//#include <boost/bind.hpp>
#include <asio/io_context.hpp>
#include "Game.h"
#include "request_handler.h"
#include "streams.h"
#include <asio/placeholders.hpp>
#include <asio/read.hpp>

connection::connection(asio::io_context & io_service,
    CGame & client, request_handler & handler, asio::ssl::context & context)
    : socket_(io_service, context),
    client(client),
    request_handler_(handler)
{
    size = 0;
    handshake_complete = false;
}

void connection::start()
{
    client.m_dwCheckSprTime = unixtime() + 1000;

    asio::error_code ec;
    socket_.handshake(asio::ssl::stream_base::client, ec);

    handshake_complete = true;

    if (!ec)
    {
        asio::async_read(socket_, asio::buffer(buffer_, 2), std::bind(&connection::handle_read_header, shared_from_this(),
            std::placeholders::_1,
            std::placeholders::_2));
    }
    else
    {
        //error
        printf("connection exception: %s\n", ec.message().c_str());
        stop();
        client.ChangeGameMode(GAMEMODE_ONMAINMENU);
    }
}

void connection::handle_handshake(const asio::error_code & error)
{
    if (!error)
    {
        asio::async_read(socket_, asio::buffer(buffer_, 2), std::bind(&connection::handle_read_header, shared_from_this(),
            std::placeholders::_1,
            std::placeholders::_2));
    }
    else
    {
        client.stop(shared_from_this());
    }
}

void connection::stop()
{
    try
    {
        socket_.lowest_layer().close();
        //client._socket.reset();
    }
    catch (std::exception & e)
    {
        printf("connection::stop() exception: %s", e.what());
    }
}

void connection::write(const char * data, const uint64_t size)
{
    char csize[2];
    try {
        uint16_t tsize = uint16_t(size);
        *(uint16_t *)csize = tsize;
        std::array<asio::const_buffer, 2> buffers = {
            asio::buffer(csize, 2),
            asio::buffer(data, size)
        };
        asio::async_write(socket_, buffers, [this](asio::error_code ec, std::size_t) {
            if (ec)
            {
                //error
            }
        });
    }
    catch (std::exception & e)
    {
        printf("asio::write_some() exception: %s", e.what());
    }
}

void connection::write(StreamWrite & sw)
{
    printf("asio::write() data\n");
    char ckey[1] = { 0 };
    char csize[2];
    try {
        uint16_t tsize = uint16_t(sw.position);
        *(uint16_t *)csize = tsize;
        /*
                shared_ptr<StreamWrite> tmppacket = make_shared<StreamWrite>();
                tmppacket->WriteBytes(sw.data, sw.position);
                std::array<boost::asio::const_buffer, 2> buffers = {
                    boost::asio::buffer(csize, 2),
                    boost::asio::buffer(tmppacket->data, tmppacket->position)
                };
                boost::asio::async_write(socket_, buffers, [this, tmppacket](boost::system::error_code ec, std::size_t){
                    if (ec)
                    {
                        //error
                    }
                });*/
        std::array<asio::const_buffer, 2> buffers = {
            asio::buffer(csize, 2),
            asio::buffer(sw.data, sw.position)
        };
        if (socket_.lowest_layer().is_open())
            asio::write(socket_, buffers);
    }
    catch (std::exception & e)
    {
        printf("asio::write_some() exception: %s", e.what());
    }
}

void connection::handle_read_header(const asio::error_code & e,
    std::size_t bytes_transferred)
{
    if (!e)
    {
        if (bytes_transferred == 2)
        {
            size = *(int16_t *)((char *)buffer_.data());
            if (size > 8192 || size <= 0)//temporary set size .. shouldn't really be more than this anyway//2048
            {
                printf("Invalid packet size : %d", size);
                client.stop(shared_from_this());
                return;
            }
            asio::async_read(socket_, asio::buffer(buffer_, size), std::bind(&connection::handle_read, shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2));
        }
    }
    else if (e != asio::error::operation_aborted)
    {
        client.stop(shared_from_this());
        return;
    }

}

void connection::handle_read(const asio::error_code & e,
    std::size_t bytes_transferred)
{
    if (!e)
    {
        if (bytes_transferred != size)
        {
            printf("Did not receive proper amount of bytes : rcv: %d needed: %d", (int32_t)bytes_transferred, size);
            client.stop(shared_from_this());
            return;
        }
        //printf("uid("XI64")\n", uid);
        // read object size
        if ((size > 8192) || (size <= 0))//2048
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
        request_handler_.handle_request(request_);

        asio::async_read(socket_, asio::buffer(buffer_, 2), std::bind(&connection::handle_read_header, shared_from_this(),
            std::placeholders::_1,
            std::placeholders::_2));
    }
    else if (e != asio::error::operation_aborted)
    {
        client.stop(shared_from_this());
        return;
    }
}

void connection::handle_write(const asio::error_code & e)
{
    if (!e)
    {
        // 		// Initiate graceful connection closure.
        // 		asio::error_code ignored_ec;
        // 		socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
    }

    if (e != asio::error::operation_aborted)
    {
        client.stop(shared_from_this());
    }
}


