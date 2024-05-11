#pragma once

#include <iostream>

namespace Socket
{
    class Connection
    {
    private:
        int fd = 0;

    public:
        Connection( int fd );
        ~Connection( void );

        int read( std::string &data );
        int write( const std::string &data );
    };

    class Server
    {
    private:
        std::string path;
        int fd = 0;

    public:
        Server( void );
        ~Server( void );

        int listen( void );
        void stop( void );

        Connection* accept( void );
    };

    class Client
    {
    private:
        int fd = 0;

    public:
        Client();
        ~Client();

        int connect( const std::string &path );
        int disconnect( void );

        int read( std::string &data );
        int write( const std::string &path );
    };
}
