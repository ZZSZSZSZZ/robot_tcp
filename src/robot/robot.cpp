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

// 初始化机器人
bool robot::Robot::init_robot() {
    return robot_device->control_robot(robot::RobotControl::create_init_command());
}

// 使能机器人
bool robot::Robot::enable_robot() {
    return robot_device->control_robot(robot::RobotControl::create_activate_command());
}

// 失能机器人
bool robot::Robot::disable_robot() {
    return robot_device->control_robot(robot::RobotControl::create_deactivate_command());
}

// 给机器人发送读命令
bool robot::Robot::read_robot() {
    return robot_device->control_robot(robot::RobotControl::create_read_command());
}

/// 关节id排序
/// @param data 关节id原始数组
/// @param order 排序数组
/// @return 根据排序数组排序的关节id数组
vector<vector<float> > robot::Robot::joints_order(const vector<vector<float> > &data, const vector<int> &order) {
    // 创建ID到行数据的映射
    std::unordered_map<int, std::vector<float> > idToRow;
    for (const auto &row: data) {
        int id = static_cast<int>(row[0]);
        idToRow[id] = row;
    }

    // 创建结果数组
    std::vector<std::vector<float> > result;

    // 按照指定顺序添加数据
    for (int id: order) {
        auto it = idToRow.find(id);
        if (it != idToRow.end()) {
            result.push_back(it->second);
        }
    }

    return result;
}

/// 读取数据
/// @param data 返回读到的数组
/// @return 是否成功读取
bool robot::Robot::read(vector<vector<float> > *data) {
    while (true) {
        RobotPacket packet; // buff

        // 当读取失败时重新发送命令，直到读取成功
        while (!robot_device->read_tcp_msg(&packet)) {
        }

        // 如果cmdid为4，返回数组
        if (packet.cmd_id == 4) {
            // 排序数组
            std::vector<int> order = {8, 9, 10, 11, 12, 13, 14, 1552, 7, 6, 5, 4, 3, 2, 1, 1553, 1551};
            *data = joints_order(packet.data, order);

            // *data = packet.data;
            break;
        }
    }
    return true;
}

/// 下发数据
/// @param data 需要下发的数据
/// @return 是否成功下发
bool robot::Robot::write(const vector<float> &data) const {
    vector<float> data_template = {0, 0, 1, 50, 1};
    vector<int> joint_order = {8, 9, 10, 11, 12, 13, 14, 18, 7, 6, 5, 4, 3, 2, 1, 19, 1551, 1552, 1553};
    vector<vector<float> > result;

    for (size_t i = 0; i < data.size(); ++i) {
        // 创建新行，复制模板
        vector<float> row = data_template;

        // 根据关节顺序设置关节ID
        row[0] = joint_order[i];

        // 使用输入数组中的值替换第二个元素
        row[1] = data[i];

        // 添加到结果中
        result.push_back(row);
    }

    for (const auto &row: result) {
        for (float element: row) {
            std::cout << element << " " << std::endl;
        }
        std::cout << ";" << std::endl;
    }

    // 下发数据
    if (robot_device->send_tcp_msg(robot::RobotControl::create_write_command(move(result)))) {
        return true;
    }
    return false;
}
