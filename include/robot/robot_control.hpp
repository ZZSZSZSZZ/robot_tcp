/**
 * @Author: ZZSZSZSZZ
 * @CreateTime: 2025-09-15
 * @Description: 
 * @Version: 1.0
 */

#include <cstdint>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

#include <robot/robot_device.hpp>

namespace robot {
    struct StateResult {
        double position;
        double velocity;
    };

    class RobotControl{
    public:
        static RobotPacket create_init_command();
        static RobotPacket create_activate_command();
        static RobotPacket create_deactivate_command();
        static RobotPacket create_read_command();
        static RobotPacket create_write_command(std::vector<double> data);
    };
}
