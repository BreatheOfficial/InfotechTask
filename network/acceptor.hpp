#pragma once

#include "socket.hpp"

namespace network {
namespace tcp {

template<NetworkLayerProtocol _LayerProto>
class acceptor_impl
{
public:

    acceptor_impl(const socket_impl<_LayerProto>& _socket) noexcept
        : socket_(_socket)
    {
        socket_.open();
    }

    acceptor_impl(socket_impl<_LayerProto>&& _socket) noexcept
        : socket_(std::move(_socket))
    {
        socket_.open();
    }

    acceptor_impl(acceptor_impl&& _other) noexcept = default;
    acceptor_impl(const acceptor_impl& _other) noexcept = default;

    acceptor_impl& operator=(acceptor_impl&& _other) noexcept = default;
    acceptor_impl& operator=(const acceptor_impl& _other) noexcept = default;


    bool accept(socket_impl<_LayerProto>& _socket) const
    {
        return socket_.impl_.accept(_socket.impl_, _socket.endpoint());
    }

    network::endpoint endpoint() const
    {
        return socket_.endpoint();
    }

    const socket_impl<_LayerProto>& lower_layer() const
    {
        return socket_;
    }
private:
    socket_impl<_LayerProto> socket_;
}; // class acceptor

namespace ipv4 {

using acceptor = tcp::acceptor_impl<NetworkLayerProtocol::IPv4Protocol>;

} // namespace ipv4

namespace ipv6 {

using acceptor = tcp::acceptor_impl<NetworkLayerProtocol::IPv6Protocol>;

} // namespace ipv6
} // namespace tcp
} // namespace network
