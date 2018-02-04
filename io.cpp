#include "io.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace IO {

struct BufIO::BufIOImpl {
    Socket::Shared ss;
    char buf[1024];
    int cur;
    int remaining;
	
    BufIOImpl(Socket::Shared ss)
        : ss(ss)
        , cur(0)
        , remaining(0)
    {}
    int ReadChar()
    {
        if (remaining == 0) {
            if ((remaining = read(ss->fd, buf, sizeof(buf))) > 0) {
                cur = 0;
            } else {
                return -1;
            }
        }
        return buf[--remaining, cur++];
    }
    std::string ReadLine()
    {
        std::string line;
        do {
            int c;
            if ((c = ReadChar()) < 0) {
                break;
            }
            line += c;
            if (c == '\n') {
                break;
            }
        } while (true);
        return line;
    }
    void Write(const std::string &msg)
    {
        send(ss->fd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
    }
};

BufIO::BufIO(Socket::Shared ss) : pimpl(new BufIOImpl(ss)) {}
BufIO::~BufIO() = default;

int BufIO::ReadChar() { return pimpl->ReadChar(); }
std::string BufIO::ReadLine() { return pimpl->ReadLine(); }
void BufIO::Write(const std::string &msg) { pimpl->Write(msg); }

Acceptor::Acceptor(const std::string &ip, short port)
    : master(Socket::MakeShared(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    if ((inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
    ||  (bind(*master, (sockaddr *) (&addr), sizeof(addr)) < 0)
    ||  (listen(*master, SOMAXCONN) < 0)) {
        
        throw Error();
    }
}

Socket::Shared Acceptor::Accept()
{
    return Socket::MakeShared(accept(*master, nullptr, nullptr));
}

}
