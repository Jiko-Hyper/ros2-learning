#include <memory>
#include <functional>
#include <chrono>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class MinimalSubscriber : public rclcpp::Node
{
private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    void timer_callback(const std_msgs::msg::String::SharedPtr msg) const
    {
        RCLCPP_INFO(this->get_logger(), "I heard: '%s", msg->data.c_str());
    }

public:
    MinimalSubscriber() : Node("minial_subscriber")
    {
        subscription_ = this->create_subscription<std_msgs::msg::String>("topic", 
                                                    10, 
                                                    std::bind(&MinimalSubscriber::timer_callback, this, std::placeholders::_1));
    }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalSubscriber>());
    rclcpp::shutdown();
}