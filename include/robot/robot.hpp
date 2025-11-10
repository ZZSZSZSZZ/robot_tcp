/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-14
 * @Description: 
 * @Version: 1.0
 */

#include <string>
#include <memory>

#include "robot_control.hpp"

namespace robot {
    class Robot {
    public:
        Robot(const int &post);
        ~Robot() = default;

        bool init_robot();

        bool enable_robot();
        bool disable_robot();

        bool read(std::vector<double> *data);
        bool write(std::vector<double> data);

    private:
        std::unique_ptr<robot::RobotDevice> robot_device;
    };
}