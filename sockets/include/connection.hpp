#pragma once

#include "socket.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string_view>

#include "socket_address.hpp"

class Connection {
public:
    Connection() = default;

    explicit Connection(Socket&& socket)
      : socket_(std::move(socket)) {
    }

    explicit Connection(const SocketAddress& address) {
        connect(address);
    }

    void connect(const SocketAddress& address) {
        Socket tmp;
        const ::sockaddr_in& sock_addr = address.get();
        auto sock_addr_ptr = reinterpret_cast<const ::sockaddr*>(&sock_addr);


        int result = ::connect(tmp.get(), sock_addr_ptr, sizeof(sock_addr));
        if (result < 0) {
            using namespace std::string_literals;
            // Class for exception with errno
            throw std::runtime_error("Cannot connect to "s + std::strerror(errno));
        }
        socket_ = std::move(tmp);
    }

    void write(const std::string& str) {
        if (!socket_.isOpen()) {
            using namespace std::string_literals;
            throw std::runtime_error("Socket is not connected");
        }

        std::string_view to_write = str;
        while (!to_write.empty()) {
            ssize_t written = ::write(socket_.get(), to_write.data(), to_write.size());
            if (written < 0) {
                using namespace std::string_literals;
                throw std::runtime_error("Error writting message"s + std::strerror(errno));
            }
            to_write.remove_prefix(written);
        }
    }

    std::string read(size_t bytes) {
        if (!socket_.isOpen()) {
            using namespace std::string_literals;
            throw std::runtime_error("Socket is not connected");
        }

        std::string buffer(bytes, '\0');
        size_t to_read = bytes;
        while (to_read > 0) {
            char* const start = buffer.data() + buffer.size() - to_read;
            ssize_t result = ::read(socket_.get(), start, to_read);
            if (result == 0) {
                buffer.resize(buffer.size() - to_read);
                return buffer;
            }
            if (result < 0) {
                using namespace std::string_literals;
                throw std::runtime_error("Error reading message"s + std::strerror(errno));
            }
            to_read -= result;
        }
        return buffer;
    }

    std::string readUntil(const std::string& terminator) {
        if (!socket_.isOpen()) {
            using namespace std::string_literals;
            throw std::runtime_error("Socket is not connected");
        }

        std::string buffer(1024, '\0');
        size_t read = 0;
        while (read < terminator.size() || buffer.substr(read - terminator.size(), terminator.size()) != terminator) {
            char* const start = buffer.data() + read;
            ssize_t result = ::read(socket_.get(), start, 1024);
            if (result == 0) {
                throw std::runtime_error("Connection closed");
            }
            if (result < 0) {
                using namespace std::string_literals;
                throw std::runtime_error("Error reading message"s + std::strerror(errno));
            }
            read+=result;
            if (buffer.size() - read < 1024) {
                buffer.resize(buffer.size() * 2);
            }
        }
        return buffer;
    }

    void close() {
        socket_.close();
    }

private:
    Socket socket_;
};
