/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-15
 * @Description: 
 * @Version: 1.0
 */

#include <robot/robot_device.hpp>
#include <jsoncpp/json/json.h>
#include <cstring>
#include <string>
#include <iostream>

robot::RobotDevice::RobotDevice(const int &post) {
    socket = std::make_unique<tcp::TCPSocket>(post);
}

bool robot::RobotDevice::send_tcp_msg(RobotPacket packet) {
    Json::FastWriter writer;
    Json::Value value;

    value["CMD"] = packet.cmd_id;

    for (const vector<double>& dataArray: packet.data) {
        Json::Value innerArray;
        for (double data : dataArray) {
            innerArray.append(data);
        }
        value["DATA"].append(innerArray);
    }

    std::string str = writer.write(value);

    std::cout << "tcp_send json: " << str << std::endl;

    const char *buff = str.c_str();

    return socket->tcp_write(buff, static_cast<int>(strlen(buff)));
}

bool robot::RobotDevice::read_tcp_msg(RobotPacket *packet) {
    char buff[1024] = {0};

    socket->tcp_read(buff, sizeof(buff) / sizeof(char));
    std::string json = buff;

    std::cout << "tcp_read json: " << json << std::endl;

    Json::Reader reader;
    Json::Value resp;

    if (!reader.parse(json, resp) || !resp.isObject())
        return false;

    packet->cmd_id = resp["CMD"].asInt();

    Json::Value arrayObj = resp["DATA"];

    for (const auto& innerArray : arrayObj) {
        std::vector<double> innerVec;
        for (const auto& val : innerArray) {
            innerVec.push_back(val.asDouble());
        }
        packet->data.push_back(innerVec);
    }

    return true;
}

bool robot::RobotDevice::control_robot(RobotPacket packet) {
    send_tcp_msg(packet); // 发送命令
    while (true) {
        RobotPacket packet_; // buff

        // 当读取失败时重新发送命令，直到读取成功
        while (!read_tcp_msg(&packet_))
            send_tcp_msg(packet);

        std::cout << "tcp_control cmd: " << unsigned(packet_.cmd_id) << std::endl;

        // 当读取的命令和发送命令相符时，跳出循环返回成功
        if (packet_.cmd_id == packet.cmd_id) {
            std::cout << "ok" << std::endl;
            break;
        }
    }
    return true;
}
