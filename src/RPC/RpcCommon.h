#ifndef RPC_COMMON_
#define RPC_COMMON_

#include <string>
#include <vector>
#include <map>

namespace RPC {

typedef unsigned int uint32;

class RpcDescriptor;
class RpcMethodDescriptor;
class RpcStubOptions;

class RpcDescriptor {
 public:
  RpcDescriptor(std::string package, std::string name);

  std::string package() const { return package_; }
  std::string name() const { return name_; }
  std::string fullname() const { return fullname_; }

  RpcMethodDescriptor* AddMethod(std::string name);
  const RpcMethodDescriptor* FindMethod(std::string name) const;

 private:
  std::string package_;
  std::string name_;
  std::string fullname_;
  std::map<std::string, RpcMethodDescriptor> methods_map_; ;
};

class RpcMethodDescriptor {
 public:
  RpcMethodDescriptor() = default;
  RpcMethodDescriptor(std::string name) : name_(name) {}

  std::string name() const { return name_; }
  void set_name(std::string name) { name_ = name; }

  std::string arg_type() const { return arg_type_; }
  void set_arg_type(std::string arg_type) { arg_type_ = arg_type; }

  std::string return_type() const { return return_type_; }
  void set_return_type(std::string return_type) { return_type_ = return_type; }

  void add_option(std::string key, std::string value);

  std::string find_option(std::string key);

 private:
  std::string name_;
  std::string arg_type_;
  std::string return_type_;
  double deadline_;
  std::map<std::string, std::string> options_;
};

// ----------------------------- RpcStubOptions ----------------------------- //
class RpcStubOptions {
 public:
  bool keep_alive = false;
};

}  // namespace RPC

#endif  /* RPC_COMMON_ */