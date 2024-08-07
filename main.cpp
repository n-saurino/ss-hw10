#include <stdio.h>
#include "ex1/ex1.h"

void Print(const boost::system::error_code& /*e*/, boost::asio::steady_timer* timer, int* count){
    if((*count) < 6){
        std::cout << *count << std::endl;
        timer->expires_at(timer->expiry() + std::chrono::seconds(1));
        timer->async_wait(std::bind(Print,
          boost::asio::placeholders::error, timer, count));
        (*count)++;
    }
}

int main(int, char**){
    // create execution context for asynchronous execution
    boost::asio::io_context io;

    // creating int on the stack and passing it by reference to Print function
    int count = 0;

    // create a timer executor with the io execution context
    boost::asio::steady_timer timer(io, std::chrono::seconds(1));
    
    // set the timer to wait asynchronously and run Print when it finishes
    timer.async_wait(std::bind(Print,
          boost::asio::placeholders::error, &timer, &count));

    // run the io execution context
    io.run();

    std::cout << "Final count: " << count << std::endl;

    return 0;
}
