//server.cpp
#include "bank_service/srv/energy_account.hpp"
#include "rclcpp/rclcpp.hpp"

#include <string>

class EnergyAccountService : public rclcpp::Node
{
private:
    float balance_;
    rclcpp::Service<bank_service::srv::EnergyAccount>::SharedPtr service_;
    void handle_request(const bank_service::srv::EnergyAccount::Request::SharedPtr rqs,
                        bank_service::srv::EnergyAccount::Response::SharedPtr rsp)
    {
        if(rqs->operation == "charge")
        {
            if(rqs->amount >= 0.0)
            {
                balance_ += rqs->amount;
                rsp->success = true;
                rsp->balance = balance_;
                rsp->message = "Charge successful";
            }
            else
            {
                rsp->success = false;
                rsp->balance = balance_;
                rsp->message = "Amount must be positive";
            }
        }
        else if(rqs->operation == "consume")
        {
            if(rqs->amount >= 0.0 && rqs->amount <= this->balance_)
            {
                balance_ -= rqs->amount;
                rsp->success = true;
                rsp->balance = balance_;
                rsp->message = "Consume successful";
            }
            else if(rqs->amount < 0.0)
            {
                rsp->success = false;
                rsp->balance = balance_;
                rsp->message = "Amount must be positive";
            }
            else if(rqs->amount >= 0.0 && rqs->amount > this->balance_)
            {
                rsp->success = false;
                rsp->balance = balance_;
                rsp->message = "Insufficient balance";
            }
        }
        else
        {
            rsp->success = false;
            rsp->balance = balance_;
            rsp->message = "Invalid operation";
        }
    }

public:
    EnergyAccountService() : Node("energy_account_service"), balance_(1000.0)
    {
        service_ = this->create_service<bank_service::srv::EnergyAccount>(
            "energy_account_service", std::bind(&EnergyAccountService::handle_request, 
                this, 
                std::placeholders::_1, 
                std::placeholders::_2)  
        );
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<EnergyAccountService>();
    RCLCPP_INFO(node->get_logger(), "Energy Account Service is ready.");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

