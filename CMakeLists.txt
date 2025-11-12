cmake_minimum_required(VERSION 3.22)
project(robot_tcp VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

include(CTest)

include(GNUInstallDirs)

link_directories(/usr/lib/x86_64-linux-gnu)

add_library(
        robot_tcp
        src/tcp/tcp_socker.cpp
        src/robot/robot.cpp
        src/robot/robot_device.cpp
        src/robot/robot_control.cpp
)
set_target_properties(robot_tcp PROPERTIES POSITION_INDEPENDENT_CODE ON)
set(USE_FILE_SET_HEADERS FALSE)
if (USE_FILE_SET_HEADERS)
    target_sources(
            robot_tcp
            PUBLIC FILE_SET
            HEADERS
            TYPE
            HEADERS
            BASE_DIRS
            include
            FILES
            src/tcp/tcp_socker.hpp
            src/robot/robot.hpp
            src/robot/robot_device.hpp
            src/robot/robot_control.hpp)
    install(
            TARGETS robot_tcp
            EXPORT robot_tcp_export
            FILE_SET HEADERS)
else ()
    target_include_directories(
            robot_tcp PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)
    install(TARGETS robot_tcp EXPORT robot_tcp_export)
    install(DIRECTORY include/robot TYPE INCLUDE)
    install(DIRECTORY include/tcp TYPE INCLUDE)
endif ()

# CMake package
set(INSTALL_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/RobotTCP)
install(
        EXPORT robot_tcp_export
        DESTINATION ${INSTALL_CMAKE_DIR}
        NAMESPACE RobotTCP::
        FILE RobotTCPTargets.cmake)
include(CMakePackageConfigHelpers)
configure_package_config_file(
        RobotTCPConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/RobotTCPConfig.cmake
        INSTALL_DESTINATION ${INSTALL_CMAKE_DIR})
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/RobotTCPVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RobotTCPConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/RobotTCPVersion.cmake
        DESTINATION ${INSTALL_CMAKE_DIR})

# pkg-config package
if (IS_ABSOLUTE "${CMAKE_INSTALL_INCLUDEDIR}")
    set(PKG_CONFIG_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}")
else ()
    set(PKG_CONFIG_INCLUDEDIR "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
endif ()
if (IS_ABSOLUTE "${CMAKE_INSTALL_LIBDIR}")
    set(PKG_CONFIG_LIBDIR "${CMAKE_INSTALL_LIBDIR}")
else ()
    set(PKG_CONFIG_LIBDIR "\${prefix}/${CMAKE_INSTALL_LIBDIR}")
endif ()
configure_file(robot-tcp.pc.in ${CMAKE_CURRENT_BINARY_DIR}/robot-tcp.pc
        @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/robot-tcp.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

target_link_libraries(robot_tcp libjsoncpp.so.1.9.5)

add_executable(test_server test/test_server.cpp)
target_link_libraries(test_server robot_tcp)

add_executable(test_robot_device test/test_robot_device.cpp)
target_link_libraries(test_robot_device robot_tcp)

add_executable(test_robot test/test_robot.cpp)
target_link_libraries(test_robot robot_tcp)

# Add tests
if (BUILD_TESTING)
    # add_subdirectory(test)
endif ()