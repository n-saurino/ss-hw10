#include <stdio.h>
#include "ex1/ex1.h"

class Printer
{
private:
    int count_;
    boost::asio::steady_timer timer_;

public:
    Printer(boost::asio::io_context& io);
    ~Printer();
    void Print();
};

Printer::Printer(boost::asio::io_context& io): timer_(io, std::chrono::seconds(1)), count_(0)
{
    timer_.async_wait(std::bind(&Printer::Print, this));
}

Printer::~Printer()
{
    std::cout << "Final count: " << count_ << std::endl;
}

void Printer::Print(){
    if((this->count_) < 6){
        std::cout << count_ << std::endl;
        this->timer_.expires_at(timer_.expiry() + std::chrono::seconds(1));
        timer_.async_wait(std::bind(&Printer::Print, this));
        ++count_;
    }
}




int main(int, char**){
    // create execution context for asynchronous execution
    boost::asio::io_context io;

    // create Printer object
    Printer printer(io);

    // run the io execution context
    io.run();

    return 0;
}
