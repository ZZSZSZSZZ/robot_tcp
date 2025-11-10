/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-15
 * @Description: 
 * @Version: 1.0
 */

#include <cstdint>
#include <vector>
#include <memory>

#include <tcp/tcp_socker.hpp>

namespace robot {
    struct RobotPacket {
        uint8_t cmd_id;
        std::vector<double> data;
    };

    class RobotDevice {
    public:
        RobotDevice(const int &post);

        ~RobotDevice() = default;

        bool send_tcp_msg(RobotPacket packet);
        bool read_tcp_msg(RobotPacket *packet);

        bool control_robot(RobotPacket packet);

    private:
        std::unique_ptr<tcp::TCPSocket> socket;
    };
}