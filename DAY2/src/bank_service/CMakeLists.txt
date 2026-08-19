cmake_minimum_required(VERSION 3.20)
project(bank_service)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 找依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# ==========================
# 生成 EnergyAccount.srv
# ==========================
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/EnergyAccount.srv"
)

ament_export_dependencies(rosidl_default_runtime)

# 获取本包生成的 srv C++ typesupport
rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME}
  "rosidl_typesupport_cpp"
)

# ==========================
# Server
# ==========================
add_executable(bank_server
  src/my_server.cpp
)

target_link_libraries(bank_server
  rclcpp::rclcpp
  "${cpp_typesupport_target}"
)

# ==========================
# Client
# ==========================
add_executable(bank_client
  src/my_client.cpp
)

target_link_libraries(bank_client
  rclcpp::rclcpp
  "${cpp_typesupport_target}"
)

# ==========================
# 安装
# ==========================
install(
  TARGETS
    bank_server
    bank_client
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()