/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-14
 * @Description: 
 * @Version: 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <iostream>

#include <tcp/tcp_socker.hpp>

using namespace std;

namespace robot::tcp {

    TCPSocket::TCPSocket(const int &post) : server_fd(-1) {
        if (!init_socket(post)) {
            throw TCPSocketException("Failed to initialize socket for interface");
        }
        accept_socket();
    }

    TCPSocket::~TCPSocket() {
        close(server_fd);
    }

    bool TCPSocket::init_socket(const int &post) {
        // 创建TCP套接字
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            // printf("创建套接字失败");
            cout << "创建套接字失败" << endl;
            return false;
        }

        // 设置 TCP_NODELAY 选项
        int flag = 1;
        if (setsockopt(server_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag)) < 0) {
            // printf("无法设置 TCP_NODELAY 选项");
            cout << "无法设置 TCP_NODELAY 选项" << endl;
            close(server_fd);
            return false;
        }

        // 设置 SO_REUSEADDR 选项
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0) {
            // printf("无法设置 SO_REUSEADDR 选项");
            cout << "无法设置 SO_REUSEADDR 选项" << endl;
            close(server_fd);
            return false;
        }

        // 初始化服务器地址结构
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;                // IPv4协议
        server.sin_addr.s_addr = htonl(INADDR_ANY); // 监听所有网络接口
        server.sin_port = htons(post);              // 设置端口号

        // 绑定套接字到指定端口
        if (bind(server_fd, (struct sockaddr *) &server, sizeof(server)) < 0) {
            // printf("绑定端口失败");
            cout << "绑定端口失败" << endl;
            close(server_fd);
            return false;
        }

        // 开始监听连接请求
        if (listen(server_fd, 10) < 0) {
            // printf("监听失败");
            cout << "监听失败" << endl;
            close(server_fd);
            return false;
        }

//        timeval timeout;
//        timeout.tv_sec = 0;
//        timeout.tv_usec = 100;
//        if (setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
//            close(server_fd);
//            return false;
//        }

        // printf("TCP服务端已启动，端口: %d \n", post);
        cout << "TCP服务端已启动 - 端口: " << post << endl;

        return true;
    }

    void TCPSocket::accept_socket() {
        socklen_t clientlen = sizeof(client);
        // 接受客户端连接
        client_fd = accept(server_fd, (struct sockaddr *) &client, &clientlen);
        if (client_fd < 0) {
            // printf("接受连接失败");
            cout << "接受连接失败" << endl;
            close(server_fd);
        }

        // 获取客户端IP和端口
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client.sin_addr, client_ip, INET_ADDRSTRLEN);
        // printf("客户端已连接 - IP: %s, 端口: %d \n", client_ip, ntohs(client.sin_port));
        cout << "客户端已连接 - IP: " << client_ip << " - 端口: " << ntohs(client.sin_port) << endl;
    }


    bool TCPSocket::tcp_write(const char *frame, const int n) {
        int remain = n;
        int wc;
        const char *ptr = frame;

        while (remain) {
            if ((wc = send(client_fd, ptr, remain, 0)) <= 0) {
                if (wc < 0 && errno == EINTR) continue;
                // printf("send error: %s" , strerror(errno));
                cout << "发送错误: " << strerror(errno) << endl;
                return false;
            }
            remain -= wc;
            ptr += wc;
        }
        return true;
    }

    bool TCPSocket::tcp_read(char *frame, const int n) {
        int remain = n;
        int rc;
        char *ptr = frame;

        while (remain) {
            if ((rc = recv(client_fd, ptr, remain, 0)) < 0) {
                if (errno == EINTR) {
                    rc = 0;
                } else {
                    // printf("recv error: %s" , strerror(errno));
                    cout << "接收错误: " << strerror(errno) << endl;
                    return false;
                }
            // } else if (rc == 0) {
            } else if (strchr(ptr, '\0') != nullptr) {
                break;
            }
            remain -= rc;
            ptr += rc;
        }
        return true;
    }

    // bool TCPSocket::tcp_read(char *frame, const int n) {
    //     int timeout_ms = -1;
    //     // 1. 参数合法性校验
    //     if (frame == nullptr || n <= 0) {
    //         cout << "[tcp_read] 非法参数：frame为空指针 或 n=" << n << "（必须>0）" << endl;
    //         return false;
    //     }
    //     if (client_fd < 0) {
    //         cout << "[tcp_read] 客户端套接字未初始化（client_fd=" << client_fd << "）" << endl;
    //         return false;
    //     }
    //
    //     int remain = n;
    //     char *ptr = frame;
    //     fd_set read_fds;
    //     struct timeval tv;
    //
    //     while (remain > 0) {
    //         // 2. 超时机制：若设置超时，通过 select 监听可读事件
    //         if (timeout_ms >= 0) {
    //             FD_ZERO(&read_fds);
    //             FD_SET(client_fd, &read_fds);
    //             tv.tv_sec = timeout_ms / 1000;       // 秒
    //             tv.tv_usec = (timeout_ms % 1000) * 1000; // 微秒
    //
    //             int select_rc = select(client_fd + 1, &read_fds, nullptr, nullptr, &tv);
    //             if (select_rc < 0) {
    //                 if (errno == EINTR) {
    //                     continue; // 被信号中断，重试 select
    //                 }
    //                 cout << "[tcp_read] select 错误（client_fd=" << client_fd << "）：" << strerror(errno) << endl;
    //                 return false;
    //             } else if (select_rc == 0) {
    //                 cout << "[tcp_read] 读取超时（" << timeout_ms << "ms），剩余未读：" << remain << "字节" << endl;
    //                 return false;
    //             }
    //             // 若有可读事件，继续调用 recv
    //         }
    //
    //         // 3. 读取数据
    //         int rc = recv(client_fd, ptr, remain, 0);
    //         if (rc < 0) {
    //             if (errno == EINTR) {
    //                 continue; // 被信号中断，重试 recv（不修改 remain）
    //             }
    //             cout << "[tcp_read] recv 错误（client_fd=" << client_fd << "）：" << strerror(errno) << endl;
    //             return false;
    //         } else if (rc == 0) {
    //             // 4. 对方关闭连接，未读取完指定长度
    //             cout << "[tcp_read] 对方关闭连接（client_fd=" << client_fd << "），已读：" << (n - remain) << "字节，剩余：" << remain << "字节" << endl;
    //             return false;
    //         }
    //
    //         // 5. 更新读取进度
    //         remain -= rc;
    //         ptr += rc;
    //         // 调试日志（可选，生产环境可注释）
    //         // cout << "[tcp_read] 读取成功（client_fd=" << client_fd << "）：" << rc << "字节，剩余：" << remain << "字节" << endl;
    //     }
    //
    //     // 6. 读取完所有 n 字节
    //     // cout << "[tcp_read] 读取完成（client_fd=" << client_fd << "）：共" << n << "字节" << endl;
    //     return true;
    // }
}