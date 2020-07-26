/**
 * Header file for HTTP Response, Request Classes
 */

#include "file.h"
#include <string>
#include <vector>
#include <unordered_map>
#define DEFAULT_MIME_TYPE = "application/octet-stream"

namespace http
{
    std::string getMIMEType(std::string);

    class HTTPRequest
    {
        std::string method_;
        std::string request_target_;
        std::string protocol_name_version_;
        std::unordered_map<std::string, std::string> headers_;

    public:
        HTTPRequest(char *);
        void parseIntialLine(std::vector<std::string> &);
        void parseHeaders(std::vector<std::string> &);
        std::string get_request_target();
        std::string get_method();
        std::string get_protocol_name_version();
        std::string get_headers(std::string);
    };

    class HTTPResponse
    {
        std::string method_;
        std::string protocol_name_version_;
        std::string status_code_;
        std::string status_message_;
        std::unordered_map<std::string, std::string> headers_;
        char *body_;
        int content_length_;

    public:
        HTTPResponse(http::HTTPRequest &);
        void set_status_code(const std::string &);
        int get_status_code(void);
        void add_header(std::string, std::string);
        void set_body(char *);
        void set_content_length(int);
        std::string generateResponse(file::FileData *);
    };

} // namespace http
