#include "server.h"

#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace Http {

void Server::Transaction(IO::Socket::Shared ss)
{
    std::thread(&Server::Perform, this, ss).detach();
}

void Server::Perform(IO::Socket::Shared ss)
{
    std::string method, uri, version, file;
    std::istringstream(IO::BufIO(ss).ReadLine()) >> method 
                                                 >> uri 
                                                 >> version;
    
    file = dir + uri.substr(0, uri.find_first_of('?'));
    
    if (std::ifstream(file)) {
        Serve(ss, file);
    } else {
        Error(ss, file, "404", "Not found");
    }
}

void Server::Serve(IO::Socket::Shared ss, const std::string &file)
{
    std::ostringstream oss;
    
    std::ifstream in(file);
    std::stringstream buffer;
    buffer << in.rdbuf();
    
    oss << "HTTP/1.0 200 OK\r\n"
        << "Server: Http Web Server\r\n"
        << "Connection: close\r\n"
        << "Content-length: " << buffer.str().size() << "\r\n"
        << "Content-type: " << "text/html" << "\r\n\r\n";
    
    IO::BufIO(ss).Write(oss.str() + buffer.str());
}

void Server::Error(IO::Socket::Shared ss, const std::string &cause,
                                          const std::string &errnum,
                                          const std::string &shortmsg)
{
    std::string err("Error occured");

    std::ostringstream oss;
    
    oss << "HTTP/1.0 " << errnum << " " << shortmsg << "\r\n"
        << "Content-length: " << err.size() << "\r\n"
        << "Content-type: " << "text/html" << "\r\n\r\n"
        << err;

    IO::BufIO(ss).Write(oss.str());
}   

}
