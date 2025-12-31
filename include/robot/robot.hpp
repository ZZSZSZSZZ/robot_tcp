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

        bool read(vector<vector<double>> *data);
        bool write(const vector<double> &data) const;

    private:
        static vector<vector<double>> joints_order(const vector<vector<double>>& data, const vector<int>& order);
        unique_ptr<robot::RobotDevice> robot_device;
    };
}