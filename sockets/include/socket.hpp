#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <stdexcept>
#include <utility>
#include <unistd.h>


class Socket {
public:
    explicit Socket(int fd)
      : fd_{fd} {
    }

    Socket() {
        fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
        if (fd_ == -1) {
            using namespace std::string_literals;
            throw std::runtime_error("Cannot create socket "s + std::strerror(errno));
        }
    }

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept {
        fd_ = std::exchange(other.fd_, -1);
    }

    bool isOpen() const {
        return fd_ != -1; // -1 to constant
    }

    Socket& operator=(Socket&& other) noexcept {
        try {
          close();
        } catch(...) {
        }

        fd_ = std::exchange(other.fd_, -1);
        return *this;
    }

    void close() {
        if (fd_ != -1) {
            int result = ::close(fd_);
            fd_ = -1;
            if (result < 0) {
                using namespace std::string_literals;
                throw std::runtime_error("Cannot close socket "s + std::strerror(errno));
            }
        }
    }

    int get() const {
        return fd_;
    }

    ~Socket() {
        try {
            close();
        } catch (...) {
        }
    }

private:
    int fd_;
};
