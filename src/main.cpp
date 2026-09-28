#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/bool.hpp"

using namespace std::chrono_literals;


class WallStopNode : public rclcpp::Node
{
public:

    WallStopNode()
    : Node("wall_stop_node"), hit_wall_(false)
    {
        // Publisher that controls the car
        cmd_vel_publisher_ =
            this->create_publisher<geometry_msgs::msg::Twist>(
                "/cmd_vel",
                10
            );


        // Subscriber that listens for the wall contact
        wall_subscription_ =
            this->create_subscription<std_msgs::msg::Bool>(
                "/wall/touched",
                10,
                std::bind(
                    &WallStopNode::wall_callback,
                    this,
                    std::placeholders::_1
                )
            );


        // Run drive_callback every 100 ms = 10 Hz
        timer_ =
            this->create_wall_timer(
                100ms,
                std::bind(
                    &WallStopNode::drive_callback,
                    this
                )
            );


        RCLCPP_INFO(
            this->get_logger(),
            "Wall-stop controller started."
        );
    }


private:

    void wall_callback(
        const std_msgs::msg::Bool::SharedPtr msg)
    {
        if (msg->data)
        {
            hit_wall_ = true;

            RCLCPP_WARN(
                this->get_logger(),
                "WALL HIT! Stopping car."
            );
        }
    }


    void drive_callback()
    {
        geometry_msgs::msg::Twist command;


        if (hit_wall_)
        {
            // STOP
            command.linear.x = 0.0;
            command.angular.z = 0.0;
        }
        else
        {
            // DRIVE FORWARD
            command.linear.x = 1.0;
            command.angular.z = 0.0;
        }


        cmd_vel_publisher_->publish(command);
    }


    bool hit_wall_;


    rclcpp::Publisher<
        geometry_msgs::msg::Twist
    >::SharedPtr cmd_vel_publisher_;


    rclcpp::Subscription<
        std_msgs::msg::Bool
    >::SharedPtr wall_subscription_;


    rclcpp::TimerBase::SharedPtr timer_;
};



int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node =
        std::make_shared<WallStopNode>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}