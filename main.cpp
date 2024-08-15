#include <functional>
#include <stdio.h>
#include <thread>
#include "ex1/ex1.h"
#include "AsyncServer/AsyncServer.h"

int main(int, char**){
    boost::asio::io_context io_context;
    AsyncServer server(io_context);
    return 0;
}
