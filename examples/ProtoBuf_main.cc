#include <iostream>
#include <memory>

#include "Compiler/ProtoParser.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"
#include "Proto/SerializedMessage.h"
#include "Proto/SerializedPrimitive.h"
#include "UnitTest/UnitTest.h"

#include "simple_pb.h"
#include "tiny_pb.h"

namespace proto {

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
  stu1.set_weight(67.5);
  stu1.set_symbol('h');
  stu1.set_sex(::HaiZhong::Student::MALE);
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

class MessageReflectionTest: public UnitTest {
 private:
   
 public:
  void Test_FieldAccessors() {
    AA::BB::DogInfo dog;
    AssertFalse(dog.has_age(), "has age");
    AssertFalse(dog.has_sex(), "has sex");
    AssertFalse(dog.has_best_friend_type(), "has best_friend_type");
    AssertFalse(dog.has_name(), "has name");
    AssertFalse(dog.has_family(), "has family");
    AssertEqual(0, dog.weight_size(), "weight size");
    AssertEqual(0, dog.alias_size(), "alias size");
    AssertEqual(0, dog.other_friend_type_size(), "other_friend_type size");
    AssertEqual(0, dog.neighbors_size(), "neighbors size");

    const MessageReflection* dog_reflection = dog.GetReflection();
    const MessageDescriptor* dog_descriptor = dog.GetDescriptor();
    AssertTrue(dog_reflection != nullptr);
    AssertTrue(dog_descriptor != nullptr);

    // age
    const auto age_descriptor = dog_descriptor->FindFieldByName("age");
    AssertTrue(age_descriptor != nullptr);
    dog_reflection->SetInt32(&dog, age_descriptor, 3);
    AssertTrue(dog.has_age(), "has age");
    int32 age = dog_reflection->GetInt32(&dog, age_descriptor);
    AssertEqual(3, age);
    // sex
    const auto sex_descriptor = dog_descriptor->FindFieldByName("sex");
    AssertTrue(sex_descriptor != nullptr);
    dog_reflection->SetEnum(&dog, sex_descriptor, AA::BB::DogInfo::MALE);
    AssertTrue(dog.has_sex(), "has sex");
    AA::BB::DogInfo::Sex sex = static_cast<AA::BB::DogInfo::Sex>(
                                 dog_reflection->GetEnum(&dog, sex_descriptor));
    AssertEqual(AA::BB::DogInfo::MALE, sex);
    // name
    const auto name_descriptor = dog_descriptor->FindFieldByName("name");
    AssertTrue(name_descriptor != nullptr);
    dog_reflection->SetString(&dog, name_descriptor, "snoopy");
    AssertTrue(dog.has_age(), "has name");
    std::string name = dog_reflection->GetString(&dog, name_descriptor);
    AssertEqual(std::string("snoopy"), name);
    // best_friend_type
    const auto bft_descriptor =
                    dog_descriptor->FindFieldByName("best_friend_type");
    AssertTrue(bft_descriptor != nullptr);
    dog_reflection->SetEnum(&dog, bft_descriptor, AA::FriendType::Panda);
    AssertTrue(dog.has_best_friend_type(), "has best_friend_type");
    AA::FriendType bft = static_cast<AA::FriendType>(
                              dog_reflection->GetEnum(&dog, bft_descriptor));
    AssertEqual(AA::FriendType::Panda, bft);
    // family
    const auto family_descriptor = dog_descriptor->FindFieldByName("family");
    AssertTrue(family_descriptor != nullptr);
    dog.mutable_family()->set_address("Shishan Xincun");
    dog.mutable_family()->set_numberdogs(1);
    AssertTrue(dog.has_family(), "has family 1 begin");

    const auto& family_1 = dynamic_cast<const AA::FamilyInfo&>(
                           dog_reflection->GetMessage(&dog, family_descriptor));
    AssertEqual(std::string("Shishan Xincun"), family_1.address());
    AssertEqual(1, family_1.numberdogs());
    dog.clear_family();
    AssertFalse(dog.has_family(), "has family 1 end");

    AA::FamilyInfo* family_2 = dynamic_cast<AA::FamilyInfo*>(
                                        dog_reflection->MutableMessage(
                                            &dog, family_descriptor));
    AssertTrue(dog.has_family(), "has family 2 begin");
    family_2->set_address("602-502");
    family_2->set_numberdogs(2);
    AssertEqual(std::string("602-502"), dog.family().address());
    AssertEqual(2, dog.family().numberdogs());
    AssertTrue(dog.has_family(), "has family 2 end");

    dog_reflection->SetAllocatedMessage(&dog, family_descriptor,
                                        new AA::FamilyInfo());
    AssertTrue(dog.has_family(), "has family 3 begin");
    dog.mutable_family()->set_address("Shishan Huating");
    dog.mutable_family()->set_numberdogs(3);
    AssertEqual(std::string("Shishan Huating"), dog.family().address());
    AssertEqual(3, dog.family().numberdogs());
    AssertTrue(dog.has_family(), "has family 3 end");

    AssertTrue(dog.has_family(), "has family 4 begin");
    AA::FamilyInfo* family_4 = dynamic_cast<AA::FamilyInfo*>(
                                        dog_reflection->ReleaseMessage(
                                            &dog, family_descriptor));
    AssertEqual(std::string("Shishan Huating"), family_4->address());
    AssertEqual(3, family_4->numberdogs());
    AssertFalse(dog.has_family(), "has family 4 end");

    // weight
    const auto weight_descriptor = dog_descriptor->FindFieldByName("weight");
    AssertTrue(weight_descriptor != nullptr);
    dog_reflection->AddDouble(&dog, weight_descriptor, 1.5);
    dog_reflection->AddDouble(&dog, weight_descriptor, 2.5);
    dog_reflection->AddDouble(&dog, weight_descriptor, 3.5);
    AssertEqual(3, dog.weight_size(), "weight size");
    double weight_1 = dog_reflection->
                          GetRepeatedDouble(&dog, weight_descriptor, 1);
    AssertEqual(2.5, weight_1);
    dog_reflection->SetRepeatedDouble(&dog, weight_descriptor, 2, 4.5);
    double weight_2 = dog_reflection->
                          GetRepeatedDouble(&dog, weight_descriptor, 2);
    AssertEqual(4.5, weight_2);
    // alias
    const auto alias_descriptor = dog_descriptor->FindFieldByName("alias");
    AssertTrue(alias_descriptor != nullptr);
    dog_reflection->AddString(&dog, alias_descriptor, "shinuqi");
    dog_reflection->AddString(&dog, alias_descriptor, "puppy");
    AssertEqual(2, dog.alias_size(), "alias size");
    std::string alias_0 = dog_reflection->
                          GetRepeatedString(&dog, alias_descriptor, 0);
    AssertEqual(std::string("shinuqi"), alias_0);
    dog_reflection->SetRepeatedString(&dog, alias_descriptor, 1, "fool puppy");
    std::string alias_1 = dog_reflection->
                          GetRepeatedString(&dog, alias_descriptor, 1);
    AssertEqual(std::string("fool puppy"), alias_1);
    // other_friend_type
    const auto oft_descriptor =
                    dog_descriptor->FindFieldByName("other_friend_type");
    AssertTrue(oft_descriptor != nullptr);
    dog_reflection->AddEnum(&dog, oft_descriptor, AA::FriendType::Human);
    dog_reflection->AddEnum(&dog, oft_descriptor, AA::FriendType::Food);
    dog_reflection->AddEnum(&dog, oft_descriptor, AA::FriendType::Dog);
    AssertEqual(3, dog.other_friend_type_size(), "other_friend_type size");
    AA::FriendType oft_1 = static_cast<AA::FriendType>(dog_reflection->
                                GetRepeatedEnum(&dog, oft_descriptor, 0));
    AssertEqual(AA::FriendType::Human, oft_1);
    dog_reflection->SetRepeatedEnum(&dog, oft_descriptor, 2,
                                    AA::FriendType::Panda);
    AA::FriendType oft_2 = static_cast<AA::FriendType>(dog_reflection->
                                GetRepeatedEnum(&dog, oft_descriptor, 2));
    AssertEqual(AA::FriendType::Panda, oft_2);
    
    // neighbors
    const auto neighbors_descriptor =
                    dog_descriptor->FindFieldByName("neighbors");
    AssertTrue(neighbors_descriptor != nullptr);
    dog_reflection->AddMessage(&dog, neighbors_descriptor,new AA::FamilyInfo());
    dog_reflection->AddMessage(&dog, neighbors_descriptor,new AA::FamilyInfo());
    AssertEqual(2, dog.neighbors_size(), "neighbors size");
    dog.mutable_neighbors(0)->set_address("Shishan Road");
    dog.mutable_neighbors(0)->set_numberdogs(1);
    dog.mutable_neighbors(1)->set_address("Haimen City");
    dog.mutable_neighbors(1)->set_numberdogs(2);

    const AA::FamilyInfo& neighbors_0 = dynamic_cast<const AA::FamilyInfo&>(
                                        dog_reflection->GetRepeatedMessage(
                                            &dog, neighbors_descriptor, 0));
    AssertEqual(std::string("Shishan Road"), neighbors_0.address());
    AssertEqual(1, neighbors_0.numberdogs());

    AA::FamilyInfo* neighbors_1 = dynamic_cast<AA::FamilyInfo*>(
                                      dog_reflection->MutableRepeatedMessage(
                                          &dog, neighbors_descriptor, 1));
    AssertEqual(std::string("Haimen City"), neighbors_1->address());
    AssertEqual(2, neighbors_1->numberdogs());    

    auto new_neighbor = new AA::FamilyInfo();
    dog_reflection->AddMessage(&dog, neighbors_descriptor, new_neighbor);
    new_neighbor->set_address("Unknown");
    new_neighbor->set_numberdogs(3);
    AssertEqual(3, dog.neighbors_size(), "neighbors size after AddMessage");
    AssertEqual(std::string("Unknown"), dog.neighbors(2).address());
    AssertEqual(3, dog.neighbors(2).numberdogs());
  }
};

}  // namespace proto

int main() {
  std::cout << "-------------------- Test ----------------------" << std::endl;
  proto::test_Serialize();

  proto::MessageReflectionTest test;
  test.setup();

  test.Test_FieldAccessors();

  test.teardown();
  std::cout << "\033[2;32mAll Passed ^_^\033[0m" << std::endl;
  return 0;
}
