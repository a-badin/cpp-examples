#include <iostream>

#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <numeric>
#include <limits>
#include <cstring>
#include <utility>
#include <string_view>

#include "connection.hpp"

int main(int argc, char* argv[]) {
    // Http example.com ip - 93.184.216.34
    Connection c{SocketAddress{"93.184.216.34", 80}};

    std::string get_request = "GET / HTTP/1.1\r\n"
                              "Host: example.com\r\n"
                              "User-Agent: Wget/1.21.2\r\n"
                              "Accept: */*\r\n"
                              "Accept-Encoding: identity\r\n"
                              "Connection: close\r\n"
                              "\r\n";

    c.write(get_request);
    std::string response = c.read(10000);
    std::cout << response << std::endl;
    return 0;
}
