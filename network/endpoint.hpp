#pragma once

#include "address.hpp"

namespace network {

class endpoint
{
public:
    endpoint() noexcept
        : endpoint(AF_INET6, 0)
    {
    }

    endpoint(const endpoint& _other) noexcept = default;

    endpoint(endpoint&& _other) noexcept = default;

    endpoint(int _family, unsigned short _port) noexcept
    {
        if( _family == AF_INET ) {
            data_.v4.sin_family = AF_INET;
            data_.v4.sin_port = detail::htons(_port);
            data_.v4.sin_addr.s_addr = 0;
            size_ = sizeof(detail::sockaddr_in4_type);
        }
        else if( _family == AF_INET6 ) {
            data_.v6.sin6_family = AF_INET6;
            data_.v6.sin6_port = detail::htons(_port);
            data_.v6.sin6_flowinfo = 0;
            data_.v6.sin6_addr.s6_addr[0] = 0; data_.v6.sin6_addr.s6_addr[1] = 0;
            data_.v6.sin6_addr.s6_addr[2] = 0; data_.v6.sin6_addr.s6_addr[3] = 0;
            data_.v6.sin6_addr.s6_addr[4] = 0; data_.v6.sin6_addr.s6_addr[5] = 0;
            data_.v6.sin6_addr.s6_addr[6] = 0; data_.v6.sin6_addr.s6_addr[7] = 0;
            data_.v6.sin6_addr.s6_addr[8] = 0; data_.v6.sin6_addr.s6_addr[9] = 0;
            data_.v6.sin6_addr.s6_addr[10] = 0; data_.v6.sin6_addr.s6_addr[11] = 0;
            data_.v6.sin6_addr.s6_addr[12] = 0; data_.v6.sin6_addr.s6_addr[13] = 0;
            data_.v6.sin6_addr.s6_addr[14] = 0; data_.v6.sin6_addr.s6_addr[15] = 0;
            data_.v6.sin6_scope_id = 0;
            size_ = sizeof(detail::sockaddr_in6_type);
        }
    }

    endpoint(const address& _address, unsigned short _port)
    {
        if( _address.is_v4() ) {
            data_.v4.sin_family = AF_INET;
            data_.v4.sin_port = detail::htons(_port);
            data_.v4.sin_addr = _address.v4().to_addr();
            size_ = sizeof(detail::sockaddr_in4_type);
        }
        else {
            data_.v6.sin6_family = AF_INET6;
            data_.v6.sin6_port = detail::htons(_port);
            data_.v6.sin6_flowinfo = 0;
            data_.v6.sin6_addr = _address.v6().to_addr();
            data_.v6.sin6_scope_id = _address.v6().scope_id();
            size_ = sizeof(detail::sockaddr_in6_type);
        }
    }

    endpoint& operator=(const endpoint& _other) noexcept = default;
    endpoint& operator=(endpoint&& _other) noexcept = default;

    bool is_v4() const { return data_.base.sa_family == AF_INET; }

    network::detail::sockaddr_type* base()
    {
        return (network::detail::sockaddr_type*)&data_.base;
    }
    const network::detail::sockaddr_type* base() const
    {
        return (const network::detail::sockaddr_type*)&data_.base;
    }

    int size() const noexcept
    {
        return size_;
    }

    int& size() noexcept
    {
        size_ = is_v4() ? sizeof(detail::sockaddr_in4_type) : sizeof(detail::sockaddr_in6_type);
        return size_;
    }

    std::size_t capacity() const noexcept
    {
        return sizeof(data_);
    }

    unsigned short port() const noexcept
    {
        if( is_v4() )
            return detail::htons(data_.v4.sin_port);
        else
            return detail::htons(data_.v6.sin6_port);
    }

    void port(unsigned short _port) noexcept
    {
        if( is_v4() )
            data_.v4.sin_port = detail::htons(_port);
        else
            data_.v6.sin6_port = detail::htons(_port);
    }

    short family()
    {
        return data_.base.sa_family;
    }

    std::string to_string() const
    {
        std::string str;
        char tmp_addr[detail::max_addr_v6_str_len] { };
        if( is_v4() ) {
            str += detail::inet_ntop(AF_INET, (const void*)&data_.v4.sin_addr,
                    tmp_addr, detail::max_addr_v6_str_len);
            str += ":" + std::to_string(detail::htons(data_.v4.sin_port));
        }
        else {
            str += '[';
            str += detail::inet_ntop(AF_INET6, (const void*)&data_.v6.sin6_addr,
                    tmp_addr, detail::max_addr_v6_str_len);
            str += ']';
            str += ":" + std::to_string(detail::htons(data_.v6.sin6_port));
        }
        return str;
    }

private:
    union
    {
        network::detail::socket_addr_type base;
        network::detail::sockaddr_in4_type v4;
        network::detail::sockaddr_in6_type v6;
    } data_ { };
    int size_ { };
}; // class endpoint

} // namespace network