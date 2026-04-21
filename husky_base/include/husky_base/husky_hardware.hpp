// #ifndef HUSKY_BASE__HUSKY_HARDWARE_HPP_
// #define HUSKY_BASE__HUSKY_HARDWARE_HPP_

// #include <memory>
// #include <string>
// #include <vector>

// #include "hardware_interface/handle.hpp"
// #include "hardware_interface/hardware_info.hpp"
// #include "hardware_interface/system_interface.hpp"
// #include "hardware_interface/types/hardware_interface_return_values.hpp"
// #include "hardware_interface/visibility_control.h"
// #include "rclcpp/macros.hpp"

// #include <chrono>

// #include "rclcpp/rclcpp.hpp"

// #include "husky_base/horizon_legacy_wrapper.h"

// using namespace std::chrono_literals;

// namespace husky_base
// {

// class HuskyHardware : public hardware_interface::SystemInterface
// {
// public:
//   RCLCPP_SHARED_PTR_DEFINITIONS(HuskyHardware)

//   HARDWARE_INTERFACE_PUBLIC
//   hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareInfo & info) override;
//   HARDWARE_INTERFACE_PUBLIC
//   std::vector<hardware_interface::StateInterface> export_state_interfaces() override;

//   HARDWARE_INTERFACE_PUBLIC
//   std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

//   HARDWARE_INTERFACE_PUBLIC
//   hardware_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override;

//   HARDWARE_INTERFACE_PUBLIC
//   hardware_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override;

//   HARDWARE_INTERFACE_PUBLIC
//   hardware_interface::return_type read(const rclcpp::Time & time, const rclcpp::Duration & period) override;

//   HARDWARE_INTERFACE_PUBLIC
//   hardware_interface::return_type write(const rclcpp::Time & time, const rclcpp::Duration & period) override;

// private:
//   void resetTravelOffset();
//   double linearToAngular(const double &travel) const;
//   double angularToLinear(const double &angle) const;
//   void writeCommandsToHardware();
//   void limitDifferentialSpeed(double &diff_speed_left, double &diff_speed_right);
//   void updateJointsFromHardware();
//   uint8_t isLeft(const std::string &str);

//   // ROS Parameters
//   std::string serial_port_;
//   double polling_timeout_;
//   double wheel_diameter_, max_accel_, max_speed_;

//   // Store the command for the robot
//   std::vector<double> hw_commands_;
//   std::vector<double> hw_states_position_, hw_states_position_offset_, hw_states_velocity_;

//   uint8_t left_cmd_joint_index_, right_cmd_joint_index_;
// };

// }  // namespace husky_base

// #endif  // HUSKY_BASE__HUSKY_HARDWARE_HPP_


#ifndef HUSKY_BASE__HUSKY_HARDWARE_HPP_
#define HUSKY_BASE__HUSKY_HARDWARE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "hardware_interface/handle.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
// #include "hardware_interface/visibility_control.h"
#include "rclcpp/macros.hpp"

// Copyright 2017 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef HARDWARE_INTERFACE__VISIBILITY_CONTROL_H_
#define HARDWARE_INTERFACE__VISIBILITY_CONTROL_H_

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#define HARDWARE_INTERFACE_EXPORT __attribute__((dllexport))
#define HARDWARE_INTERFACE_IMPORT __attribute__((dllimport))
#else
#define HARDWARE_INTERFACE_EXPORT __declspec(dllexport)
#define HARDWARE_INTERFACE_IMPORT __declspec(dllimport)
#endif
#ifdef HARDWARE_INTERFACE_BUILDING_DLL
#define HARDWARE_INTERFACE_PUBLIC HARDWARE_INTERFACE_EXPORT
#else
#define HARDWARE_INTERFACE_PUBLIC HARDWARE_INTERFACE_IMPORT
#endif
#define HARDWARE_INTERFACE_PUBLIC_TYPE HARDWARE_INTERFACE_PUBLIC
#define HARDWARE_INTERFACE_LOCAL
#else
#define HARDWARE_INTERFACE_EXPORT __attribute__((visibility("default")))
#define HARDWARE_INTERFACE_IMPORT
#if __GNUC__ >= 4
#define HARDWARE_INTERFACE_PUBLIC __attribute__((visibility("default")))
#define HARDWARE_INTERFACE_LOCAL __attribute__((visibility("hidden")))
#else
#define HARDWARE_INTERFACE_PUBLIC
#define HARDWARE_INTERFACE_LOCAL
#endif
#define HARDWARE_INTERFACE_PUBLIC_TYPE
#endif

#endif  // HARDWARE_INTERFACE__VISIBILITY_CONTROL_H_


#include <chrono>

#include "rclcpp/rclcpp.hpp"

#include "husky_base/horizon_legacy_wrapper.h"

using namespace std::chrono_literals;

namespace husky_base
{

class HuskyHardware : public hardware_interface::SystemInterface
{
public:
  RCLCPP_SHARED_PTR_DEFINITIONS(HuskyHardware)

  HARDWARE_INTERFACE_PUBLIC
  hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareInfo & info) override;
  HARDWARE_INTERFACE_PUBLIC
  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;

  HARDWARE_INTERFACE_PUBLIC
  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

  HARDWARE_INTERFACE_PUBLIC
  hardware_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override;

  HARDWARE_INTERFACE_PUBLIC
  hardware_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override;

  HARDWARE_INTERFACE_PUBLIC
  hardware_interface::return_type read(const rclcpp::Time & time, const rclcpp::Duration & period) override;

  HARDWARE_INTERFACE_PUBLIC
  hardware_interface::return_type write(const rclcpp::Time & time, const rclcpp::Duration & period) override;

private:
  void resetTravelOffset();
  double linearToAngular(const double &travel) const;
  double angularToLinear(const double &angle) const;
  void writeCommandsToHardware();
  void limitDifferentialSpeed(double &diff_speed_left, double &diff_speed_right);
  void updateJointsFromHardware();
  uint8_t isLeft(const std::string &str);

  // ROS Parameters
  std::string serial_port_;
  double polling_timeout_;
  double wheel_diameter_, max_accel_, max_speed_;

  // Store the command for the robot
  std::vector<double> hw_commands_;
  std::vector<double> hw_states_position_, hw_states_position_offset_, hw_states_velocity_;

  uint8_t left_cmd_joint_index_, right_cmd_joint_index_;
};

}  // namespace husky_base

#endif  // HUSKY_BASE__HUSKY_HARDWARE_HPP_
