
#编译
sudo apt-get install libjsoncpp-dev
cd ~/workspace/robot_tcp
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# 终端1：启动服务端
cd ~/workspace/robot_tcp
./build/test_robot

# 终端2：启动客户端
cd ~/code/chapt
source install/setup.bash
ros2 run client client_node


# 删除客户端节点
sudo pkill -f -9 "server_node|client_node"