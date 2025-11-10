/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-15
 * @Description: 
 * @Version: 1.0
 */

#include <iostream>
#include <unistd.h>
#include "robot/robot.hpp"

int main() {
    try {
        std::cout << "test robot device" << std::endl;
        auto robot = robot::Robot(139);
        robot.init_robot();

        while (true){
            std::vector<double> data;
            if (robot.read(&data)){
                for (double i : data) {
                    printf("%f ", i); // 打印每个元素
                }
                printf("\n");
            }
            (robot.write(data));
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}