/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-15
 * @Description: 
 * @Version: 1.0
 */

#include <robot/robot_device.hpp>
#include <jsoncpp/json/json.h>
#include <cstring>

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

    return socket->tcp_write(buff, (int) strlen(buff));
}


bool robot::RobotDevice::read_tcp_msg(RobotPacket *packet) {
    char buff[1024];
    socket->tcp_read(buff, (int) strlen(buff));
    std::string json = buff;

    Json::Reader reader;
    Json::Value resp;

    if (!reader.parse(json, resp))
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
        read_tcp_msg(&packet_);

        printf("cmd : %u\n", packet_.cmd_id);
//        for (size_t i = 0; i < packet_.data.size(); ++i) {
//            printf("%d ", packet_.data[i]); // 打印每个元素
//        }
//        printf("\n");

        if (packet_.cmd_id == packet.cmd_id) {
            break;
        }
    }
    return true;
}
