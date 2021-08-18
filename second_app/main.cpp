#include <iostream>
#include "../network/socket.hpp"

class task
{
public:
    using socket_type = network::tcp::ipv4::socket;
    using endpoint_type = network::endpoint;

    task(const endpoint_type& _ep)
        : socket_(_ep)
    {
        if( !socket_.connect() )
            throw std::runtime_error("Connect error, try again.");
    }

    void run()
    {
        while( true ) {
            std::string msg = socket_.read();
            if( msg.empty() )
                break;
            std::cout << ( (msg.size() < 3 || std::stoull(msg) % 32 != 0) ? "Invalid data.\n" : "Valid data.\n" );
        }
    }
private:
    socket_type socket_;
};

int main()
{
    // Virtual machine:
    task t({ network::address_v4::loopback(), 1234 });
    t.run();
    return 0;
}
