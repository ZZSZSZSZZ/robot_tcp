/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-15
 * @Description: 
 * @Version: 1.0
 */

#include <iostream>
#include <unistd.h>
#include <robot/robot_device.hpp>

int main() {
    try {
        std::cout << "test robot device" << std::endl;
        auto robot_device = robot::RobotDevice(8080);
        robot::RobotPacket packet;
        packet.cmd_id = 1;
        packet.data = {0,0,1,11};
        robot_device.send_tcp_msg(packet);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}