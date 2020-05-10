/**
 * main.c -- A Web Server written in C++
 */

#include <iostream>
#include <server.h>

int main(int argc, char *argv[])
{
    // Check if PORT arguement is provided in command line
    if (argc < 2)
    {
        std::cerr << "Error, no port provided" << std::endl;
        exit(1);
    }
    const int PORT = atoi(argv[1]);

    // Create a Server Instance
    server::Server web_server;
    web_server.createServer(PORT);
    web_server.startAcceptingConnections(100);
    web_server.closeServer();
    // Unreachable code
    return 0;
}