## Overview

In Week 10, you will explore advanced networking concepts, focusing on Boost.Asio, high-performance networking, and message queues. This assignment will help you understand how to build robust and efficient network applications, an essential skill in developing trading systems and real-time applications.

**Objectives**

1. Understand the Boost.Asio library for asynchronous network programming.
2. Implement high-performance networking solutions.
3. Utilize message queues for inter-process communication.

**Primary Texts:**

- “C++ Network Programming, Volume 1: Mastering Complexity with ACE and Patterns” by Douglas C. Schmidt and Stephen D. Huston
    - Chapter 3: An Overview of ACE
    - Chapter 4: Networked Applications and ACE
    - Chapter 6: The Reactor and Proactor Patterns
- “Boost.Asio C++ Network Programming” by John Torjo
    - Chapter 1: Introduction to Boost.Asio
    - Chapter 2: Synchronous and Asynchronous Operations
    - Chapter 4: TCP Servers and Clients
    - Chapter 5: UDP Servers and Clients
    - Chapter 8: Timers

# Assignment Details

## **Part 1: Boost.Asio TCP Server and Client**

**Step-by-Step Instructions**

**Step 1: Setting up the TCP Server**

1. **Initialize Boost.Asio:**
    - Create an io_context object to provide I/O services.
2. **Create an Acceptor:**
    - Create a tcp::acceptor object to listen for incoming connections.
    - Bind the acceptor to a TCP endpoint (IP address and port).
3. **Accept Connections:**
    - Use a loop to accept incoming connections.
    - For each accepted connection, create a new tcp::socket and start a new thread to handle the client session.
4. **Handle Client Session:**
    - In the client session function, read data from the socket.
    - Echo the received data back to the client.
    - Close the socket after handling the client.

**Step 2: Setting up the TCP Client**

1. **Initialize Boost.Asio:**
    - Create an io_context object to provide I/O services.
2. **Resolve Server Address:**
    - Use tcp::resolver to resolve the server’s IP address and port.
3. **Connect to Server:**
    - Create a tcp::socket and connect it to the server’s endpoint.
4. **Send and Receive Messages:**
    - In a loop, read user input from the console.
    - Send the input to the server.
    - Read the server’s response and print it.

## **Part 2: High-Performance UDP Communication**

**Step-by-Step Instructions**

**Step 1: Setting up the UDP Server**

1. **Initialize Boost.Asio:**
    - Create an io_context object to provide I/O services.
2. **Create a Socket:**
    - Create a udp::socket object and bind it to a UDP endpoint (IP address and port).
3. **Handle Incoming Datagrams:**
    - Use async_receive_from to asynchronously receive data from clients.
    - Echo the received data back to the client.
    - Use a loop to continuously handle incoming datagrams.

**Step 2: Setting up the UDP Client**

1. **Initialize Boost.Asio:**
    - Create an io_context object to provide I/O services.
2. **Resolve Server Address:**
    - Use udp::resolver to resolve the server’s IP address and port.
3. **Create a Socket:**
    - Create a udp::socket object.
4. **Send and Receive Messages:**
    - In a loop, read user input from the console.
    - Send the input as a datagram to the server.
    - Receive the server’s response and print it.

## **Part 3: Message Queues**

**Step-by-Step Instructions**

**Step 1: Setting up the Message Queue Server**

1. **Generate a Unique Key:**
    - Use ftok to generate a unique key for the message queue.
2. **Create the Message Queue:**
    - Use msgget to create a message queue and obtain its identifier.
3. **Receive Messages:**
    - Use msgrcv to receive messages from the queue.
    - Print the received messages.
4. **Send Response:**
    - Use msgsnd to send a response message back to the client.
5. **Clean Up:**
    - Use msgctl to remove the message queue when done.

**Step 2: Setting up the Message Queue Client**

1. **Generate a Unique Key:**
    - Use ftok to generate a unique key for the message queue.
2. **Create the Message Queue:**
    - Use msgget to create a message queue and obtain its identifier.
3. **Send Messages:**
    - Use msgsnd to send messages to the server.
4. **Receive Response:**
    - Use msgrcv to receive the server’s response.

**Deliverables**

1. **Source Code:** Submit the complete source code for the TCP and UDP servers and clients, as well as the message queue server and client.
2. **Documentation:** Include comments in the code to explain your approach. Additionally, write a brief document (1-2 pages) explaining:
    - The design and implementation of the TCP and UDP servers and clients.
    - How you handled asynchronous I/O.
    - The use of message queues for inter-process communication.

**Submission**

Submit your source code and documentation via the course’s assignment submission portal by the end of Week 10.

**Evaluation Criteria**

- **Correctness:** The program should compile and run without errors.
- **Use of Boost.Asio:** Effective and appropriate use of Boost.Asio for networking.
- **Performance:** Implementation of high-performance networking solutions.
- **Message Queues:** Correct implementation of message queue communication.
- **Documentation:** Clarity and completeness of the comments and the accompanying document.
- **Code Quality:** Code should be clean, well-organized, and follow modern C++ best practices.

**Solutions**

**Part 1: Boost.Asio TCP Server and Client**

**TCP Server:**

```cpp
// tcp_server.cpp
#include <iostream>
#include <boost/asio.hpp>
#include <thread>

using boost::asio::ip::tcp;

void session(tcp::socket socket) {
    try {
        for (;;) {
            char data[1024];
            boost::system::error_code error;

            size_t length = socket.read_some(boost::asio::buffer(data), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            boost::asio::write(socket, boost::asio::buffer(data, length));
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

int main() {
    try {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::thread(session, std::move(socket)).detach();
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
```

**TCP Client:**

```cpp
// tcp_client.cpp
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "8080");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        for (;;) {
            std::cout << "Enter message: ";
            std::string msg;
            std::getline(std::cin, msg);

            boost::asio::write(socket, boost::asio::buffer(msg));

            char reply[1024];
            size_t reply_length = boost::asio::read(socket, boost::asio::buffer(reply, msg.length()));
            std::cout << "Reply: ";
            std::cout.write(reply, reply_length);
            std::cout << "\n";
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
```

**Part 2: High-Performance UDP Communication**

**UDP Server:**

```cpp
// udp_server.cpp
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

void handle_receive(boost::asio::io_context& io_context, udp::socket& socket, char* data, udp::endpoint& client_endpoint) {
    socket.async_receive_from(
        boost::asio::buffer(data, 1024), client_endpoint,
        [&](const boost::system::error_code& error, std::size_t bytes_recvd) {
            if (!error) {
                std::string message(data, bytes_recvd);
                std::cout << "Received: " << message << std::endl;
                socket.async_send_to(boost::asio::buffer(data, bytes_recvd), client_endpoint,
                    [&](const boost::system::error_code&, std::size_t) {
                        handle_receive(io_context, socket, data, client_endpoint);
                    });
            }
        });
}

int main() {
    try {
        boost::asio::io_context io_context;

        udp::socket socket(io_context, udp::endpoint(udp::v4(), 8080));

        char data[1024];
        udp::endpoint client_endpoint;

        handle_receive(io_context, socket, data, client_endpoint);

        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
```

**UDP Client:**

```cpp
// udp_client.cpp
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main() {
    try {
        boost::asio::io_context io_context;

        udp::resolver resolver(io_context);
        udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), "127.0.0.1", "8080");

        udp::socket socket(io_context);
        socket.open(udp::v4());

        for (;;) {
            std::cout << "Enter message: ";
            std::string msg;
            std::getline(std::cin, msg);

            socket.send_to(boost::asio::buffer(msg), *endpoints.begin());

            char reply[1024];
            udp::endpoint sender_endpoint;
            size_t reply_length = socket.receive_from(boost::asio::buffer(reply, 1024), sender_endpoint);
            std::cout << "Reply: ";
            std::cout.write(reply, reply_length);
            std::cout << "\n";
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
```

**Part 3: Message Queues**

**Message Queue Server:**

```cpp
// msg_queue_server.cpp
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>

struct message_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key;
    int msgid;

    // ftok to generate unique key
    key = ftok("progfile", 65);

    // msgget creates a message queue and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);
    message_buffer message;

    while (true) {
        // msgrcv to receive message
        msgrcv(msgid, &message, sizeof(message), 1, 0);

        // display the message
        std::cout << "Data Received: " << message.msg_text << std::endl;

        // Responding to client
        strcpy(message.msg_text, "Message received");
        message.msg_type = 2;
        msgsnd(msgid, &message, sizeof(message), 0);
    }

    // to destroy the message queue
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
```

**Message Queue Client:**

```cpp
// msg_queue_client.cpp
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>

struct message_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key;
    int msgid;

    // ftok to generate unique key
    key = ftok("progfile", 65);

    // msgget creates a message queue and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);
    message_buffer message;

    while (true) {
        std::cout << "Enter message: ";
        std::cin.getline(message.msg_text, 100);
        message.msg_type = 1;

        // msgsnd to send message
        msgsnd(msgid, &message, sizeof(message), 0);

        // msgrcv to receive message
        msgrcv(msgid, &message, sizeof(message), 2, 0);

        // display the message
        std::cout << "Data Received from server: " << message.msg_text << std::endl;
    }

    return 0;
}
```
