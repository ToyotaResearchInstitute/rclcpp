// Copyright 2015 Open Source Robotics Foundation, Inc.
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

#ifndef RCLCPP_RCLCPP_PARAMETER_CLIENT_HPP_
#define RCLCPP_RCLCPP_PARAMETER_CLIENT_HPP_

#include <string>

#include <rmw/rmw.h>

#include <rclcpp/executors.hpp>
#include <rclcpp/macros.hpp>
#include <rclcpp/node.hpp>
#include <rclcpp/parameter.hpp>

#include <rcl_interfaces/GetParameters.h>
#include <rcl_interfaces/GetParameterTypes.h>
#include <rcl_interfaces/Parameter.h>
#include <rcl_interfaces/ParameterDescriptor.h>
#include <rcl_interfaces/ParameterType.h>
#include <rcl_interfaces/SetParameters.h>
#include <rcl_interfaces/SetParametersAtomically.h>
#include <rcl_interfaces/ListParameters.h>
#include <rcl_interfaces/DescribeParameters.h>

namespace rclcpp
{

namespace parameter_client
{

class AsyncParametersClient
{

public:
  RCLCPP_MAKE_SHARED_DEFINITIONS(AsyncParametersClient);

  AsyncParametersClient(const rclcpp::node::Node::SharedPtr & node)
  : node_(node)
  {
    get_parameters_client_ = node_->create_client<rcl_interfaces::GetParameters>(
      "get_parameters");
    get_parameter_types_client_ = node_->create_client<rcl_interfaces::GetParameterTypes>(
      "get_parameter_types");
    set_parameters_client_ = node_->create_client<rcl_interfaces::SetParameters>(
      "set_parameters");
    list_parameters_client_ = node_->create_client<rcl_interfaces::ListParameters>(
      "list_parameters");
    describe_parameters_client_ = node_->create_client<rcl_interfaces::DescribeParameters>(
      "describe_parameters");
  }

  std::shared_future<std::vector<rclcpp::parameter::ParameterVariant>>
  get_parameters(
    std::vector<std::string> names,
    std::function<void(
      std::shared_future<std::vector<rclcpp::parameter::ParameterVariant>>)> callback = nullptr)
  {
    std::shared_future<std::vector<rclcpp::parameter::ParameterVariant>> f;
    return f;
  }

  std::shared_future<std::vector<rclcpp::parameter::ParameterType>>
  get_parameter_types(
    std::vector<std::string> parameter_names,
    std::function<void(
      std::shared_future<std::vector<rclcpp::parameter::ParameterType>>)> callback = nullptr)
  {
    std::shared_future<std::vector<rclcpp::parameter::ParameterType>> f;
    return f;
  }

  std::shared_future<std::vector<rcl_interfaces::SetParametersResult>>
  set_parameters(
    std::vector<rclcpp::parameter::ParameterVariant> parameters,
    std::function<void(
      std::shared_future<std::vector<rcl_interfaces::SetParametersResult>>)> callback = nullptr)
  {
    std::shared_future<std::vector<rcl_interfaces::SetParametersResult>> f;
    return f;
  }

  std::shared_future<rcl_interfaces::SetParametersResult>
  set_parameters_atomically(
    std::vector<rclcpp::parameter::ParameterVariant> parameters,
    std::function<void(
      std::shared_future<rcl_interfaces::SetParametersResult>)> callback = nullptr)
  {
    std::shared_future<rcl_interfaces::SetParametersResult> f;
    return f;
  }

  std::shared_future<rcl_interfaces::ListParametersResult>
  list_parameters(
    std::vector<std::string> parameter_prefixes,
    uint64_t depth,
    std::function<void(
      std::shared_future<rcl_interfaces::ListParametersResult>)> callback = nullptr)
  {
    std::shared_future<rcl_interfaces::ListParametersResult> f;
    return f;
  }

private:
  const rclcpp::node::Node::SharedPtr node_;
  rclcpp::client::Client<rcl_interfaces::GetParameters>::SharedPtr get_parameters_client_;
  rclcpp::client::Client<rcl_interfaces::GetParameterTypes>::SharedPtr get_parameter_types_client_;
  rclcpp::client::Client<rcl_interfaces::SetParameters>::SharedPtr set_parameters_client_;
  rclcpp::client::Client<rcl_interfaces::SetParametersAtomically>::SharedPtr
    set_parameters_atomically_client_;
  rclcpp::client::Client<rcl_interfaces::ListParameters>::SharedPtr list_parameters_client_;
  rclcpp::client::Client<rcl_interfaces::DescribeParameters>::SharedPtr describe_parameters_client_;
};

class SyncParametersClient
{
public:
  RCLCPP_MAKE_SHARED_DEFINITIONS(SyncParametersClient);

  SyncParametersClient(
    rclcpp::node::Node::SharedPtr & node)
  : node_(node)
  {
    executor_ = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
    async_parameters_client_ = std::make_shared<AsyncParametersClient>(node);
  }

  SyncParametersClient(
    rclcpp::executor::Executor::SharedPtr & executor,
    rclcpp::node::Node::SharedPtr & node)
  : executor_(executor), node_(node)
  {
    async_parameters_client_ = std::make_shared<AsyncParametersClient>(node);
  }

  std::vector<rclcpp::parameter::ParameterVariant>
  get_parameters(std::vector<std::string> parameter_names)
  {
    auto f = async_parameters_client_->get_parameters(parameter_names);
    return rclcpp::executors::spin_node_until_future_complete(*executor_, node_, f).get();
  }

  std::vector<rclcpp::parameter::ParameterType>
  get_parameter_types(std::vector<std::string> parameter_names)
  {
    auto f = async_parameters_client_->get_parameter_types(parameter_names);
    return rclcpp::executors::spin_node_until_future_complete(*executor_, node_, f).get();
  }

  std::vector<rcl_interfaces::SetParametersResult>
  set_parameters(std::vector<rclcpp::parameter::ParameterVariant> parameters)
  {
    auto f = async_parameters_client_->set_parameters(parameters);
    return rclcpp::executors::spin_node_until_future_complete(*executor_, node_, f).get();
  }

  rcl_interfaces::SetParametersResult
  set_parameters_atomically(std::vector<rclcpp::parameter::ParameterVariant> parameters)
  {
    auto f = async_parameters_client_->set_parameters_atomically(parameters);
    return rclcpp::executors::spin_node_until_future_complete(*executor_, node_, f).get();
  }

  rcl_interfaces::ListParametersResult
  list_parameters(
    std::vector<std::string> parameter_prefixes,
    uint64_t depth)
  {
    auto f = async_parameters_client_->list_parameters(parameter_prefixes, depth);
    return rclcpp::executors::spin_node_until_future_complete(*executor_, node_, f).get();
  }

private:
  rclcpp::executor::Executor::SharedPtr executor_;
  rclcpp::node::Node::SharedPtr node_;
  AsyncParametersClient::SharedPtr async_parameters_client_;
};

} /* namespace parameter_client */

} /* namespace rclcpp */

#endif /* RCLCPP_RCLCPP_PARAMETER_CLIENT_HPP_ */