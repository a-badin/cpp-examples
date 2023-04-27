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

#include "server.hpp"

int main(int argc, char* argv[]) {
    Server s{SocketAddress{argv[1], 8080}};
    s.Run();

    return 0;
}
