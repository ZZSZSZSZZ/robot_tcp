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
        char read_buf[1024];
        while (true) {
            socker.tcp_read(read_buf, (int) strlen(read_buf));
            printf("%s", read_buf);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}