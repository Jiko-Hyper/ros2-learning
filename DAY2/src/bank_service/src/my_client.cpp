#include "bank_service/srv/energy_account.hpp"
#include "rclcpp/rclcpp.hpp"

#include <string>

using namespace std::chrono_literals;

class EnergyAccountClient : public rclcpp::Node
{
private:
    rclcpp::Client<bank_service::srv::EnergyAccount>::SharedPtr client_;
public:
    EnergyAccountClient() : Node("energy_account_client")
    {
        client_ = this->create_client<bank_service::srv::EnergyAccount>("energy_account_service");
    }

    void send_request(const std::string &operation, float amount)
    {
        while (!client_->wait_for_service(1s))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_ERROR(this->get_logger(), "Interrupted while waiting for the service. Exiting.");
                return;
            }
            RCLCPP_INFO(this->get_logger(), "Service not available, waiting again...");
        }

        auto request = std::make_shared<bank_service::srv::EnergyAccount::Request>();
        request->operation = operation;
        request->amount = amount;

        auto future = client_->async_send_request(request);

        while (rclcpp::spin_until_future_complete(
                this->shared_from_this(),
                future,
                1s)== rclcpp::FutureReturnCode::TIMEOUT)
        {
            RCLCPP_INFO(
                this->get_logger(),
                "Waiting for response...");
        }

        auto response = future.get();
        RCLCPP_INFO(this->get_logger(), "Sucess: %s, Balance: %.1f, Message: %s", 
            response->success ? "true" : "false", 
            response->balance, 
            response->message.c_str());
    }

};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    if(argc != 3)
    {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Usage: ros2 run bank_service my_client <operation> <amount>");
        rclcpp::shutdown();
        return 1;
    }
    auto client_node = std::make_shared<EnergyAccountClient>();
    client_node->send_request(argv[1], std::stof(argv[2]));
    rclcpp::shutdown();
    return 0;
}