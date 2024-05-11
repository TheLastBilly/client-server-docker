// Based on another project of mine, tildebin
// https://tildegit.org/drevil/tildebin/src/branch/main/tildebin.c

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

#define READ_BUFFER_SIZE    8
#ifndef GIT_VERSION
#error "GIT_VERSION not defined"
#endif

static int sfd = 0;
static int cfd = 0;
extern int errno;

typedef struct
{
    union
    {
        uint64_t code;
        char str[sizeof(uint64_t)];
    };

    void (*func)( int fd );
} command_t;

static void
version( int fd )
{
    WRITES(fd, GIT_VERSION);
}

static command_t
commands[] =
{
    { .str = {"VERSION"}, .func = version }
};

static void
clean( void )
{
    if(sfd >= 0)
    {
        close(sfd);
        sfd = 0;
    }

    if(cfd >= 0)
    {
        close(sfd);
        sfd = 0;
    }
}

static void
help()
{
    printf(
        "Usage: server [-h] PATH \n"
        "Creates an UNIX socket on PATH and waits for"
        "incomming requests.\n"
        "\n"
        "None of the options below are mandatory\n"
        "   -h                 \tprints this message\n\n"
        "\n"
        "This tool can accept the following commands:\n"
        "   VERSION            \tsends tool's git HEAD hash to client\n"
        "\n"
    );
}

int
main( int argc, char * argv[] )
{
    ssize_t len = 0;
    int ret = 0, i = 0;
    const char * path = NULL;
    struct sockaddr_un s_addr = {0};
    command_t command = {0};

    atexit(clean);

    while((i = getopt(argc, (char * const*)argv, "h")) != -1)
    {
        switch(i)
        {
            case 'h':
                help();
                return 0;
            default:
                help();
                return EINVAL;
        }
    }

    if(optind >= argc)
    {
        help();
        return EINVAL;
    }

    path = argv[optind];

    // Create unix socket
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sfd < 0)
        FAIL(errno, "cannot create server socket");

    // Bind socket to path
    s_addr.sun_family = AF_UNIX;
    strncpy(s_addr.sun_path, path, ARRLEN(s_addr.sun_path));
    unlink(path);
    ret = bind(sfd, (const struct sockaddr *) &s_addr, sizeof(s_addr));
    if(ret < 0)
        FAIL(errno, "failed to bind socket to \"%s\"", path);

    // Begin listening
    listen(sfd, 1);

    do
    {
        // Wait for incomming connections
        cfd = accept(sfd, NULL, NULL);
        if(cfd < 0)
        {
            FAIL(errno, "failed to receive data from socket");
        }

        memset(command.str, 0, sizeof(command.str));
        len = read(cfd, command.str, sizeof(command.str));
        if(len < 0)
        {
            FAIL(errno, "failed to receive data from socket");
        }

        for(i = 0; i < ARRLEN(commands); i++)
        {
            if(commands[i].code == command.code && commands[i].func != NULL)
            {
                commands[i].func(cfd);
                break;
            }
        }

        if(i == ARRLEN(commands))
            WRITES(cfd, "REJECTED");

        close(cfd);
        cfd = 0;
    } while(ret == 0);

    return 0;
}
