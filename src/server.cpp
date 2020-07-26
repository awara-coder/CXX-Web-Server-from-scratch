#pragma once
#include "server.h"
#include "worker.h"
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

/**
 * Server
 * 
 * Status Code for Server
 * 0 : Not Running
 * 1 : bound to a socket
 * 2 : listening to incoming requests
 */

/**
 * Create an instance of Server
 */
server::Server::Server()
{
    status_ = 0;

    // clear address structure to store server address
    bzero((char *)&server_address, sizeof server_address); // string.h
}

/**
 * Shuts down the server and closes the socket
 */
server::Server::~Server(void)
{
    if (status_ == 1)
    {
        std::cout << "server: Shutting down Server" << std::endl;

        // Close the socket File Descriptor
        close(server_socket_file_descriptor_);
    }
}

/**
 * Returns the server status code
 */
int server::Server::status()
{
    return status_;
}

/**
 * Creates a socket and binds the socket to the port.
 * 
 * AF_INET : IPv4
 * SOCK_STREAM : TCP/IP
 * 0 : Default Protocol
 */
int server::Server::createServer(const int PORT)
{
    // Create a new socket and store file descriptor in server_socket_file_descriptor_
    server_socket_file_descriptor_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_file_descriptor_ < 0)
    {
        std::cerr << "ERROR: unable to open socket" << std::endl;
        exit(1);
    }

    /**
     * setup host address structure for use in bind()
     */

    // server byte order
    server_address.sin_family = AF_INET;

    // fill with current host's IP address
    server_address.sin_addr.s_addr = INADDR_ANY;

    // assign the port no after converting to network byte order
    server_address.sin_port = htons(PORT);

    // bind the socked to current host's IP address with PORT as port no.
    int bind_status = bind(server_socket_file_descriptor_, (struct sockaddr *)&server_address, sizeof server_address);

    if (bind_status < 0)
    {
        close(server_socket_file_descriptor_);
        std::cerr << "ERROR: unable to bind socket to port" << std::endl;
        exit(1);
    }
    status_ = 1;
    std::cout << "server: Server running at port " << PORT << std::endl;

    return 0;
}

/**
 * Shuts down the server and releases the socket
 * server is bound to.
 */
void server::Server::closeServer(void) 
{
    if (status_ == 1) 
    {
        status_ = 0;

        // close the socket file descriptor
        close(server_socket_file_descriptor_);
    }
}

/**
 * Starts listening to the port and accepting incoming requests.
 */
void server::Server::startAcceptingConnections(int backLogQueueSize = 10) 
{
    if (listen(server_socket_file_descriptor_, backLogQueueSize) == -1) 
    {
        std::cerr << "ERROR: Unable to get listening socket" << std::endl;
        exit(1);
    }
    status_ = 2;
    std::cout << "server: Starting to accept Connections" << std::endl;

    // Warning, Blocking Code
    while(true) 
    {
        // Address structure to store Client's address
        struct sockaddr_in  clientAddress;  
        socklen_t socketLength = sizeof(clientAddress);
        int clientSocketFileDescriptor = accept(server_socket_file_descriptor_,
                                                (struct sockaddr *) &clientAddress,
                                                (socklen_t *) &socketLength);
        std::cout << "server : new connection from " 
                << inet_ntoa(clientAddress.sin_addr) 
                << " port " << ntohs(clientAddress.sin_port) 
                << std::endl;
        worker::handleHTTPRequest(clientSocketFileDescriptor); 
    }
}