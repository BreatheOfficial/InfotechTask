#pragma once

#ifndef WSAVER
#define WSAVER 514
#endif

namespace network {
namespace detail {

#if defined(__WIN32)

#ifndef WINVER
#define WINVER 0x0A00
#endif

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define _WIN32_WINNT 0x0A00

#include <winsock2.h>
#include <ws2tcpip.h>

typedef SOCKET socket_type;
const socket_type invalid_socket = INVALID_SOCKET;
const int max_addr_v4_str_len = 256;
const int max_addr_v6_str_len = 256;

#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <sys/un.h>
#include <unistd.h>
typedef int socket_type;
const socket_type invalid_socket = -1;

#if defined(INET_ADDRSTRLEN)
const int max_addr_v4_str_len = INET_ADDRSTRLEN;
#else
const int max_addr_v4_str_len = 256;
#endif
#if defined(INET6_ADDRSTRLEN)
const int max_addr_v6_str_len = INET6_ADDRSTRLEN;
#else
const int max_addr_v6_str_len = 256;
#endif

#endif
typedef sockaddr sockaddr_type;
struct socket_addr_type { short int sa_family; };
typedef in_addr in4_addr_type;
typedef sockaddr_in sockaddr_in4_type;
typedef in6_addr in6_addr_type;
typedef sockaddr_in6 sockaddr_in6_type;
typedef sockaddr_storage sockaddr_storage_type;

} // namespace detail

using error = int;

enum class NetworkLayerProtocol : int {
    IPv4Protocol = AF_INET,
    IPv6Protocol = AF_INET6,
};

#ifdef _WIN32
enum class SocketType : int {
    TcpSocket = SOCK_STREAM,
    UdpSocket = SOCK_DGRAM,
};
#else
enum class SocketType : int {
    TcpSocket = network::detail::SOCK_STREAM,
    UdpSocket = network::detail::SOCK_DGRAM,
};
#endif
enum class SocketProtocol : int {
    AutoSocketProtocol = 0
};

} // namespace network
