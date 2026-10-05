#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/temperature.hpp"
#include "std_msgs/msg/string.hpp"

class TempMonitorNode : public rclcpp::Node
{
public:
  TempMonitorNode() : Node("temp_monitor_node"), threshold_(28.0)
  {
    subscription_ = this->create_subscription<sensor_msgs::msg::Temperature>(
      "temperature", 10,
      std::bind(&TempMonitorNode::temperature_callback, this, std::placeholders::_1));

    publisher_ = this->create_publisher<std_msgs::msg::String>("temperature_alarm", 10);
    RCLCPP_INFO(this->get_logger(), "TempMonitorNode elindult (Küszöbérték: %.1f °C)!", threshold_);
  }

private:
  void temperature_callback(const sensor_msgs::msg::Temperature::SharedPtr msg)
  {
    if (msg->temperature > threshold_) {
      auto alarm_msg = std_msgs::msg::String();
      alarm_msg.data = "FIGYELEM: Magas hőmérséklet! Mért érték: " + std::to_string(msg->temperature) + " °C";
      
      RCLCPP_WARN(this->get_logger(), "Riasztás kiadva: %.2f °C", msg->temperature);
      publisher_->publish(alarm_msg);
    } else {
      RCLCPP_INFO(this->get_logger(), "Hőmérséklet normális: %.2f °C", msg->temperature);
    }
  }

  double threshold_;
  rclcpp::Subscription<sensor_msgs::msg::Temperature>::SharedPtr subscription_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TempMonitorNode>());
  rclcpp::shutdown();
  return 0;
}