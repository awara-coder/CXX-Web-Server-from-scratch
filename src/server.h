#pragma once
#include <netinet/in.h> // for sockaddr_in



/**
 * Header file for Server Class
 * 
 * CodeName - Legion
 */

namespace server {
    class Server 
    {
        int status_; // status code of the server
        int sockfd; // File Descriptor for Socket
        int backlog_queue_size;
        struct sockaddr_in server_address;

    public:
        Server(void);
        ~Server(void);
        int status(void);
        int createServer(int port);

    };

}
