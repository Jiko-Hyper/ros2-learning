cmake_minimum_required(VERSION 3.20)
project(demo_nto_one)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

# temperature publisher
add_executable(demo_temperature_publisher src/temperature_publisher.cpp)
target_link_libraries(demo_temperature_publisher rclcpp::rclcpp std_msgs::std_msgs)
# humidity publisher
add_executable(demo_humidity_publisher src/humidity_publisher.cpp)
target_link_libraries(demo_humidity_publisher rclcpp::rclcpp std_msgs::std_msgs)
# smart_subscriber
add_executable(demo_smart_subscriber src/smart_subscriber.cpp)
target_link_libraries(demo_smart_subscriber rclcpp::rclcpp std_msgs::std_msgs)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}/
)

install(TARGETS
  demo_temperature_publisher
  demo_humidity_publisher
  demo_smart_subscriber
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
