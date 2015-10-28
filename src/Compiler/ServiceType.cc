#include <iostream>

#include "../Utility/Strings.h"
#include "ServiceType.h"

namespace proto {
namespace ProtoParser {

ServiceType::ServiceType(std::string name, std::string package) :
    PbType(name, package) {
}

int ServiceType::AddRpcMethod(std::shared_ptr<RpcMethod> rpc) {
  if (rpc) {
    rpcs_list_.push_back(rpc);
    rpcs_map_[rpc->name()] = rpc;
    return 0;
  }
  return -1;
}

int ServiceType::NumberRpcMethods() const {
  return rpcs_list_.size();
}

const RpcMethod* ServiceType::FindRpcMethod(std::string name) const {
  if (rpcs_map_.find(name) != rpcs_map_.end()) {
    return rpcs_map_.at(name).get();
  }
  return nullptr;
}

const std::vector<std::shared_ptr<RpcMethod>>&
ServiceType::RpcMethods() const {
  return rpcs_list_;
}

void ServiceType::Print() const {
  std::cout << "Service " << name_ << " {" << std::endl;
  for (const auto& rpc: rpcs_list_) {
    rpc->Print();
  }
  std::cout  << "}" << std::endl;
}

void RpcMethod::Print() const {
  std::cout << "  rpc " << name_ << "(";
  std::string args, rets;
  for (const auto& arg: args_list_) {
    args += (arg.type() + ", ");
  }
  std::cout << args.substr(0, args.length() - 2) << ") returns (";
  for (const auto& ret: returns_list_) {
    rets += (ret.type() + ", ");
  }
  std::cout << rets.substr(0, rets.length() - 2) << ") {" << std::endl;
  for (const auto& op: options_map_) {
    std::cout << "    option " << op.first << " = " << op.second << ";"
              << std::endl;
  }
  std::cout << "  }" << std::endl;
}

void RpcMethod::AddOption(const std::string key, const std::string value) {
  options_map_[key] = value;
}

}  // namespace ProtoParser
}  // namespace Proto