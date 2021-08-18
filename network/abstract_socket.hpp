#pragma once

#include "detail/socket_types.hpp"
#include "windows/wsa_handler.hpp"

#include <vector>


#define NETWORK_DEFAULT_READ_CHUNK_SIZE 1024
#define NETWORK_DEFAULT_WRITE_CHUNK_SIZE 1024

namespace network {
namespace tcp {

class abstract_socket
{
public:
    using byte_type = char;

    virtual network::detail::socket_type socket() const = 0;

    operator network::detail::socket_type() const
    {
        return socket();
    }

    bool is_valid() const
    {
        return socket() != network::detail::invalid_socket;
    }

    void close() const
    {
        if( is_valid() ) {
#ifdef __WIN32
            network::detail::closesocket(socket());
#else
            network::detail::close(socket());
#endif
        }
    }

    static error last_error()
    {
        //TODO: custom error system
#ifdef __WIN32
        return network::detail::WSAGetLastError();
#else
        return errno;
#endif
    }


    int recv(char* _buff, size_t _n, int _flags = 0) const
    {
        return network::detail::recv(socket(), _buff, _n, _flags);
    }

    std::string read(int _flags = 0) const
    {
        std::string msg;
        int size;
        do {
            byte_type tmp[NETWORK_DEFAULT_READ_CHUNK_SIZE]{ };
            size = recv(tmp, NETWORK_DEFAULT_READ_CHUNK_SIZE, _flags);
            msg += tmp;
        } while(size == NETWORK_DEFAULT_READ_CHUNK_SIZE);
        return msg;
    }

    int send(const char* _buff, size_t _n, int _flags = 0) const
    {
        return detail::send(socket(), _buff, _n, _flags);
    }

    bool write(const std::string& _str, int _flags = 0) const
    {
        if( _str.empty() )
            return false;
        int nByte = 0;
        int size;
        do {
            size = send(_str.c_str(), _str.size(), _flags);
            nByte += size;
        } while(size == NETWORK_DEFAULT_WRITE_CHUNK_SIZE);
        return nByte > 0;
    }


}; // class abstract_socket
} // namespace tcp
} // namespace network
