#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "utils.h"

#define BUFFER_LEN          128

static int fd = 0;
static char buffer[BUFFER_LEN] = {0};

extern int errno;

static void
help()
{
    printf(
        "Usage: client PATH COMMAND \n"
        "Connects to the UNIX socket on PATH and sends COMMAND.\n"
        "The reply will be printed on stdout.\n"
        "\n"
        "This tool can accept the following commands:\n"
        "   VERSION            \tsends tool's git HEAD hash to client\n"
        "\n"
    );
}

int
main( int argc, char * argv[] )
{
    int ret = 0;
    std::string buffer = "";
    Client client();

    atexit(clean);

    if(argc != 3)
    {
        help();
        return EINVAL;
    }

    path = argv[1];
    command = argv[2];

    ret = client.connect(path);
    if(ret != 0)
        FAIL(ret, "failed to connect to socket");

    buffer = command;
    ret = client.write(buffer);
    if(ret != 0)
        FAIL(ret, "failed to send command");

    ret = client.read(buffer);
    if(ret != 0)
        FAIL(errno, "failed to read reply from server");

    printf("%s\n", buffer.c_str());

    return 0;
}
