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
        int disconnect( void );
    };

    class Server
    {
    private:
        int fd = 0;
        Connection * connection = nullptr;

    public:
        Server( void );
        ~Server( void );

        int listen( const std::string &path );
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
        void disconnect( void );

        int read( std::string &data );
        int write( const std::string &path );
    };
}
