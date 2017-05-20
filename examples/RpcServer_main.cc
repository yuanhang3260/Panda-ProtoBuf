#include <iostream>
#include <memory>

#include "Compiler/Message.h"
#include "Compiler/ProtoParser.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"
#include "Proto/SerializedMessage.h"
#include "Proto/SerializedPrimitive.h"
#include "RPC/RpcServer.h"

#include "simple_pb.h"

using namespace HaiZhong;

class StudentManagementImpl : public StudentManagement {
 public:
  void AddStudent(RPC::Rpc* rpc,
                  const StudentRequest*,
                  StudentResponse*,
                  Base::Closure* done);

  void DeleteStudent(RPC::Rpc* rpc,
                     const StudentRequest*,
                     StudentResponse*,
                     Base::Closure* done);
};

void StudentManagementImpl::AddStudent(
      RPC::Rpc* rpc, const StudentRequest* request,
      StudentResponse* response, Base::Closure* done) {
  // TODO : implement AddStudent.
  std::cout << "op_type = " << request->op_type() << std::endl;
  std::cout << "class_number = " << request->class_number() << std::endl;
  std::cout << "student_name = " << request->student_name() << std::endl;
  std::cout << "stu1.age() = " << request->student().age() << std::endl;

  response->set_return_code(0);
  response->set_error_message("SUCCESS!");
}

void StudentManagementImpl::DeleteStudent(
      RPC::Rpc* rpc, const StudentRequest* request,
      StudentResponse* response, Base::Closure* done) {
  std::cout << "DeleteStudent not implemented" << std::endl;
}

int main(int argc, char** argv) {
  int port = 9090;
  if (argc > 1) {
    port = std::stoi(argv[1]);
  }

  // Create rpc server.
  RPC::RpcServer server(port);

  StudentManagementImpl sm_impl;
  sm_impl.RegisterToServer(&server);

  server.StartServing();
}
