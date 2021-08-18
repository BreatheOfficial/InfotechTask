#pragma once

#include "base_socket.hpp"

namespace network {
namespace tcp {

template<NetworkLayerProtocol _LayerProto>
class acceptor_impl;

template<NetworkLayerProtocol _LayerProto>
class socket_impl : public abstract_socket
{
public:
    friend class network::tcp::acceptor_impl<_LayerProto>;

    socket_impl() noexcept = default;

    socket_impl(const network::endpoint& _ep) noexcept
        : has_endpoint_(true)
        , endpoint_(_ep)
        , impl_((int)_LayerProto, (int)SocketType::TcpSocket)
    {
    }

    socket_impl(socket_impl&&) noexcept = default;
    socket_impl(const socket_impl&) noexcept = default;

    socket_impl& operator=(socket_impl&&) noexcept = default;
    socket_impl& operator=(const socket_impl&) noexcept = default;


    network::detail::socket_type socket() const override
    {
        return impl_.socket();
    }

    const network::endpoint& endpoint() const
    {
        return endpoint_;
    }

    network::endpoint& endpoint()
    {
        return endpoint_;
    }

    bool is_open() const
    {
        return is_open_;
    }

    bool open(int _n = std::numeric_limits<int>::max())
    {
        if( is_open() )
            return true;
        int reuse = 1;
        //  I have problems with "Address already use" error on Linux
        //  On win all is ok without this piece
#ifndef __WIN32
        detail::setsockopt(socket(), SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));
        detail::setsockopt(socket(), SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse));
        //
#endif
        if( !has_endpoint_ || !impl_.bind(endpoint_) || !impl_.listen(_n) )
            return false;
        is_open_ = true;
        return true;
    }

    bool open(error& _error, int _n = std::numeric_limits<int>::max())
    {
        if( is_open() )
            return true;
        if( !has_endpoint_ || !impl_.bind(endpoint_) || !impl_.listen(_n) ) {
            _error = last_error();
            return false;
        }
        is_open_ = true;
        return true;
    }

    void close()
    {
        impl_.close();
        is_open_ = false;
    }

    bool connect() const
    {
        return impl_.connect(endpoint_);
    }

private:
    socket_impl(network::detail::socket_type _socket)
        : impl_(_socket)
    {
    }

    socket_impl(network::detail::socket_type _socket, const network::endpoint& _ep)
        : has_endpoint_(true)
        , endpoint_(_ep)
        , impl_(_socket)
    {
    }

    socket_impl(network::detail::socket_type _socket, network::endpoint&& _ep)
        : has_endpoint_(true)
        , endpoint_(std::move(_ep))
        , impl_(_socket)
    {
    }

    void endpoint(const network::endpoint& _ep) noexcept
    {
        has_endpoint_ = true;
        endpoint_ = _ep;
    }

    void endpoint(network::endpoint&& _ep) noexcept
    {
        has_endpoint_ = true;
        endpoint_ = std::move(_ep);
    }

    void socket(network::detail::socket_type _socket)
    {
        impl_ = _socket;
    }


    bool has_endpoint_{ false };
    base_socket impl_;
    bool is_open_{ false };
    network::endpoint endpoint_ { (int)_LayerProto, 0 };
}; // class socket_impl


namespace ipv4 {

using socket = tcp::socket_impl<NetworkLayerProtocol::IPv4Protocol>;

} // namespace ipv4

namespace ipv6 {

using socket = tcp::socket_impl<NetworkLayerProtocol::IPv6Protocol>;

} // namespace ipv6
} // namespace tcp
} // namespace network
