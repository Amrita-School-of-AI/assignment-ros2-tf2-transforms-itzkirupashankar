#include <chrono>
#include <functional>
#include <memory>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"

using namespace std::chrono_literals;

class TFBroadcaster : public rclcpp::Node
{
public:
    TFBroadcaster()
        : Node("tf_broadcaster")
    {
        // Create the TransformBroadcaster
        tf_broadcaster_ =
            std::make_unique<tf2_ros::TransformBroadcaster>(*this);

        // Create timer (100ms)
        timer_ = this->create_wall_timer(
            100ms,
            std::bind(&TFBroadcaster::timer_callback, this));
    }

private:
    void timer_callback()
    {
        geometry_msgs::msg::TransformStamped transform;

        // Timestamp
        transform.header.stamp = this->get_clock()->now();

        // Frame IDs
        transform.header.frame_id = "world";
        transform.child_frame_id = "robot";

        // Get time in seconds
        double time_sec = this->get_clock()->now().seconds();

        // Circular motion
        transform.transform.translation.x = 2.0 * std::cos(time_sec);
        transform.transform.translation.y = 2.0 * std::sin(time_sec);
        transform.transform.translation.z = 0.0;

        // Identity quaternion (no rotation)
        transform.transform.rotation.x = 0.0;
        transform.transform.rotation.y = 0.0;
        transform.transform.rotation.z = 0.0;
        transform.transform.rotation.w = 1.0;

        // Broadcast transform
        tf_broadcaster_->sendTransform(transform);
    }

    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TFBroadcaster>());
    rclcpp::shutdown();
    return 0;
}
