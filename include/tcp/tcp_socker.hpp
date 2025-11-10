/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-14
 * @Description: 
 * @Version: 1.0
 */

#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

namespace robot::tcp {
    class TCPSocketException : public std::runtime_error {
    public:
        explicit TCPSocketException(const std::string &message)
                : std::runtime_error("Socket error: " + message) {}
    };

    class TCPSocket {
    public:
        explicit TCPSocket(const int &post);
        ~TCPSocket();

        bool tcp_write(const char* frame, int n);
        bool tcp_read(char *frame, int n);
    protected:
        bool init_socket(const int &post);
        void accept_socket();

        int server_fd;
        int client_fd;
        struct sockaddr_in server;   // 服务器地址结构
        struct sockaddr_in client;   // 客户端地址结构
    };
}
