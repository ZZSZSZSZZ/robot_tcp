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

        robot::RobotPacket packet1;
        packet1.cmd_id = 01;
        packet1.data = {};

        robot::RobotPacket packet2;
        packet2.cmd_id = 02;
        packet2.data = {};

        robot::RobotPacket packet3;
        packet3.cmd_id = 03;
        packet3.data = {};

        robot::RobotPacket packet4;
        packet4.cmd_id = 04;
        packet4.data = {};

        robot_device.control_robot(packet1);
        robot_device.control_robot(packet2);
        robot_device.control_robot(packet3);

        robot_device.send_tcp_msg(packet4);
        robot::RobotPacket packet_;
        while (true) {
            if (!robot_device.read_tcp_msg(&packet_)) {
                std::cout << "!!!!!!!!!!!!!!!" << std::endl;
            }

            // printf("cmd : %u\n", packet_.cmd_id);
            std::cout << "cmd: " << unsigned(packet_.cmd_id) << std::endl;
            //        for (size_t i = 0; i < packet_.data.size(); ++i) {
            //            printf("%d ", packet_.data[i]); // 打印每个元素
            //        }
            //        printf("\n");

            if (packet_.cmd_id == packet4.cmd_id) {
                break;
            }
        }

        if (packet_.cmd_id == 4){
            std::cout << "data: " << std::endl;
            std::cout << "list: " << packet_.data.size() << std::endl;
            for (std::vector<double>::const_iterator it = packet_.data.begin(); it != packet_.data.end(); ++it) {
                std::cout << *it << " ";
            }
            std::cout << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}