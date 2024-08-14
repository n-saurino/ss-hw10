#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <vector>

using boost::asio::ip::tcp;

class Client
{
private:
    /* data */
    std::vector<char> buf_;
    int Start();
public:
    Client(/* args */);
    ~Client();
};

Client::Client(/* args */)
{
    Start();
}

Client::~Client()
{
}

int Client::Start(){
    // create context
    boost::asio::io_context io;
    std::string hostname = "127.0.0.1";
    tcp::resolver resolver(io);
    // A resolver takes a host name and service name and turns them into a list of endpoints. 
    // We perform a resolve call using the name of the server, specified in argv[1], 
    // and the name of the service, in this case "daytime".
    tcp::resolver::results_type endpoints = resolver.resolve(hostname, "13");
    tcp::socket socket(io);
    boost::asio::connect(socket, endpoints);

    while(true){
        std::vector<char> buf(128);
        boost::system::error_code error;
        size_t len = socket.read_some(boost::asio::buffer(buf), error);

        if(error == boost::asio::error::eof){
            break; // Connection closed cleanly by peer.
        }else if (error){
            throw boost::system::system_error(error);
        }

        std::cout.write(buf.data(), len);
    }

    return 0;
}
