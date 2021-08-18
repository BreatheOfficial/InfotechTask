#pragma once

#include "abstract_socket.hpp"
#include "endpoint.hpp"
#include <limits>

namespace network {
namespace tcp {

class base_socket : public abstract_socket
{
public:
    base_socket(network::detail::socket_type _base_socket = network::detail::invalid_socket) noexcept
        : socket_(_base_socket)
    {
    }

    base_socket(int _layer_proto, int _type, int _socket_proto = 0) noexcept
        : socket_(detail::socket(_layer_proto, _type, _socket_proto))
    {
    }

    base_socket(network::NetworkLayerProtocol _layer_proto,
                network::SocketType _type,
                network::SocketProtocol _socket_proto = network::SocketProtocol::AutoSocketProtocol) noexcept
        : base_socket((int)_layer_proto, (int)_type, (int)_socket_proto)
    {
    }

    base_socket(base_socket&& _other) noexcept
            : socket_(_other.exchange())
    {
    }

    base_socket(const base_socket& _other) noexcept = default;


    virtual ~base_socket()
    {
        close();
    }

    base_socket& operator=(network::detail::socket_type _base_socket) noexcept
    {
        socket_ = _base_socket;
        return *this;
    }

    base_socket& operator=(base_socket&& _other) noexcept
    {
        socket_ = _other.exchange();
        return *this;
    }

    base_socket& operator=(const base_socket& _other) noexcept = default;


    network::detail::socket_type socket() const override
    {
        return socket_;
    }

    network::detail::socket_type exchange(network::detail::socket_type _base_socket = detail::invalid_socket)
    {
        auto old_socket_ = socket_;
        socket_ = _base_socket;
        return old_socket_;
    }

    bool connect(const detail::sockaddr_type* _name, int _namelen) const
    {
        return is_retval_valid(detail::connect(socket(), _name, _namelen));
    }

    bool connect(const detail::sockaddr_type* _name, int _namelen, error& _error) const
    {
        return bool_error(connect(_name, _namelen), _error);
    }

    bool connect(const endpoint& _ep) const
    {
        return connect(_ep.base(), _ep.size());
    }

    bool connect(const endpoint& _ep, error& _error) const
    {
        return connect(_ep.base(), _ep.size(), _error);
    }

    bool bind(const detail::sockaddr_type* _name, int _namelen) const
    {
        return is_retval_valid(detail::bind(socket(), _name, _namelen));
    }

    bool bind(const endpoint& _ep) const
    {
        return bind(_ep.base(), _ep.size());
    }

    bool bind(const endpoint& _ep, error& _error) const
    {
        return bool_error(bind(_ep.base(), _ep.size()), _error);
    }

    bool listen(int _n) const
    {
        return is_retval_valid(detail::listen(socket(), _n));
    }

    bool listen(int _n, error& _error) const
    {
        return bool_error(listen(_n), _error);
    }

    bool accept(base_socket& _socket,
                detail::sockaddr_type* _addr = nullptr,
                int* _addrlen = nullptr) const
    {
        detail::socket_type tmp = detail::accept(socket(), _addr, _addrlen);
        if(tmp != detail::invalid_socket) {
            _socket.socket_ = tmp;
            return true;
        }
        return false;
    }

    bool accept(base_socket& _socket, endpoint& _ep) const
    {
        return accept(_socket, _ep.base(), &_ep.size());
    }

private:
    static bool is_retval_valid(int _retval)
    {
        return _retval == NO_ERROR;
    }

    bool bool_error(bool _is_valid, error& _error) const
    {
        if(!_is_valid) {
            _error = last_error();
            return false;
        }
        return true;
    }


    network::detail::socket_type socket_;
}; // class base_socket
} // namespace tcp
} // namespace network