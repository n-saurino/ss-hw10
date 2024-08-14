#pragma once
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string MakeDatetimeString(){
    time_t now = time(0);
    return ctime(&now);
}

// Asynchronous Server Class
class AsyncServer
{
private:
    /* data */
public:
    AsyncServer(/* args */);
    ~AsyncServer();
};

AsyncServer::AsyncServer(/* args */)
{
}

AsyncServer::~AsyncServer()
{
}

// TCP Connection Class
class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
    typedef std::shared_ptr<TCPConnection> pointer;
private:
    /* data */
    tcp::socket socket_;
    std::string message_;
    void HandleWrite(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/);
public:
    TCPConnection(boost::asio::io_context& io);
    ~TCPConnection();
    static pointer TCPConnection::Create(boost::asio::io_context& io){
        return pointer(new TCPConnection(io));
    }

    tcp::socket& Socket();
    void Start();
    
};

TCPConnection::TCPConnection(boost::asio::io_context& io) : socket_(io)
{
}

TCPConnection::~TCPConnection()
{
}

void TCPConnection::HandleWrite(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/){

}

tcp::socket& TCPConnection::Socket(){
    return socket_;
}

void TCPConnection::Start(){
    message_ = MakeDatetimeString();
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        std::bind(&TCPConnection::HandleWrite, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
}

class TCPServer
{
private:
    /* data */
public:
    TCPServer(/* args */);
    ~TCPServer();
};

TCPServer::TCPServer(/* args */)
{
}

TCPServer::~TCPServer()
{
}


