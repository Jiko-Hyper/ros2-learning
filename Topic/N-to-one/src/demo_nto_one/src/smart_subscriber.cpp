#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class SmartSubscriber : public rclcpp::Node
{
private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    void topic_callback(const std_msgs::msg::String::SharedPtr msg) const
    {
        if(msg->data.rfind("TEMP", 0) == 0)
        {
            RCLCPP_INFO(this->get_logger(), 
                        "Temperature message received: '%s'", msg->data.c_str());
        }
        else if(msg->data.rfind("HUMI", 0) == 0)
        {
            RCLCPP_INFO(this->get_logger(), 
                        "Humidity message received: '%s'", msg->data.c_str());
        }
        else
        {
            RCLCPP_WARN(this->get_logger(), 
                        "Unknown message received: '%s'", msg->data.c_str());
        }
    }

public:
    SmartSubscriber() : Node("smart_subscriber")
    {
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "data", 
            10, 
            std::bind(&SmartSubscriber::topic_callback, this, std::placeholders::_1)
        );
    }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SmartSubscriber>());
    rclcpp::shutdown();
    return 0;
}
