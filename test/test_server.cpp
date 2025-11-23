/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-14
 * @Description: 
 * @Version: 1.0
 */

#include <chrono>
#include <iostream>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <tcp/tcp_socker.hpp>

int main() {
    try {
        std::cout << "test server" << std::endl;
        auto socker = robot::tcp::TCPSocket(8080);
        // char write_buf[1024] = "{CMD:01}";
        // socker.tcp_write(write_buf, (int) strlen(write_buf));
        char read_buf[1024];
        while (true) {
            socker.tcp_read(read_buf, (int) strlen(read_buf));
            printf("%s", read_buf);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}