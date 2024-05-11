#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <map>
#include <functional>

#include "utils.hpp"
#include "socket.hpp"

#ifndef GIT_VERSION
#error "GIT_VERSION not defined"
#endif

std::map<std::string, std::function<void(Socket::Connection *)>> commands = {
    {"VERSION", [](Socket::Connection *c){ c->write(GIT_VERSION); }},

    // While not part of the assignment, makes testing easier
#ifndef DISABLE_QUIT_COMAND
    {"QUIT", [](Socket::Connection *c){ c->write("OK"); exit(0); }},
#endif
};

static void
help()
{
    printf(
        "Usage: server PATH \n"
        "Creates an UNIX socket on PATH and waits for incomming requests.\n"
        "\n"
        "This tool can accept the following commands:\n"
        "   VERSION            \tsends tool's git HEAD hash to client\n"
#ifndef DISABLE_QUIT_COMAND
        "   QUIT               \tsends \"OK\" to client and stops the server\n"
#endif
    );
}

int
main( int argc, char * argv[] )
{
    int ret = 0;
    std::string buffer = "";
    Socket::Server server;
    Socket::Connection * connection = NULL;

    if(argc != 2)
    {
        help();
        return EINVAL;
    }

    ret = server.listen(std::string(argv[1]));
    if(ret != 0)
        FAIL(ret, "cannot create server socket");

    do
    {
        // Wait for incomming connections
        connection  = server.accept();
        if(connection == NULL)
            FAIL(errno, "failed to accept connection");

        ret = connection->read(buffer);
        if(ret != 0)
            FAIL(ret, "failed to receive data from socket");

        if(commands.find(buffer) != commands.end())
            commands[buffer](connection);
        else
            connection->write("REJECTED");

    } while(ret == 0);

    return 0;
}
