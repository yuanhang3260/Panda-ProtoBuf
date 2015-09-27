#ifndef SERVICE_TYPE_
#define SERVICE_TYPE_

#include <string>
#include <vector>
#include <map>

#include "Type.h"

namespace proto {
namespace ProtoParser {

class RpcService;

class ServiceType : public PbType {
 public:
  ServiceType(const std::string& name, const std::string& package);

  FIELD_TYPE type() override { return UNDETERMINED; }

  int AddRpcService(std::shared_ptr<RpcService> rpc);
  int NumberRpcServices() const;
  const RpcService* FindRpcService(std::string rpc_name) const;
  void Print() const;
  const std::vector<std::shared_ptr<RpcService>>& RpcServices() const;

 protected:
  std::vector<std::shared_ptr<RpcService>> rpcs_list_;
  std::map<std::string, std::shared_ptr<RpcService>> rpcs_map_;
};

class RpcParam {
 public:
  RpcParam(const std::string& name, bool is_message) :
      name_(name), is_message_(is_message) {}
  std::string name() const { return name_; }
  bool IsMessage() const { return is_message_; }
 private:
  std::string name_;
  bool is_message_;
};

class RpcService {
 public:
  RpcService(const std::string& name) : name_(name) {}
  virtual ~RpcService() {}

  std::string name() { return name_; }
  
  void AddArg(const std::string& arg_type, bool is_message) {
    args_list_.push_back(RpcParam(arg_type, is_message));
  }
  
  void AddReturn(const std::string& return_type, bool is_message) {
    returns_list_.push_back(RpcParam(return_type, is_message));
  }
  
  void AddOption(const std::string& key, const std::string& value) {
    options_map_[key] = value;
  }
  
  void Print() const;

 private:
  std::string name_;
  std::vector<RpcParam> args_list_;
  std::vector<RpcParam> returns_list_;
  std::map<std::string, std::string> options_map_;
};


}  // namespace ProtoParser
}  // namespace Proto

#endif  /* _SERVICE_TYPE */