#include <memory>
#include <string>
#include <chrono>
#include <functional>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class HumidityPublsher : public rclcpp::Node
{
private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    size_t count_;
    void timer_callback()
    {
        auto message = std_msgs::msg::String();
        message.data = "HUMI:" + std::to_string((count_ ++) % 20);
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
        publisher_->publish(message);
    }
public:
    HumidityPublsher() : Node("humidity_publisher"), count_(0)
    {
        this->declare_parameter("publish_frequency", 1.0);
        double hz = this->get_parameter("publish_frequency").as_double();
        RCLCPP_INFO(this->get_logger(), "Frequency = %f", hz);
        auto timer = std::chrono::duration<double>(1.0 / hz);
        publisher_ = this->create_publisher<std_msgs::msg::String>("data", 10);
        timer_ = this->create_wall_timer(
            timer,
            std::bind(&HumidityPublsher::timer_callback, this)
        );
    }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HumidityPublsher>());
    rclcpp::shutdown();
}