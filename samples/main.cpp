#include <iostream>
#include <memory>

#include "Compiler/Message.h"
#include "Compiler/ProtoParser.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"
#include "Proto/SerializedMessage.h"
#include "Proto/SerializedPrimitive.h"


#include "tiny_pb.h"
#include "simple_pb.h"

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

  ::proto::SerializedMessage* sdmsg = stu1.Serialize();
  const char* obj_data = sdmsg->GetBytes();
  for (unsigned int i = 0; i < sdmsg->size(); i++) {
    printf("0x%x ", obj_data[i] & 0xff);
  }
  printf("\n");
  
  // Single student serialization
  ::HaiZhong::Student stu2;
  stu2.DeSerialize(obj_data, sdmsg->size());
  
  PRINT_HAS(stu2, name, "stu2", "name")
  PRINT_HAS(stu2, age, "stu2", "age")
  PRINT_HAS(stu2, xyz, "stu2", "xyz")
  PRINT_HAS(stu2, def, "stu2", "def")
  PRINT_HAS(stu2, ghi, "stu2", "ghi")
  PRINT_HAS(stu2, graduated, "stu2", "graduated")
  PRINT_HAS(stu2, weight, "stu2", "weight")
  PRINT_HAS(stu2, sex, "stu2", "sex")
  PRINT_HAS(stu2, first_pet, "stu2", "first_pet")
  PRINT_HAS(stu2, partner, "stu2", "partner")
  std::cout << "stu2.name() = " << stu2.name() << std::endl;
  std::cout << "stu2.age() = " << stu2.age() << std::endl;
  std::cout << "stu2.xyz() = " << stu2.xyz() << std::endl;
  std::cout << "stu2.def() = " << stu2.def() << std::endl;
  std::cout << "stu2.ghi() = " << stu2.ghi() << std::endl;
  std::cout << "stu2.graduated() = " << stu2.graduated() << std::endl;
  std::cout << "stu2.weight() = " << stu2.weight() << std::endl;
  std::cout << "stu2.sex() = " << stu2.sex() << std::endl;
  std::cout << "stu2.scores(): [ ";
  for (const auto& score: stu2.scores()) {
    std::cout << score << " ";
  }
  std::cout << "]" << std::endl;
  std::cout << "stu2.alias(): [ ";
  for (const auto& alias: stu2.alias()) {
    std::cout << alias << " ";
  }
  std::cout << "]" << std::endl;

  std::cout << "stu2.pets(): [ ";
  for (const auto& pet: stu2.pets()) {
    std::cout << "(" << pet.name() << " ";
    std::cout << pet.type() << "), ";  
  }
  std::cout << " ]" << std::endl;
  std::cout << "stu2.first_pet.name = " << stu2.first_pet().name() << std::endl;
  std::cout << "stu2.first_pet.type = " << stu2.first_pet().type() << std::endl;

  std::cout << "stu2.partner.name = " << stu2.partner().name() << std::endl;
  std::cout << "stu2.partner.age = " << stu2.partner().age() << std::endl;
  std::cout << "stu2.partner.ghi = " << stu2.partner().ghi() << std::endl;

  std::cout << "stu2.partner.partner.name = "
            << stu2.mutable_partner()->partner().name() << std::endl;
  std::cout << "stu2.partner.partner.age = "
            << stu2.mutable_partner()->partner().age() << std::endl;
  std::cout << "stu2.partner.partner.alias[0] = "
            << stu2.partner().partner().alias(0) << std::endl;
  std::cout << "stu2.partner.partner.alias[1] = "
            << stu2.partner().partner().alias(1) << std::endl;

  std::cout << "deleting sdmsg ..." << std::endl;

  // mutiple students in class
  ::HaiZhong::SchoolClass class1;
  ::HaiZhong::Student* stu3 = class1.add_students();
  //::HaiZhong::Student* stu3 = new ::HaiZhong::Student();
  stu3->set_def(17375839784);
  PRINT_HAS((*stu3), def, "stu3", "def")
  *stu3 = (stu2);
  PRINT_HAS((*stu3), name, "stu3", "name")
  PRINT_HAS((*stu3), age, "stu3", "age")
  PRINT_HAS((*stu3), xyz, "stu3", "xyz")
  PRINT_HAS((*stu3), def, "stu3", "def")
  PRINT_HAS((*stu3), ghi, "stu3", "ghi")
  PRINT_HAS((*stu3), graduated, "stu3", "graduated")
  PRINT_HAS((*stu3), weight, "stu3", "weight")
  PRINT_HAS((*stu3), sex, "stu3", "sex")
  PRINT_HAS((*stu3), first_pet, "stu3", "first_pet")
  PRINT_HAS((*stu3), partner, "stu3", "partner")
  std::cout << "stu3.name() = " << stu3->name() << std::endl;
  std::cout << "stu3.age() = " << stu3->age() << std::endl;
  std::cout << "stu3.xyz() = " << stu3->xyz() << std::endl;
  std::cout << "stu3.def() = " << stu3->def() << std::endl;
  std::cout << "stu3.ghi() = " << stu3->ghi() << std::endl;
  std::cout << "stu3.graduated() = " << stu3->graduated() << std::endl;
  std::cout << "stu3.weight() = " << stu3->weight() << std::endl;
  std::cout << "stu3.sex() = " << stu3->sex() << std::endl;
  std::cout << "stu3.scores(): [ ";
  for (const auto& score: stu3->scores()) {
    std::cout << score << " ";
  }
  std::cout << "]" << std::endl;
  std::cout << "stu3.alias(): [ ";
  for (const auto& alias: stu3->alias()) {
    std::cout << alias << " ";
  }
  std::cout << "]" << std::endl;

  std::cout << "stu3.pets(): [ ";
  for (const auto& pet: stu3->pets()) {
    std::cout << "(" << pet.name() << " ";
    std::cout << pet.type() << "), ";  
  }
  std::cout << " ]" << std::endl;
  std::cout << "stu3.first_pet.name = " << stu3->first_pet().name() << std::endl;
  std::cout << "stu3.first_pet.type = " << stu3->first_pet().type() << std::endl;

  std::cout << "stu3.partner.name = " << stu3->partner().name() << std::endl;
  std::cout << "stu3.partner.age = " << stu3->partner().age() << std::endl;
  std::cout << "stu3.partner.ghi = " << stu3->partner().ghi() << std::endl;

  std::cout << "stu3.partner.partner.name = "
            << stu3->mutable_partner()->partner().name() << std::endl;
  std::cout << "stu3.partner.partner.age = "
            << stu3->mutable_partner()->partner().age() << std::endl;
  std::cout << "stu3.partner.partner.alias[0] = "
            << stu3->partner().partner().alias(0) << std::endl;
  std::cout << "stu3.partner.partner.alias[1] = "
            << stu3->partner().partner().alias(1) << std::endl;

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

  std::cout << "class2.captain.name() = "
            << class2.captain().name() << std::endl;
  std::cout << "class2.captain.alias(0) = "
            << class2.captain().alias(0) << std::endl;
  std::cout << "class2.captain.partner.name() = "
            << class2.captain().partner().name() << std::endl;

  int i = 0;
  for (const auto& student: class2.students()) {
    std::cout << "*** student[" << i++ << "] *** ";
    std::cout << student.name() << " " << student.age() << ", ";
    for (const auto& alias: student.alias()) {
      std::cout << alias << " ";
    }
    std::cout << std::endl;
  }

  delete sdmsg;
}

int main() {
  std::cout << "-------------------- Test ----------------------" << std::endl;
  test_Serialize();
  return 0;
}