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

// TCP Connection Class
class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
private:
    /* data */
    tcp::socket socket_;
    std::string message_;
    void HandleWrite(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/);
public:
    typedef std::shared_ptr<TCPConnection> pointer;
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
    boost::asio::io_context& io_;
    tcp::acceptor acceptor_;
    void StartAccept();
    void HandleAccept(TCPConnection::pointer new_connection,
      const boost::system::error_code& error);

public:
    TCPServer(boost::asio::io_context& io);
    ~TCPServer();
};

TCPServer::TCPServer(boost::asio::io_context& io) : io_(io), acceptor_(io, tcp::endpoint(tcp::v4(), 13))
{
    StartAccept();
}

TCPServer::~TCPServer()
{
}

void TCPServer::StartAccept(){
    TCPConnection::pointer new_connection = TCPConnection::Create(io_);
    acceptor_.async_accept(new_connection->Socket(),
        std::bind(&TCPServer::HandleAccept, this, new_connection,
          boost::asio::placeholders::error));
}

void TCPServer::HandleAccept(TCPConnection::pointer new_connection,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_connection->Start();
    }

    StartAccept();
  }


// Asynchronous Server Class
class AsyncServer
{
private:
    /* data */
    boost::asio::io_context& io_;
public:
    AsyncServer(boost::asio::io_context& io);
    ~AsyncServer();
};

AsyncServer::AsyncServer(boost::asio::io_context& io) : io_(io)
{
    try
    {
        TCPServer server(io_);
        io_.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

AsyncServer::~AsyncServer()
{
}

