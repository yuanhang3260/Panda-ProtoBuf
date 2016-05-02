#include <iostream>
#include <memory>

#include "Compiler/ProtoParser.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"
#include "Proto/SerializedMessage.h"
#include "Proto/SerializedPrimitive.h"

#include "simple_pb.h"
#include "tiny_pb.h"

using namespace proto;

void test_Basic() {
  // test singular numeric field;
  AA::BB::DogInfo dog1;
  std::cout << dog1.has_age() << std::endl;
  dog1.set_name("snoopy");
  dog1.set_age(3);
  std::cout << dog1.has_age() << std::endl;
  dog1.clear_age();
  std::cout << dog1.has_age() << std::endl;
  std::cout << std::endl;

  // test singular message field
  std::cout << dog1.has_family() << std::endl;
  std::cout << "dog1.family ptr = " << dog1.mutable_family() << std::endl;
  const AA::FamilyInfo& family = dog1.family();
  std::cout << "default family: (" << family.address() << ", "
            << family.numberdogs() << ") " << std::endl;
  std::cout << dog1.has_family() << std::endl;
  dog1.clear_family();
  std::cout << dog1.has_family() << std::endl;
  
  AA::FamilyInfo* new_family = new AA::FamilyInfo();
  new_family->set_address("Shishan Road");
  new_family->set_numberdogs(1);
  std::cout << "setting allocated family Shishan Road" << std::endl;
  dog1.set_allocated_family(new_family);
  std::cout << dog1.has_family() << std::endl;
  auto fp = dog1.release_family();
  std::cout << "released family: (" << fp->address() << ", "
            << fp->numberdogs() << ") " << std::endl;
  delete fp;
  std::cout << dog1.has_family() << std::endl;
  std::cout << std::endl << std::endl;

  // test repeated string field
  dog1.add_alias("doggy");
  dog1.add_alias("snooqi");
  std::cout << "dog1 alias: ";
  for (auto& alias: dog1.alias()) {
    std::cout << alias << " ";
  }
  std::cout << std::endl << std::endl;

  // test repeated message field
  AA::FamilyInfo* new_neighbor = dog1.add_neighbors();
  new_neighbor->set_address("Shishan Huating");
  new_neighbor->set_numberdogs(1);
  new_neighbor = dog1.add_neighbors();
  new_neighbor->set_address("Fumin Xiaoqu");
  new_neighbor->set_numberdogs(2);
  for (auto& neighbor: dog1.neighbors()) {
    std::cout << "(" << neighbor.address() << ", "
              << neighbor.numberdogs()
              << ") ";
  }
  std::cout << std::endl;

  AA::FamilyInfo* neighbor = dog1.mutable_neighbors(1);
  neighbor->set_address("Chengbei Xincun");
  for (auto& neighbor: dog1.neighbors()) {
    std::cout << "(" << neighbor.address() << ", "
              << neighbor.numberdogs()
              << ") ";
  }
  std::cout << std::endl;
}

#define PRINT_HAS(obj, field, obj_name, field_name)                       \
  if (obj.has_##field()) {                                                \
    std::cout << obj_name << " has " << field_name << std::endl;          \
  }                                                                       \
  else {                                                                  \
    std::cout << obj_name << " not has " << field_name << std::endl;      \
  }

void test_Serialize() {
  std::cout << __FUNCTION__ << "() ..." << std::endl;

  ::HaiZhong::Student stu1;
  stu1.set_name("hy");
  stu1.set_age(24);
  stu1.set_xyz(-3);
  // stu1.set_def(17375839784);
  stu1.set_ghi(-53159748968);
  stu1.set_graduated(true);
  stu1.set_weight(70.5);
  stu1.set_sex(::HaiZhong::Student::FEMALE);
  stu1.add_scores(10);
  stu1.add_scores(20);
  stu1.add_scores(30);
  stu1.add_alias("chicken");
  stu1.add_alias("xiaoji");
  
  ::HaiZhong::Pet* pet = new ::HaiZhong::Pet();
  pet->set_name("panda");
  pet->set_type(::HaiZhong::Pet::PANDA);
  stu1.set_allocated_first_pet(pet);
  
  pet = stu1.add_pets();
  pet->set_name("snoopy");
  pet->set_type(::HaiZhong::Pet::DOG);
  pet = stu1.add_pets();;
  pet->set_name("xiaoxiong");
  pet->set_type(::HaiZhong::Pet::PANDA);
  pet->add_friend_to(::HaiZhong::Pet::PANDA);
  pet->add_friend_to(::HaiZhong::Pet::DOG);

  ::HaiZhong::Student* partner = new ::HaiZhong::Student();
  partner->set_name("cb");
  partner->set_age(25);
  partner->set_ghi(-111212);
  stu1.set_allocated_partner(partner);

  ::HaiZhong::Student* partner2 = new ::HaiZhong::Student();
  partner2->set_name("gh");
  partner2->set_age(25);
  partner2->add_alias("marpy");
  partner2->add_alias("pi");
  stu1.mutable_partner()->set_allocated_partner(partner2);

  stu1.Print(0);

  ::proto::SerializedMessage* sdmsg = stu1.Serialize();
  const char* obj_data = sdmsg->GetBytes();
  for (unsigned int i = 0; i < sdmsg->size(); i++) {
    printf("0x%x ", obj_data[i] & 0xff);
  }
  printf("\n");
  
  // Single student serialization
  ::HaiZhong::Student stu2;
  stu2.DeSerialize(obj_data, sdmsg->size());
  
  if (!stu1.Equals(stu2)) {
    std::cerr << "\033[1;31mERROR\033[0m: deserialized stu2 != stu1"
              << std::endl;
    return;
  }
  else {
    std::cout << "\033[2;32mSUCCESS\033[0m: stu1 == stu2 ^_^" << std::endl;
  }

  // mutiple students in class
  ::HaiZhong::SchoolClass class1;
  ::HaiZhong::Student* stu3 = class1.add_students();
  *stu3 = std::move(stu2);
  if (!stu1.Equals(*stu3)) {
    std::cerr << "\033[1;31mERROR\033[0m: moved to stu3 != stu1" << std::endl;
    return;
  }
  else {
    std::cout << "\033[2;32mSuccess\033[0m: stu1 == stu3 ^_^" << std::endl;
  }

  ::HaiZhong::Student* stu4 = class1.add_students();
  stu4->set_name("rjj");
  stu4->set_age(25);
  stu4->add_alias("biao");
  stu4->add_alias("zhazi");

  ::HaiZhong::Student* stu5 = class1.add_students();
  stu5->set_name("zq");
  stu5->set_age(25);
  stu5->add_alias("xiaonn");

  class1.set_number(16);

  ::HaiZhong::Student* stu6 = new ::HaiZhong::Student();
  stu6->set_name("wty");
  stu6->add_alias("fayu");
  ::HaiZhong::Student* stu7 = new ::HaiZhong::Student();
  stu6->set_allocated_partner(stu7);
  stu7->set_name("yy");
  stu7->add_scores(-3);
  class1.set_allocated_captain(stu6);

  // serialize class 1 to class 2
  ::proto::SerializedMessage* sdmsg2 = class1.Serialize();
  const char* obj_data2 = sdmsg2->GetBytes();
  for (unsigned int i = 0; i < sdmsg2->size(); i++) {
    printf("0x%x ", obj_data2[i] & 0xff);
  }
  printf("\n");

  // Whole class de-serialization
  ::HaiZhong::SchoolClass class2;
  class2.DeSerialize(obj_data2, sdmsg2->size());
  if (!class1.Equals(class2)) {
    std::cerr << "\033[1;31mERROR\033[0m: deserialized class2 != class1"
              << std::endl;
    return;
  }
  else {
    std::cout << "\033[2;32mSUCCESS\033[0m: class1 == class2 ^_^" << std::endl;
  }

  class2.Print();

  delete sdmsg;
}

int main() {
  std::cout << "-------------------- Test ----------------------" << std::endl;
  test_Serialize();

  std::cout << "\033[2;32mAll Passed ^_^\033[0m" << std::endl;
  return 0;
}
