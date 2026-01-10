/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-14
 * @Description: 
 * @Version: 1.0
 */

#include <string>
#include <memory>
#include <unordered_map>

#include "robot_control.hpp"

namespace robot {
    class Robot {
    public:
        Robot(const int &post);
        ~Robot() = default;

        bool init_robot();

        bool enable_robot();
        bool disable_robot();

        bool read_robot();

        bool read(vector<vector<float>> *data);
        bool write(const vector<float> &data) const;

    private:
        static vector<vector<float>> joints_order(const vector<vector<float>>& data, const vector<int>& order);
        std::unique_ptr<robot::RobotDevice> robot_device;
    };
}
