/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-14
 * @Description: 
 * @Version: 1.0
 */

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>
#include <tcp/tcp_socker.hpp>

int main() {
    try {
        std::cout << "test server" << std::endl;
        auto socker = robot::tcp::TCPSocket(8080);
        char write_buf[] = {"{CMD:01}"};
        socker.tcp_write(write_buf, sizeof(write_buf) / sizeof(char));
        char read_buf[1024] = {0};
        while (true) {
            memset(read_buf, 0, sizeof(read_buf) / sizeof(char));
            socker.tcp_read(read_buf, sizeof(read_buf) / sizeof(char));
            std::string str = read_buf;
            // printf("%s", read_buf);
            std::cout << str << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}
