/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-15
 * @Description: 
 * @Version: 1.0
 */

#include <iostream>
#include <unistd.h>
// #include <robot/robot_device.hpp>
#include <robot/robot_control.hpp>

int main() {
    try {
        std::cout << "test robot device" << std::endl;
        auto robot_device = robot::RobotDevice(8080);

        robot_device.control_robot(robot::RobotControl::create_init_command());
        robot_device.control_robot(robot::RobotControl::create_activate_command());
        robot_device.control_robot(robot::RobotControl::create_deactivate_command());

        // robot_device.send_tcp_msg(packet4);
        robot::RobotPacket packet_;
        while (true) {
            while (!robot_device.read_tcp_msg(&packet_)) {
                // robot_device.send_tcp_msg(packet4);
            }

            if (packet_.cmd_id == 4) {
                robot_device.send_tcp_msg(robot::RobotControl::create_write_command(std::move(packet_.data)));
                break;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
