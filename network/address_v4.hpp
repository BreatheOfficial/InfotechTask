#pragma once
#include "detail/socket_types.hpp"
#include <array>

namespace network {

class address_v4
{
public:
    typedef network::detail::in4_addr_type addr_type;
    typedef unsigned int uint_type;
    typedef std::array<unsigned char, 4> bytes_type;

    address_v4() noexcept = default;
    address_v4(address_v4&& _other) noexcept = default;
    address_v4(const address_v4& _other) noexcept = default;


    explicit address_v4(uint_type _uint) noexcept
        : addr_()
    {
        from_uint(_uint);
    }

    explicit address_v4(const bytes_type& _bytes) noexcept
        : addr_()
    {
        from_bytes(_bytes);
    }

    explicit address_v4(addr_type _addr) noexcept
        : addr_(_addr)
    {
    }

    address_v4& operator=(address_v4&& _other) noexcept = default;
    address_v4& operator=(const address_v4& _other) noexcept = default;


    friend bool operator==(const address_v4& _a1, const address_v4& _a2) noexcept
    {
        return _a1.addr_.s_addr == _a2.addr_.s_addr;
    }
    friend bool operator!=(const address_v4& _a1, const address_v4& _a2) noexcept
    {
        return _a1.addr_.s_addr != _a2.addr_.s_addr;
    }

    void from_addr(addr_type _addr) noexcept
    {
        addr_ = _addr;
    }

    void from_uint(uint_type _uint) noexcept
    {
        addr_.s_addr = network::detail::htonl(_uint);
    }

    void from_uint_net(uint_type _uint) noexcept
    {
        addr_.s_addr = _uint;

    }

    void from_bytes(const bytes_type& _bytes) noexcept
    {
        detail::memcpy(&addr_, _bytes.data(), sizeof(addr_));
    }

    addr_type to_addr() const noexcept
    {
        return addr_;
    }

    uint_type to_uint() const noexcept
    {
        return network::detail::htonl(addr_.s_addr);
    }

    uint_type to_uint_net() const noexcept
    {
        return addr_.s_addr;
    }

    bytes_type to_bytes() const
    {
        bytes_type bytes;
        detail::memcpy(bytes.data(), &addr_, sizeof(addr_));
        return bytes;
    }

    std::string to_string() const
    {
        return inet_ntoa(addr_);
    }

    static address_v4 loopback() { return address_v4(0x7f000001); }

private:
    addr_type addr_ { };
}; // class address_v4

address_v4 make_address_v4(unsigned int _uint) noexcept
{
    return address_v4(_uint);
}
address_v4 make_address_v4(const address_v4::bytes_type& _bytes) noexcept
{
    return address_v4(_bytes);
}
address_v4 make_address_v4(address_v4::addr_type _addr) noexcept
{
    return address_v4(_addr);
}

address_v4 make_address_v4(const char* _str) noexcept
{
    address_v4 addr;
    addr.from_uint_net(network::detail::inet_addr(_str));
    return addr;
}

address_v4 make_address_v4(const char* _str, error& error_code) noexcept
{
    address_v4::addr_type addr;
    error_code = inet_pton(AF_INET, _str, &addr) != 1 ? EINVAL : 0;
    return address_v4(addr);
}

address_v4 make_address_v4(const std::string& _str) noexcept
{
    return make_address_v4(_str.c_str());
}

address_v4 make_address_v4(const std::string& _str, error& error_code) noexcept
{
    return make_address_v4(_str.c_str(), error_code);
}

template<typename _Elems, typename _Traits>
std::basic_ostream<_Elems, _Traits>& operator<<(std::basic_ostream<_Elems, _Traits>& _os,
        const address_v4& _addr)
{
    return _os << _addr.to_string().c_str();
}

} // namespace network
