#ifndef SERVER_H
#define SERVER_H

#include "io.h"

#include <memory>

namespace Http {
    
struct Request 
{
    using Shared = std::shared_ptr<Request>;    
    IO::BufIO io;
    
    Request() = default;
    explicit Request(IO::Socket::Shared ss) : io(ss) {}
    
    virtual void Handle() const = 0;
    static Shared MakeShared(const std::string &method, IO::Socket::Shared ss);
};

struct Get : Request {
    Get(IO::Socket::Shared ss) : Request(ss) {}
    void Handle() const override;
};

struct Unimplemented : Request {
    Unimplemented(IO::Socket::Shared ss) : Request(ss) {}
    void Handle() const override;
};

}

#endif
