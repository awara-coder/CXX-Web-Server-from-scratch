#pragma once
#include "worker.h"
#include "http_parser.h"
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>


#define BUFFER_SIZE 65536 // 64K

/**
 * Takes the Socket File descriptor and handles the HTTP Request
 * and sends appropirate response.
 */
void worker::handleHTTPRequest(int clientSocketFileDescriptor)
{
    // Read contents of socket and store it into buffer
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    read(clientSocketFileDescriptor, (void *)buffer, BUFFER_SIZE);

    // Create the reqest and response object
    http::HTTPRequest request(buffer);
    http::HTTPResponse response(request);

    if (request.get_method() == "GET")
    {
        char filePath[200];
        strcpy(filePath, SERVER_ROOT);
        strcat(filePath, request.get_request_target().c_str());
        file::FileData *requestedFile = file::openFile(filePath);
        if (requestedFile == NULL)
        {
            response.set_status_code("404");
        }
        else
        {
            response.set_status_code("200");
            response.set_body(requestedFile->data);
            response.set_content_length(requestedFile->sizeOfFile);
        }

        /**
         * Add http headers like date, content-type, content-size
         * e.t.c
         */

        // Date and Time header
        char *timeBuffer = (char *)malloc(1000);
        time_t now = time(0);
        struct tm tm = *gmtime(&now);
        strftime(timeBuffer, sizeof timeBuffer, "%a, %d %b %Y %H:%M:%S %Z", &tm);
        response.add_header("Date", std::string(timeBuffer));

        // Connection Header
        response.add_header("Connection", "close");

        // Content-Encoding
        response.add_header("Content-Encoding", "identity");
        
        // Add Content-Length header only when file is present.
        if (response.get_status_code() != 404) 
        {
            // Content-Length
            response.add_header("Content-Length", std::to_string(requestedFile ->sizeOfFile));

            // Content-Type
            response.add_header("Content-Type", http::getMIMEType(filePath));
        }
        else 
        {
            response.add_header("Content-Length", "0"); // empty content
        }
        // Server
        response.add_header("Server", "Legion");

        // Generate the http response message and send to client
        std::string HTTPResponseMessage = response.generateResponse(requestedFile);
        int send_status = send(clientSocketFileDescriptor,
                               HTTPResponseMessage.c_str(),
                               HTTPResponseMessage.size(),
                               0);
        if (send_status < 0)
        {
            std::cerr << "ERROR Unable to send response" << std::endl;
        }
        free(requestedFile);
    }
    else
    {
        std::cerr << "ERROR: Unkown HTTP Method" << std::endl;
    }

    // Close the Client Socket
    close(clientSocketFileDescriptor);
}