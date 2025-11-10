
# 编译
```bash
$ sudo apt-get install libjsoncpp-dev
$ cd workspace/robot_tcp
$ git clone https://github.com/ZZSZSZSZZ/robot_tcp.git
$ mkdir build && cd build
$ cmake ..
$ make
```

# 终端1：启动服务端
```bash
$ cd workspace/robot_tcp/build
$ ./test_robot_device
```

# 终端2：启动客户端
```bash
$ cd workspace/robot_ws
$ source install/setup.bash
$ ros2 launch robot_ros2_control robot.launch.py
```

# 删除客户端节点
```bash
$ sudo pkill -f -9 "server_node|client_node"
```