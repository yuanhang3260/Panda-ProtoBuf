#include <functional>
#include <iostream>
#include <memory>

#include "Utility/CallBack.h"
#include "Utility/ThreadPool.h"
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

  int RegisterNewStudent();
  int RegisterNewStudentAsync();

  int DeRegisterStudent();

 private:
  Student* CreateStudent();
  void DoneRegister();
  void DoneRegisterAsync(RPC::Rpc* rpc,
                         StudentRequest* request,
                         StudentResponse* response);

  HaiZhong::StudentManagement* sm_stub_ = nullptr;
  int port_;
};

void HmzxStudentSystem::Init() {
  RPC::RpcStubOptions options;
  options.keep_alive = true;
  sm_stub_ = StudentManagement::NewStub(
      new RPC::RpcClientChannel("localhost", port_), options);
}

int HmzxStudentSystem::RegisterNewStudent() {
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
    std::cerr << "Rpc call status: " << rpc.RpcCallStatus() << std::endl;
    std::cerr << "Rpc return code: " << rpc.RpcReturnCode() << std::endl;
    std::cerr << "Rpc return msg: " << rpc.RpcReturnMessage() << std::endl;
    return -1;
  }

  if (response.return_code() != 0) {
    std::cerr << "StduentResponse error code: "
              << response.return_code() << std::endl;
    return -1;
  }
  return 0;
}

int HmzxStudentSystem::RegisterNewStudentAsync() {
  RPC::Rpc* rpc = new RPC::Rpc();
  StudentRequest* request = new StudentRequest();
  StudentResponse* response = new StudentResponse();

  request->set_op_type(StudentRequest::ADD);
  request->set_class_number(16);
  request->set_student_name("hy");
  
  // create a student
  request->set_allocated_student(CreateStudent());

  // Rpc call
  sm_stub_->AddStudent(rpc, request, response,
                       new Base::Closure(&HmzxStudentSystem::DoneRegisterAsync,
                                         this, rpc, request, response));
  return 0;
}

int HmzxStudentSystem::DeRegisterStudent() {
  return 0;
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
  std::cout << "Done register ~" << std::endl;
}

void HmzxStudentSystem::DoneRegisterAsync(
    RPC::Rpc* rpc, StudentRequest* request, StudentResponse* response) {
  if (!rpc->isOk()) {
    std::cerr << "Rpc call status: " << rpc->RpcCallStatus() << std::endl;
    std::cout << "\033[1;31mFailed***\033[0m" << std::endl;
    exit(0);
  }

  if (response->return_code() != 0) {
    std::cerr << "StduentResponse error code: "
              << response->return_code() << std::endl;
    std::cout << "\033[1;31mFailed***\033[0m" << std::endl;
    exit(0);
  }

  delete rpc;
  delete request;
  delete response;
  std::cout << "Done register async ~" << std::endl;
}

void LoadTest(int port) {
  HmzxStudentSystem hmzx;
  hmzx.SetRpcServerPort(port);
  hmzx.Init();

  for (int i = 0; i < 20; i++) {
    std::cout << "***** Reqeust " << i << " *****" << std::endl;
    if (hmzx.RegisterNewStudent() < 0) {
      std::cout << "\033[1;31mFailed***\033[0m" << std::endl;
      exit(1);
    }
  }
}

int main(int argc, char** argv) {
  int port = 9090;
  if (argc > 1) {
    port = std::stoi(argv[1]);
  }

  Executors::FixedThreadPool pool(30);
  pool.Start();

  for (int i = 0; i < 1000; i++) {
    pool.AddTask(std::bind(LoadTest, port));
  }

  pool.Stop();
  pool.AwaitTermination();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}
