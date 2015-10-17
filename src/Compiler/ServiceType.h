#ifndef SERVICE_TYPE_
#define SERVICE_TYPE_

#include <string>
#include <vector>
#include <map>

#include "Type.h"

namespace proto {
namespace ProtoParser {

class RpcMethod;

class ServiceType : public PbType {
 public:
  ServiceType(std::string name, std::string package);

  FIELD_TYPE type() override { return UNDETERMINED; }

  int AddRpcMethod(std::shared_ptr<RpcMethod> rpc);
  int NumberRpcMethods() const;
  const RpcMethod* FindRpcMethod(std::string rpc_name) const;
  void Print() const;
  const std::vector<std::shared_ptr<RpcMethod>>& RpcMethods() const;

 protected:
  std::vector<std::shared_ptr<RpcMethod>> rpcs_list_;
  std::map<std::string, std::shared_ptr<RpcMethod>> rpcs_map_;
};

class RpcParam {
 public:
  RpcParam(std::string type) :
      type_(type), is_message_(false) {}
  RpcParam(std::string type, const PbType* type_class) :
      type_(type), is_message_(true), type_class_(type_class) {}
  std::string type() const { return type_; }
  const PbType* type_class() const { return type_class_; }
  bool IsMessage() const { return is_message_; }
 private:
  std::string type_;
  bool is_message_;
  const PbType* type_class_ = nullptr;
};

class RpcMethod {
 public:
  RpcMethod(std::string name) : name_(name) {}
  virtual ~RpcMethod() {}

  std::string name() const { return name_; }

  const std::vector<RpcParam>& args_list() const { return args_list_; }
  const std::vector<RpcParam>& returns_list() const { return returns_list_; }
  const std::map<std::string, std::string>& options_map() const {
    return options_map_;
  }

  void AddArg(const std::string arg_type, PbType* type_class) {
    args_list_.push_back(RpcParam(arg_type, type_class));
  }

  void AddArg(const std::string arg_type) {
    args_list_.push_back(RpcParam(arg_type));
  }
  
  void AddReturn(const std::string return_type, PbType* type_class) {
    returns_list_.push_back(RpcParam(return_type, type_class));
  }

  void AddReturn(const std::string return_type) {
    returns_list_.push_back(RpcParam(return_type));
  }

  void AddOption(const std::string key, const std::string value);

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