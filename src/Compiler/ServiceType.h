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
  RpcParam(const std::string& type) :
      type_(type), is_message_(false) {}
  RpcParam(const std::string& type, const PbType* type_class) :
      type_(type), is_message_(true), type_class_(type_class) {}
  std::string type() const { return type_; }
  const PbType* type_class() { return type_class_; }
  bool IsMessage() const { return is_message_; }
 private:
  std::string type_;
  bool is_message_;
  const PbType* type_class_ = nullptr;
};

class RpcService {
 public:
  RpcService(const std::string& name) : name_(name) {}
  virtual ~RpcService() {}

  std::string name() { return name_; }

  std::vector<RpcParam>& args_list() { return args_list_; }
  std::vector<RpcParam>& returns_list() { return returns_list_; }

  void AddArg(const std::string& arg_type, PbType* type_class) {
    args_list_.push_back(RpcParam(arg_type, type_class));
  }

  void AddArg(const std::string& arg_type) {
    args_list_.push_back(RpcParam(arg_type));
  }
  
  void AddReturn(const std::string& return_type, PbType* type_class) {
    returns_list_.push_back(RpcParam(return_type, type_class));
  }

  void AddReturn(const std::string& return_type) {
    returns_list_.push_back(RpcParam(return_type));
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