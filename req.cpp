#include "req.h"

namespace Http {
 
Request::Shared Request::MakeShared(const std::string &method, IO::Socket::Shared ss)
{
    if (method == "GET") {
        return std::make_shared<Get>(ss);
    }
    return std::make_shared<Unimplemented>(ss);
}

void Get::Handle() const
{

}

void Unimplemented::Handle() const
{

}
    
}
