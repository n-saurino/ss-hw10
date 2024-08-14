#pragma once
#include <functional>
#include <stdio.h>
#include <thread>
#include <boost/asio.hpp>

class Printer
{
private:
    int count_;
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;

public:
    Printer(boost::asio::io_context& io);
    ~Printer();
    void Print1();
    void Print2();
};

Printer::Printer(boost::asio::io_context& io): strand_(boost::asio::make_strand(io)), 
                                                timer1_(io, std::chrono::seconds(1)), 
                                                timer2_(io, std::chrono::seconds(1)), 
                                                count_(0)
{
    timer1_.async_wait(boost::asio::bind_executor(strand_,
          std::bind(&Printer::Print1, this)));
    
    timer2_.async_wait(boost::asio::bind_executor(strand_,
    std::bind(&Printer::Print2, this)));
}

Printer::~Printer()
{
    std::cout << "Final count: " << count_ << std::endl;
}

void Printer::Print1(){
    if((this->count_) < 10){
        std::cout << "Timer 1: " << count_ << std::endl;
        this->timer1_.expires_at(timer1_.expiry() + std::chrono::seconds(1));
        timer1_.async_wait(boost::asio::bind_executor(strand_,
          std::bind(&Printer::Print1, this)));
        ++count_;
    }
}

void Printer::Print2(){
    if((this->count_) < 10){
        std::cout << "Timer 2: " << count_ << std::endl;
        this->timer2_.expires_at(timer2_.expiry() + std::chrono::seconds(1));
        timer2_.async_wait(boost::asio::bind_executor(strand_,
    std::bind(&Printer::Print2, this)));
        ++count_;
    }
}


int RunPrinter(int, char**){
    // create execution context for asynchronous execution
    boost::asio::io_context io;

    // create Printer object
    Printer printer(io);

    // start a second thread
    std::thread t([&]{ io.run(); });
    // run main thread
    io.run();
    // wait for child thread to complete
    t.join();

    return 0;
}
