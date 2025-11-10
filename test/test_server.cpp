/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-14
 * @Description: 
 * @Version: 1.0
 */

#include <iostream>
#include <unistd.h>
#include <tcp/tcp_socker.hpp>

int main() {
    try {
        std::cout << "test server" << std::endl;
        auto socker = robot::tcp::TCPSocket(8080);
        char recvbuf[1024];
        while (true){
            socker.tcp_read(recvbuf, sizeof recvbuf);
            printf("%s", recvbuf);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}