cmake_minimum_required(VERSION 3.8)
project(my_robot)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(my_robot_node src/main.cpp)
ament_target_dependencies(my_robot_node rclcpp geometry_msgs)

install(TARGETS my_robot_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch worlds models config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()