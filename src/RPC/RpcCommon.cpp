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

}  // namespace RPC
