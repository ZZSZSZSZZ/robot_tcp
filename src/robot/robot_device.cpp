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
    for (double data: packet.data) {
        value["DATA"].append(data);
    }
    std::string str = writer.write(value);

    const char *buff = str.c_str();

    return socket->tcp_write(buff, static_cast<int>(strlen(buff)));
}

bool robot::RobotDevice::read_tcp_msg(RobotPacket *packet) {
    char buff[1024] = {0};
    socket->tcp_read(buff, sizeof(buff) / sizeof(char));
    std::string json = buff;

    std::cout << "json: " << json << std::endl;

    Json::Reader reader;
    Json::Value resp;

    if (!reader.parse(json, resp) || !resp.isObject())
        return false;

    packet->cmd_id = resp["CMD"].asInt();

    Json::Value arrayObj = resp["DATA"];
    int mSize = arrayObj.size();

    for (int i = 0; i < mSize; i++) {
        packet->data.push_back(arrayObj[i].asDouble());
    }
    return true;
}

bool robot::RobotDevice::control_robot(RobotPacket packet) {
    send_tcp_msg(packet);
    while (true) {
        RobotPacket packet_;
        while (!read_tcp_msg(&packet_)) {
            send_tcp_msg(packet);
        }

        // printf("cmd : %u\n", packet_.cmd_id);
        std::cout << "cmd: " << unsigned(packet_.cmd_id) << std::endl;

        if (packet_.cmd_id == packet.cmd_id) {
            std::cout << "ok" << std::endl;
            break;
        }
    }
    return true;
}
