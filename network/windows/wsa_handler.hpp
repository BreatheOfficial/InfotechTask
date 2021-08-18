#pragma once
#ifdef _WIN32

#include "../detail/socket_types.hpp"
#include <memory>

namespace network {

class wsa_handler
{
public:
    ~wsa_handler()
    {
        network::detail::WSACleanup();
    }

    static wsa_handler* init()
    {
        return ptr_ ? ptr_ : new wsa_handler();
    }

private:
    wsa_handler()
    {
        ptr_ = this;
        network::detail::WSAStartup(WSAVER, &data_);
    }


    static network::detail::WSAData data_;
    static wsa_handler* ptr_;
};  // class wsa_handler

network::detail::WSAData wsa_handler::data_ = { };
wsa_handler* wsa_handler::ptr_ = nullptr;


static std::shared_ptr<wsa_handler> WSA { wsa_handler::init() };

}  // namespace network

#endif