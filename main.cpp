#include "server.h"
#include "opts.h"

#include <signal.h>

int main(int argc, char **argv)
{
    signal(SIGHUP, SIG_IGN);

    if (daemon(0, 0) < 0) {
        return 1;
    }
    
    Opts::Instance().Reset(argc, argv);
    
    try {
        Http::Server(Opts::Instance().dir,
                     Opts::Instance().ip,
                     Opts::Instance().port);
    } catch (...) {
        return 1;
    }
    return 0;
}
