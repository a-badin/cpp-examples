#pragma once

#include <functional>

#include "socket.hpp"
#include "connection.hpp"
#include "socket_address.hpp"

class Server {
public:
    Server(const SocketAddress& bindAddress) {
        const ::sockaddr_in& sock_addr = bindAddress.get();
        auto sock_addr_ptr = reinterpret_cast<const ::sockaddr*>(&sock_addr);
        int result = ::bind(socket_.get(), sock_addr_ptr, sizeof(sock_addr));
        if (result < 0) {
            using namespace std::string_literals;
            throw std::runtime_error("Cannot bind to address "s + std::strerror(errno));
        }

        result = ::listen(socket_.get(), 1);
        if (result < 0) {
            using namespace std::string_literals;
            throw std::runtime_error("Cannot mark socket as listen "s + std::strerror(errno));
        }
    }

    void Run() {
        while (true) {
            int fd = ::accept(socket_.get(), nullptr, nullptr);
            if (fd < 0) {
                using namespace std::string_literals;
                throw std::runtime_error("Cannot accept new connection "s + std::strerror(errno));
            }
            Socket s{fd};
            Connection connection{std::move(s)};
            std::string data = connection.readUntil("\r\n\r\n");
            connection.write("Hello world!");
        }
    }

private:
    Socket socket_;
};
