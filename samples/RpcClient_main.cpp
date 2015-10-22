#include <iostream>
#include <memory>

#include "Utility/CallBack.h"
#include "Compiler/Message.h"
#include "Compiler/ProtoParser.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"
#include "Proto/SerializedMessage.h"
#include "Proto/SerializedPrimitive.h"
#include "RPC/Rpc.h"
#include "RPC/RpcClientChannel.h"

#include "simple_pb.h"

using namespace HaiZhong;

class HmzxStudentSystem {
 public:
  HmzxStudentSystem() = default;
  ~HmzxStudentSystem() {
    if (sm_stub_) {
      delete sm_stub_;
    }
  }

  void SetRpcServerPort(int port) { port_ = port; }
  void Init();

  void RegisterNewStudent();

  void DeRegisterStudent();

 private:
  Student* CreateStudent();
  void DoneRegister();

  HaiZhong::StudentManagement* sm_stub_ = nullptr;
  int port_;
};

void HmzxStudentSystem::Init() {
  sm_stub_ = StudentManagement::NewStub(
      new RPC::RpcClientChannel("localhost", port_));
}

void HmzxStudentSystem::RegisterNewStudent() {
  RPC::Rpc rpc;
  StudentRequest request;
  StudentResponse response;

  request.set_op_type(StudentRequest::ADD);
  request.set_class_number(16);
  request.set_student_name("hy");
  
  // create a student
  request.set_allocated_student(CreateStudent());

  // Rpc call
  sm_stub_->AddStudent(&rpc, &request, &response,
                       new Base::Closure(&HmzxStudentSystem::DoneRegister,
                                         this));

  // Wait
  rpc.Wait();

  if (!rpc.isOk()) {
    std::cerr << rpc.client_status() << std::endl;
  }
}

Student* HmzxStudentSystem::CreateStudent() {
  Student* stu1 = new Student();
  stu1->set_name("hy");
  stu1->set_age(24);
  stu1->set_xyz(-3);
  // stu1->set_def(17375839784);
  stu1->set_ghi(-53159748968);
  stu1->set_graduated(true);
  stu1->set_weight(70.5);
  stu1->set_sex(::HaiZhong::Student::FEMALE);
  stu1->add_scores(10);
  stu1->add_scores(20);
  stu1->add_scores(30);
  stu1->add_alias("chicken");
  stu1->add_alias("xiaoji");
  return stu1;
}

void HmzxStudentSystem::DoneRegister() {
  std::cout << "Done register :)" << std::endl;
}

void HmzxStudentSystem::DeRegisterStudent() {

}

int main(int argc, char** argv) {
  int port = 9090;
  if (argc > 1) {
    port = std::stoi(argv[1]);
  }

  HmzxStudentSystem hmzx;
  hmzx.SetRpcServerPort(port);
  hmzx.Init();

  hmzx.RegisterNewStudent();
}
