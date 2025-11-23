/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-15
 * @Description: 
 * @Version: 1.0
 */

#include <robot/robot_control.hpp>
#include <utility>

robot::RobotPacket robot::RobotControl::create_init_command() {
    RobotPacket packet;
    packet.cmd_id = 01;
    packet.data = {};
    return packet;
}

robot::RobotPacket robot::RobotControl::create_activate_command() {
    RobotPacket packet;
    packet.cmd_id = 02;
    packet.data = {};
    return packet;
}

robot::RobotPacket robot::RobotControl::create_deactivate_command() {
    RobotPacket packet;
    packet.cmd_id = 03;
    packet.data = {};
    return packet;
}

robot::RobotPacket robot::RobotControl::create_read_command() {
    RobotPacket packet;
    packet.cmd_id = 04;
    packet.data = {};
    return packet;
}

robot::RobotPacket robot::RobotControl::create_write_command(std::vector<double> data) {
    RobotPacket packet;
    packet.cmd_id = 05;
    packet.data = std::move(data);
    return packet;
}