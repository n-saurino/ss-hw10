/*
**Part 1: Boost.Asio TCP Server and Client**

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
*/