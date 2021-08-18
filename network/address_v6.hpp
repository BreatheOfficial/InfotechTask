#pragma once
#include "detail/socket_types.hpp"
#include <array>

namespace network {

class address_v6
{
public:
    typedef network::detail::in6_addr_type addr_type;
    typedef std::array<unsigned char, 16> bytes_type;

    address_v6() noexcept = default;
    address_v6(address_v6&& _other) noexcept = default;
    address_v6(const address_v6& _other) noexcept = default;

    explicit address_v6(const bytes_type& _bytes, uint32_t _scope_id = 0) noexcept
        : scope_id_(_scope_id)
    {
        from_bytes(_bytes);
    }

    explicit address_v6(addr_type _addr, uint32_t _scope_id = 0) noexcept
        : addr_(_addr)
        , scope_id_(_scope_id)
    {
    }

    address_v6& operator=(address_v6&& _other) noexcept = default;
    address_v6& operator=(const address_v6& _other) noexcept = default;


    friend bool operator==(const address_v6& _a1, const address_v6& _a2) noexcept
    {
        return network::detail::memcmp(&_a1.addr_.s6_addr, &_a2.addr_.s6_addr, 16) == 0;
    }
    friend bool operator!=(const address_v6& _a1, const address_v6& _a2) noexcept
    {
        return !(_a1 == _a2);
    }


    void from_addr(addr_type _addr) noexcept
    {
        detail::memcpy(&addr_, &_addr, sizeof(addr_type));
    }

    void from_bytes(const bytes_type& _bytes) noexcept
    {
        memcpy(&addr_.s6_addr, _bytes.data(), sizeof(addr_type));
    }

    void scope_id(uint32_t _scope_id) noexcept
    {
        scope_id_ = _scope_id;
    }

    addr_type to_addr() const noexcept
    {
        return addr_;
    }

    bytes_type to_bytes() const
    {
        bytes_type bytes;
        memcpy(bytes.data(), &addr_.s6_addr, 16);
        return bytes;
    }

    uint32_t scope_id() const noexcept
    {
        return scope_id_;
    }

    std::string to_string() const {
        using namespace network::detail;
        char buff[max_addr_v6_str_len] { };
        inet_ntop(AF_INET6, &addr_, buff, max_addr_v6_str_len);
        return buff;
    }

    std::string to_string(error& error_code) const
    {
        using namespace network::detail;
        char buff[max_addr_v6_str_len] { };
        error_code = inet_ntop(AF_INET6, &addr_, buff, max_addr_v6_str_len) != nullptr;
        return buff;
    }

    static address_v6 loopback()
    {
        address_v6 addr;
        addr.addr_.u.Byte[15] = 1;
        return addr;
    }

private:
    addr_type addr_;
    uint32_t scope_id_;
}; // class address_v6

address_v6 make_address_v6(const char* _str, uint32_t _scope_id = 0) noexcept
{
    address_v6::addr_type addr;
    inet_pton(AF_INET6, _str, &addr);
    return address_v6(addr, _scope_id);
}

address_v6 make_address_v6(const char* _str, error& error_code, uint32_t _scope_id = 0) noexcept
{
    address_v6::addr_type addr;
    error_code = inet_pton(AF_INET6, _str, &addr) != 1;
    return address_v6(addr, _scope_id);
}

address_v6 make_address_v6(const std::string& _str, uint32_t _scope_id = 0) noexcept
{
    return make_address_v6(_str.c_str(), _scope_id);
}

address_v6 make_address_v6(const std::string& _str, error& error_code, uint32_t _scope_id = 0) noexcept
{
    return make_address_v6(_str.c_str(), error_code, _scope_id);
}

template<typename _Elems, typename _Traits>
std::basic_ostream<_Elems, _Traits>& operator<<(std::basic_ostream<_Elems, _Traits>& _os,
                                                const address_v6& _addr)
{
    return _os << _addr.to_string().c_str();
}

} // namespace network