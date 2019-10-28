#include <chrono>
#include <cstdio>
#include <memory>
#include <utility>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"

#include "std_msgs/msg/int32.hpp"

#include "high_freq_pub/visibility_control.h"

using namespace std::chrono_literals;

namespace demo_nodes_cpp
{

class Talker : public rclcpp::Node
{
public:
    DEMO_NODES_CPP_PUBLIC
    explicit Talker(const rclcpp::NodeOptions& options)
        : Node("talker", options)
    {
        setvbuf(stdout, NULL, _IONBF, BUFSIZ);
        auto publish_message =
            [this]() -> void
            {
                auto msg = std::make_unique<std_msgs::msg::Int32>();
                msg->data = count_++;
                //RCLCPP_INFO(this->get_logger(), "Publishing message #%d", msg->data);
                pub_->publish(std::move(msg));

                if (count_ > 100000)
                {
                    timer_->cancel();
                    RCLCPP_INFO(this->get_logger(), "Sent 100000 messages");
                }
            };

        rclcpp::QoS qos = rclcpp::QoS(rclcpp::KeepLast(1000)).reliable();
        pub_ = this->create_publisher<std_msgs::msg::Int32>("chatter", qos);

        RCLCPP_INFO(this->get_logger(), "Sending messages...");
        timer_ = this->create_wall_timer(1us, publish_message);
    }

private:
    int32_t count_ = 1;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

}  // namespace demo_nodes_cpp

RCLCPP_COMPONENTS_REGISTER_NODE(demo_nodes_cpp::Talker)
