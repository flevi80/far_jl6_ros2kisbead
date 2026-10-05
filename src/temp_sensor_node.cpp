#include <chrono>
#include <random>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/temperature.hpp"

using namespace std::chrono_literals;

class TempSensorNode : public rclcpp::Node
{
public:
  TempSensorNode() : Node("temp_sensor_node"), rd_(), gen_(rd_()), dist_(20.0, 35.0)
  {
    publisher_ = this->create_publisher<sensor_msgs::msg::Temperature>("temperature", 10);
    timer_ = this->create_wall_timer(
      1000ms, std::bind(&TempSensorNode::publish_temperature, this));
    RCLCPP_INFO(this->get_logger(), "TempSensorNode elindult!");
  }

private:
  void publish_temperature()
  {
    auto msg = sensor_msgs::msg::Temperature();
    msg.header.stamp = this->now();
    msg.header.frame_id = "temp_sensor_frame";
    msg.temperature = dist_(gen_); // Véletlenszerű hőmérséklet érték
    msg.variance = 0.0;

    RCLCPP_INFO(this->get_logger(), "Mért hőmérséklet: %.2f °C", msg.temperature);
    publisher_->publish(msg);
  }

  std::random_device rd_;
  std::mt19937 gen_;
  std::uniform_real_distribution<> dist_;
  rclcpp::Publisher<sensor_msgs::msg::Temperature>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TempSensorNode>());
  rclcpp::shutdown();
  return 0;
}