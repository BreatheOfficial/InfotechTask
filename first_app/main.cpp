#include <iostream>
#include "../network/acceptor.hpp"
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>

class task
{
public:

    task(const network::endpoint& _ep)
    {
        network::tcp::ipv4::acceptor acceptor { _ep };
        if( !acceptor.accept(socket_) )
            throw std::runtime_error("Accept error, try again.");
    }

    ~task()
    {
        stop();
    }

    void run()
    {
        is_working_ = true;
        is_ready_ = false;
        std::thread(&task::sender_thread, this).detach();
        main_thread();
    }

    void stop()
    {
        is_working_ = false;
    }

private:

    bool process_line(const std::string& _line, std::array<int, 10>& _array) const
    {
        if( _line.size() > 64 )
            return false;
        std::array<int, 10> tmp_array { };
        for( const auto& chr : _line ) {
            if( !std::isdigit(chr) )
                return false;
            ++tmp_array[chr - '0'];
        }
        _array = std::move(tmp_array);
        return true;
    }

    std::string dtoKB(const std::array<int, 10>& _digits)
    {
        std::string kb_str;
        bool is_shifted = false;
        auto process_digit = [&](int _count, const std::initializer_list<char>& _head,
                                 const std::initializer_list<char>& _tail) {
            for( auto j = _count; j > 1; j -= 2 ) {
                kb_str.append(_head);
            }
            if( _count & 1 ) {
                kb_str.append(_tail);
                is_shifted = !is_shifted;
            }
        };
        for( int i = 9; i >= 0; --i ) {
            char chr = i + '0';
            if( !is_shifted )
                process_digit(_digits[i], { 'K', 'B', chr }, { 'K', 'B' });
            else
                process_digit(_digits[i], { chr, 'K', 'B' }, { chr });
        }
        return kb_str;
    }

    void main_thread()
    {
        std::array<int, 10> digits;
        while( is_working_ ) {
            std::string line;
            std::getline(std::cin, line);
            if( process_line(line, digits) ) {
                std::string kb_str = dtoKB(digits);
                std::unique_lock<std::mutex> lk(mutex_);
                buffer_ = std::move(kb_str);
                is_ready_ = true;
                lk.unlock();
                condition_variable_.notify_one();
            }
        }
    }

    void sender_thread()
    {
        while( is_working_ ) {
            std::unique_lock<std::mutex> lk(mutex_);
            condition_variable_.wait(lk, [&]() { return is_ready_; });
            std::string buffer = std::move(buffer_);
            is_ready_ = false;
            std::cout << buffer << '\n';
            unsigned sum = 0;
            for(std::size_t i = 2; i < buffer.size(); i += 3) {
                sum += buffer[i] - '0';
            }
            auto str_sum = std::to_string(sum);
            if( !socket_.write(str_sum) )
                stop();
        }
    }


    network::tcp::ipv4::socket socket_;
    std::string buffer_;
    std::mutex mutex_;
    std::condition_variable condition_variable_;
    std::atomic_bool is_working_;
    bool is_ready_;
};

int main()
{
    task t({ network::address_v4::loopback(), 1234 });
    t.run();
    return 0;
}
