#include "socket.hpp"

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

#include "utils.hpp"

#define READ_BUFFER_LEN     10

using namespace Socket;

extern int errno;

static int
read_fd( int fd, std::string &data )
{
    ssize_t len = 0;
    char buffer[READ_BUFFER_LEN] = {0};

    data = "";
    while((len = read(fd, buffer, sizeof(buffer))) > 0)
        data += std::string(buffer, len);
    if(len < 0)
    {
        data = "";
        return errno;
    }

    return 0;
}

static int
write_fd( int fd, const std::string &data )
{
    if(write(fd, data.c_str(), data.length()+1) < 0)
        return errno;

    return 0;
}

Connection::Connection( int fd ):fd(fd) {}
Connection::~Connection( void )
{
    if(fd <= 0)
        return;

    close(fd);
    fd = 0;
}

int
Connection::read( std::string &data )
{
    return read_fd(fd, data);
}

int
Connection::write( const std::string &data )
{
    return write_fd(fd, data);
}

Server::Server()
{}

Server::~Server()
{
    stop();

    if(connection != nullptr)
    {
        delete connection;
        connection = nullptr;
    }
}

int
Server::listen( const std::string &path )
{
    int ret = 0;
    struct sockaddr_un s_addr = {0};

    stop();

    // Create unix socket
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(fd < 0)
        return errno;

    // Bind socket to path
    s_addr.sun_family = AF_UNIX;
    strncpy(s_addr.sun_path, path.c_str(), path.length());
    unlink(path.c_str());

    ret = bind(fd, (const struct sockaddr *) &s_addr, sizeof(s_addr));
    if(ret < 0)
        return errno;

    if(::listen(fd, 1) < 0)
        return errno;

    return 0;
}

Connection *
Server::accept( void )
{
    int fd = 0;

    if(connection != nullptr)
    {
        delete connection;
        connection = nullptr;
    }

    if((fd = ::accept(this->fd, NULL, NULL)) < 0)
        return NULL;

    connection = new Connection(fd);
    return connection;
}

void
Server::stop( void )
{
    if(fd <= 0)
        return;

    close(fd);
    fd = 0;
}

Client::Client() {}

Client::~Client()
{
    disconnect();
}

int
Client::connect( const std::string &path )
{
    int ret = 0;
    struct sockaddr_un s_addr = {0};

    // Create unix socket
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(fd < 0)
        return errno;

    // Bind socket to path
    s_addr.sun_family = AF_UNIX;
    strncpy(s_addr.sun_path, path.c_str(), ARRLEN(s_addr.sun_path));

    // Try to connect to socket
    ret = ::connect(fd, (const struct sockaddr *) &s_addr, sizeof(s_addr));
    if(ret < 0)
        return errno;

    return 0;
}

void
Client::disconnect( void )
{
    if(fd <= 0)
        return;

    close(fd);
    fd = 0;
}

int
Client::read( std::string &data )
{
    return read_fd(fd, data);
}

int
Client::write( const std::string &data )
{
    return write_fd(fd, data);
}
