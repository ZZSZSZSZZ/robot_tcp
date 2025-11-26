/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-14
 * @Description: 
 * @Version: 1.0
 */

#include <utility>
#include <iostream>

#include "robot/robot.hpp"

robot::Robot::Robot(const int &post) {
    robot_device = std::make_unique<robot::RobotDevice>(post);
}

bool robot::Robot::init_robot() {
    return robot_device->control_robot(robot::RobotControl::create_init_command());
}

bool robot::Robot::enable_robot() {
    return robot_device->control_robot(robot::RobotControl::create_activate_command());
}

bool robot::Robot::disable_robot() {
    return robot_device->control_robot(robot::RobotControl::create_deactivate_command());
}

// bool robot::Robot::read(std::vector<double> *data) {
//     RobotPacket packet;
//     if (!robot_device->read_tcp_msg(&packet)){
//         return false;
//     }
//     if (packet.cmd_id == 4){
//         printf("read \n");
//         *data = packet.data;
//         return true;
//     }
//     return false;
// }

bool robot::Robot::read(std::vector<double> *data) {
    RobotPacket packet;
    robot_device->send_tcp_msg(robot::RobotControl::create_read_command());

    while (!robot_device->read_tcp_msg(&packet)) {
        robot_device->send_tcp_msg(robot::RobotControl::create_read_command());
    }

    if (packet.cmd_id == 4){
        *data = packet.data;
        return true;
    }

    return false;
}

bool robot::Robot::write(std::vector<double> data) {
    if (robot_device->send_tcp_msg(robot::RobotControl::create_write_command(std::move(data)))){
        return true;
    }
    return false;
}

