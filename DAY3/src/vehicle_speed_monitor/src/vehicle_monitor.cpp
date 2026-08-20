#include <memory>
#include <functional>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

#include "vehicle_speed_monitor/srv/get_status.hpp"
#include "vehicle_speed_monitor/srv/reset_peak.hpp"


class VehicleMonitor : public rclcpp::Node
{
private:
    float current_speed_;
    float peak_speed_;
    float limit_speed_;
    bool is_over_limit_;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr speed_;
    rclcpp::Service<vehicle_speed_monitor::srv::GetStatus>::SharedPtr service_get_;
    rclcpp::Service<vehicle_speed_monitor::srv::ResetPeak>::SharedPtr service_reset_;

    void speed_callback(const std_msgs::msg::Float32::SharedPtr msg)
    {
        current_speed_ = msg->data;
        if(current_speed_ < limit_speed_)
        {
            is_over_limit_ = false;
            if(current_speed_ > peak_speed_)
            {
                peak_speed_ = current_speed_;
            }
            RCLCPP_INFO(this->get_logger(), "Current speed: %.2f, Peak speed: %.2f, Is_over_limit: %s", 
                        current_speed_, 
                        peak_speed_,
                        is_over_limit_ ? "true" : "false"
                        );
        }
        else
        {
            is_over_limit_ = true;
            if(current_speed_ > peak_speed_)
            {
                peak_speed_ = current_speed_;
            }
            RCLCPP_INFO(this->get_logger(), "Current speed: %.2f, Peak speed: %.2f, Is_over_limit: %s", 
                        current_speed_, 
                        peak_speed_,
                        is_over_limit_ ? "true" : "false"
                        );
        }
    }

    void get_status_callback(
        const std::shared_ptr<vehicle_speed_monitor::srv::GetStatus::Request> request,
        std::shared_ptr<vehicle_speed_monitor::srv::GetStatus::Response> response)
    {
        response->current_speed = current_speed_;
        response->peak_speed = peak_speed_;
        response->is_over_limit = is_over_limit_;

        RCLCPP_INFO(this->get_logger(), "Current speed: %.2f, Peak speed: %.2f, Is_over_limit: %s", 
                        current_speed_, 
                        peak_speed_,
                        is_over_limit_ ? "true" : "false"
                        );

    }

    void reset_peak_callback(
        const std::shared_ptr<vehicle_speed_monitor::srv::ResetPeak::Request> request,
        std::shared_ptr<vehicle_speed_monitor::srv::ResetPeak::Response> response)
    {
        peak_speed_ = current_speed_;
        response->success = true;
        response->message = "Peak speed reset successfully.";
    }
    

public:
    VehicleMonitor() : Node("vehicle_monitor"), 
                    current_speed_(0.0), 
                    peak_speed_(0.0),
                    limit_speed_(10.0),
                    is_over_limit_(false) 
    {
        speed_ = this->create_subscription<std_msgs::msg::Float32>(
            "speed", 10, std::bind(&VehicleMonitor::speed_callback, this, std::placeholders::_1)
        );
        service_get_ = this->create_service<vehicle_speed_monitor::srv::GetStatus>(
            "get_status", std::bind(&VehicleMonitor::get_status_callback, this, std::placeholders::_1, std::placeholders::_2)
        );
        service_reset_ = this->create_service<vehicle_speed_monitor::srv::ResetPeak>(   
            "reset_peak", std::bind(&VehicleMonitor::reset_peak_callback, this, std::placeholders::_1, std::placeholders::_2)
        );
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<VehicleMonitor>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}