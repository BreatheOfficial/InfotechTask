#pragma once
#include "address_v4.hpp"
#include "address_v6.hpp"

namespace network {

class address
{
public:
    address() = default;
    address(const address& _other) noexcept = default;
    address(address&& _other) noexcept = default;

    address(const address_v4& _v4) noexcept
        : v4_(_v4)
    {
    }

    address(address_v4&& _v4) noexcept
        : v4_(std::move(_v4))
    {
    }

    address(const address_v6& _v6) noexcept
        : type_(ipv6)
        , v6_(_v6)
    {
    }

    address(address_v6&& _v6) noexcept
        : type_(ipv6)
        , v6_(std::move(_v6))
    {
    }

    address& operator=(const address& _other) noexcept = default;
    address& operator=(address&& _other) noexcept = default;

    bool is_v4() const
    {
        return type_ == ipv4;
    }

    std::string to_string() const
    {
        return is_v4() ? v4_.to_string() : v6_.to_string();
    }

    network::address_v4 v4() const noexcept
    {
        return v4_;
    }

    network::address_v6 v6() const noexcept
    {
        return v6_;
    }

private:
    enum { ipv4 = AF_INET, ipv6 = AF_INET6 } type_ { ipv4 };
    network::address_v4 v4_ { };
    network::address_v6 v6_ { };
};



} // namespace network