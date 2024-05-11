#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>

#include <sys/un.h>
#include <sys/socket.h>

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

static void
clean( void )
{
    if(fd >= 0)
    {
        close(fd);
        fd = 0;
    }
}

int
main( int argc, char * argv[] )
{
    int ret = 0;
    const char * path = NULL;
    const char * command = NULL;
    struct sockaddr_un s_addr = {0};

    atexit(clean);

    if(argc != 3)
    {
        help();
        return EINVAL;
    }

    path = argv[1];
    command = argv[2];

    // Create unix socket
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(fd < 0)
        FAIL(errno, "cannot create server socket");

    // Bind socket to path
    s_addr.sun_family = AF_UNIX;
    strncpy(s_addr.sun_path, path, ARRLEN(s_addr.sun_path));

    // Try to connect to socket
    ret = connect(fd, (const struct sockaddr *) &s_addr, sizeof(s_addr));
    if(ret < 0)
        FAIL(errno, "failed to connect to socket");

    if(write(fd, command, strlen(command)) < 1)
        FAIL(errno, "failed to send command");

    if(read(fd, buffer, sizeof(buffer)) < 1)
        FAIL(errno, "failed to read reply from server");

    printf("%s\n", buffer);

    return 0;
}
