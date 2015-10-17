#include "RpcCommon.h"

namespace RPC {

RpcDescriptor::RpcDescriptor(std::string package, std::string name) :
    package_(package),
    name_(name) {
  fullname_ = package.empty()? name : package_ + "." + name;
}

RpcMethodDescriptor* RpcDescriptor::AddMethod(std::string name) {
  methods_map_.emplace(name, RpcMethodDescriptor(name));
  return &methods_map_[name];
}

const RpcMethodDescriptor*
RpcDescriptor::FindMethod(std::string name) const {
  if (methods_map_.find(name) != methods_map_.end()) {
    return &methods_map_.at(name);
  }
  return nullptr;
}

void RpcMethodDescriptor::add_option(std::string key, std::string value) {
  options_[key] = value;
  if (key == "deadline") {
    try {
      deadline_ = std::stod(value);
    }
    catch (std::exception& err) {
      return;
    }
  }
}

std::string RpcMethodDescriptor::find_option(std::string key) {
  if (options_.find(key) != options_.end()) {
    return options_.at(key);
  }
  return "";
}

}  // namespace RPC
