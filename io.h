#ifndef IO_H
#define IO_H

#include <memory>
#include <unistd.h>

namespace IO {

class Error {};

struct Socket {
    using Shared = std::shared_ptr<Socket>;

    int fd;
    explicit Socket(int fd) : fd(fd) {}
    ~Socket() { close(fd); }
    
    Socket(const Socket &) = delete;
    Socket(Socket &&) = delete;
    Socket &operator==(const Socket &) = delete;
    Socket &operator==(Socket &&) = delete;
    
    operator bool() const { return fd >= 0; }
    operator int() const { return fd; }
    
    static Shared MakeShared(int fd) { return std::make_shared<Socket>(fd); }
};

class BufIO {
public:
    explicit BufIO(Socket::Shared ss);
    ~BufIO();
    
    int ReadChar();
    std::string ReadLine();
    
    void Write(const std::string &msg);
private:
    struct BufIOImpl;
    std::unique_ptr<BufIOImpl> pimpl;
};

class Acceptor {
public:
    explicit Acceptor(const std::string &ip, short port);
    Socket::Shared Accept();
private:
    Socket::Shared master;
};

}

#endif
