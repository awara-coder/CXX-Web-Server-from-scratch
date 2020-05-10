/**
 * Contains the declaration of worker() which takes the
 * socket file descriptor of the TCP connection to the client
 * and processes the request and sends appropirate response.
 * 
 * The idea behind using a function was that it could be called as a
 * thread.
 */

#define SERVER_ROOT "./serverroot"

namespace worker
{
    void handleHTTPRequest(int);
} // namespace worker