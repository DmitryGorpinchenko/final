#ifndef SERVER_H
#define SERVER_H

#include "io.h"

namespace Http {

class Server {
public:
    explicit Server(const std::string &dir, const std::string &ip, short port)
        : dir(dir)
        , acceptor(ip, port)
    {
        do { Transaction(acceptor.Accept()); } while (true);
    }
private:
    void Transaction(IO::Socket::Shared ss);
    
    void Perform(IO::Socket::Shared ss);
    void Serve(IO::Socket::Shared ss, const std::string &file);
    void Error(IO::Socket::Shared ss, const std::string &cause,
                                      const std::string &errnum,
                                      const std::string &shortmsg);

    IO::Acceptor acceptor;
    std::string dir;
};

}

#endif
