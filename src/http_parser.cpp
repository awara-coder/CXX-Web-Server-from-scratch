/**
 * Definitions for declarations in http_parser.h
 */
#include "http_parser.h"
#include "file.h"
#include <iostream>
#include <algorithm>

const std::string END_LINE = "\r\n";

/**
 * Stores the common Content-type 
 */
std::unordered_map<std::string, std::string> MIME = {
    {"html", "text/html"},
    {"htm", "text/html"},
    {"jpeg", "image/jpg"},
    {"jpg", "image/jpg"},
    {"css", "text/css"},
    {"js", "application/javascript"},
    {"json", "application/json"},
    {"txt", "text/plain"},
    {"gif", "image/gif"},
    {"png", "image/png"}};

/**
 * Returns the MIME type from the filename
 */
std::string http::getMIMEType(std::string fileName)
{
    // retrive the position of '.' in filename
    auto it = fileName.find(".");
    if (it == std::string::npos)
    {
        return "application/octet-stream";
    }

    // Extract the extension of file and convert it to lower case
    std::string extension = fileName.substr(it + 1);
    std::for_each(extension.begin(), extension.end(), [](char &ch) {
        ch = tolower(ch);
    });

    auto Iterator = MIME.find(extension);
    if (Iterator == MIME.end())
    {
        return "application/octet-stream";
    }
    return Iterator->second;
}

/**
 * Constructor for HTTPRequest Class.
 * 
 * Parses the HTTP Request message stored in the buffer
 */
http::HTTPRequest::HTTPRequest(char *buffer)
{
    std::vector<std::string> lines;
    std::string temporaryStringBuffer;
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == '\r' and buffer[i + 1] == '\n')
        {
            if (!temporaryStringBuffer.empty())
            {
                lines.push_back(temporaryStringBuffer);
                temporaryStringBuffer.clear();
            }
            i++;
        }
        else
        {
            temporaryStringBuffer += buffer[i];
        }
    }

    // parse the first line to get method, httpversion, target
    this->parseIntialLine(lines);
    // parse the remaining lines to get headers;
    this->parseHeaders(lines);
}

/**
 * Parses the first line of HTTP Request message to get method, target, and version.
 */
void http::HTTPRequest::parseIntialLine(std::vector<std::string> &lines)
{
    std::string temporaryBuffer;
    int i = 0;
    int lineSize = lines[0].size();
    while (i < lineSize and lines[0][i] != ' ')
    {
        temporaryBuffer += lines[0][i];
        i++;
    }
    method_ = temporaryBuffer;
    temporaryBuffer.clear();
    i++;
    while (i < lineSize and lines[0][i] != ' ')
    {
        temporaryBuffer += lines[0][i];
        i++;
    }
    request_target_ = temporaryBuffer;
    temporaryBuffer.clear();
    i++;
    while (i < lineSize)
    {
        temporaryBuffer += lines[0][i];
        i++;
    }
    protocol_name_version_ = temporaryBuffer;
}

/**
 * Parses the headers present in the http request message.
 */
void http::HTTPRequest::parseHeaders(std::vector<std::string> &lines)
{
    // parse all the lines for header except first
    for (int i = 1; i < lines.size(); i++)
    {
        int positionOfColon = lines[i].find(':');
        std::string keyword = lines[i].substr(0, positionOfColon);
        std::string value = lines[i].substr(positionOfColon + 2);
        headers_[keyword] = value;
    }
}

/**
 * return the request target string
 */
std::string http::HTTPRequest::get_request_target()
{
    return request_target_;
}

/**
 * returns the HTTP Method
 */
std::string http::HTTPRequest::get_method()
{
    return method_;
}

/**
 * returns the protocol used and its version;
 */
std::string http::HTTPRequest::get_protocol_name_version()
{
    return protocol_name_version_;
}

/**
 * return the value of header provided.
 */
std::string http::HTTPRequest::get_headers(std::string keyword)
{
    auto it = headers_.find(keyword);
    if (it == headers_.end())
    {
        return "Header Not Found!!";
    }
    return it->second;
}

/** 
 * Defintions for HTTPResponce class start here
 */

/**
 * Constructor for HTTPResponse class
 * 
 * Takes HTTPRequest Object as input
 */
http::HTTPResponse::HTTPResponse(http::HTTPRequest &request)
{
    body_ = NULL;
    // Retrive the protocol name and version
    this->protocol_name_version_ = request.get_protocol_name_version();

    // Retrive the HTTP Method
    this->method_ = request.get_method();
}

/**
 * Sets the status code and its corresponding status message
 */
void http::HTTPResponse::set_status_code(const std::string &code)
{
    status_code_ = code;
    if (code == "200")
    {
        status_message_ = "OK";
    }
    else if (code == "404")
    {
        status_message_ = "Not Found";
    }
    else if (code == "500")
    {
        status_message_ = "Server Error";
    }
}

/**
 * Stores the given (keyword, value) pair in headers;
 */
void http::HTTPResponse::add_header(std::string keyword, std::string value)
{
    headers_[keyword] = value;
}

/**
 * Sets the body_ to point to file buffer
 */
void http::HTTPResponse::set_body(char *filePointer)
{
    this->body_ = filePointer;
}

/**
 * Sets the content length
 */
void http::HTTPResponse::set_content_length(int contentLength)
{
    this->content_length_ = contentLength;
}

/**
 * Generates the complete HTTP Response message.
 */
std::string http::HTTPResponse::generateResponse(file::FileData *requestedFile)
{
    std::string responseMessage;

    /**
     * Headers for http response message
     * Initial Line (protocol statuscode statusmessage)
     * Date
     * Connection
     * Content-Length
     * Content-Type
     * \r\n
     * body
     */

    // Insert inital line containing protocol status_code e.t.c
    responseMessage += this->protocol_name_version_;
    responseMessage += " ";
    responseMessage += status_code_;
    responseMessage += " ";
    responseMessage += status_message_;
    responseMessage += END_LINE;

    // Insert Headers
    for (auto header : headers_)
    {
        responseMessage += header.first;
        responseMessage += ": ";
        responseMessage += header.second;
        responseMessage += END_LINE;
    }

    // Add a blank line to separate body of message
    responseMessage += END_LINE;

    // Add the body of message
    responseMessage += std::string(requestedFile->data);
    
    return responseMessage;
}