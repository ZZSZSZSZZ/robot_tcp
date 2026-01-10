
/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-15
 * @Description: 
 * @Version: 1.0
 */

#include <iostream>
#include <chrono>
#include <vector>
#include "robot/robot.hpp"


std::time_t GetTimeStamp() {
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>
            tp = std::chrono::time_point_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now());
    auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(
            tp.time_since_epoch());
    std::time_t timestamp = tmp.count();
    return timestamp;
}

/**
 * @brief 打印统计信息
 * @param durations 持续时间数组（毫秒）
 */
void PrintStatistics(const std::vector<uint64_t>& durations) {
    if (durations.empty()) return;

    uint64_t sum = 0;
    uint64_t min = durations[0];
    uint64_t max = durations[0];

    for (const auto& d : durations) {
        sum += d;
        if (d < min) min = d;
        if (d > max) max = d;
    }

    double average = static_cast<double>(sum) / durations.size();

    std::cout << "\n========== 性能统计 ==========\n";
    std::cout << "总执行次数: " << durations.size() << "\n";
    std::cout << "平均耗时: " << average << " ms\n";
    std::cout << "最小耗时: " << min << " ms\n";
    std::cout << "最大耗时: " << max << " ms\n";
    std::cout << "总耗时: " << sum << " ms\n";
    std::cout << "============================\n";
}

int main() {
    constexpr int TEST_ITERATIONS = 100;

    try {
        std::cout << "开始TCP通信测试...\n";

        // 初始化机器人
        auto robot = robot::Robot(8080);
        robot.read_robot();

        std::vector<uint64_t> read_durations;
        read_durations.reserve(TEST_ITERATIONS);

        int success_count = 0;

        for (int i = 0; i < TEST_ITERATIONS; ++i) {
            uint64_t read_begin_time = GetTimeStamp();

            std::vector<std::vector<float>> data;
            bool read_success = robot.read(&data);

            uint64_t read_end_time = GetTimeStamp();
            uint64_t duration = read_end_time - read_begin_time;

            read_durations.push_back(duration);

            if (read_success) {
                ++success_count;
                if (i == 0) {  // 第一次读取后显示数据结构
                    std::cout << "数据格式: " << data.size() << "行";
                    if (!data.empty()) {
                        std::cout << " × " << data[0].size() << "列";
                    }
                    std::cout << std::endl;
                }
            }

            // 每10次输出一次进度
            if ((i + 1) % 10 == 0) {
                std::cout << "已完成 " << (i + 1) << "/" << TEST_ITERATIONS
                          << " 次读取\n";
            }
        }

        // 打印统计信息
        PrintStatistics(read_durations);
        std::cout << "成功读取次数: " << success_count << "/"
                  << TEST_ITERATIONS << std::endl;

        return 0;

    } catch (const std::exception &e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "未知错误" << std::endl;
        return EXIT_FAILURE;
    }
}