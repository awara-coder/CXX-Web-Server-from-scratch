#pragma once
#include <netinet/in.h> // for sockaddr_in

/**
 * Header file for Server Class
 * 
 * CodeName - Legion
 */

#define BUFFER_SIZE 65536 // 64K

namespace server
{
    class Server
    {
        int status_;                        // status code of the server
        int server_socket_file_descriptor_; // File Descriptor for Socket
        struct sockaddr_in server_address;

    public:
        Server(void);
        ~Server(void);
        int status(void);
        int createServer(int port);
        void closeServer(void);
        void startAcceptingConnections(int);
    };

} // namespace server