#include <iostream>
#include <memory>

#include "Compiler/Message.h"
#include "Compiler/ProtoParser.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"

#include "tiny_pb.h"

using namespace proto;

int main() {
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

  return 0;
}