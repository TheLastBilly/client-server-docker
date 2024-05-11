#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "utils.hpp"
#include "socket.hpp"

extern int errno;

static void
help()
{
    printf(
        "Usage: client PATH COMMAND \n"
        "Connects to the UNIX socket on PATH and sends COMMAND. It will wait for a reply "
        "and print it over stdout.\n"
    );
}

int
main( int argc, char * argv[] )
{
    int ret = 0;
    std::string buffer = "";
    Socket::Client client;

    if(argc != 3)
    {
        help();
        return EINVAL;
    }

    ret = client.connect(argv[1]);
    if(ret != 0)
        FAIL(ret, "failed to connect to socket");

    ret = client.write(std::string(argv[2]));
    if(ret != 0)
        FAIL(ret, "failed to send command");

    ret = client.read(buffer);
    if(ret != 0)
        FAIL(errno, "failed to read reply from server");

    printf("%s\n", buffer.c_str());

    return 0;
}
