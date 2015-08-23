#include <iostream>

#include "RepeatedFields.h"

void test_RepeatedField() {
  proto::RepeatedField<int> list;
  list.Add(1);
  list.Add(2);
  list.Add(3);
  list.Add(4);
  list.Add(5);
  std::cout << "list.size() = " << list.size() << std::endl;
  
  // list.pass_const_iterator();

  for (const auto& value: list) {
    std::cout << value;
  }
  std::cout << std::endl;

  const proto::RepeatedField<int> list2(list);

  for (auto it = list2.begin();
       it != list2.end();
       it++) {
    std::cout << *it;
  }
  std::cout << std::endl;

  for (const auto& value: list) {
    std::cout << value;
  }
  std::cout << std::endl;

  auto it = list.begin();
  it++;
  for (int i = 0; i < list.size() - 1; i++) {
    *it = 100 - i;
    it += 1;
  }
  if (it == list.end()) {
    std::cout << "end" << std::endl;
  }
  for (const auto& value: list) {
    std::cout << value << " ";
  }
  std::cout << std::endl;

  it = list.begin();
  proto::RepeatedField<int>::iterator cit = it;
  std::cout << *cit << std::endl;
  *it++ = 11;
  cit = it;
  int next_value = *it;
  std::cout << "next_value = " << next_value << std::endl;
  for (; cit != list.end(); cit++) {
    std::cout << *cit << " ";
  }
  std::cout << std::endl;
  for (const auto& value: list) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
}

class DogInfo {
 public:
  DogInfo() = default;
  DogInfo(std::string name, int age) : name_(name), age_(age) {}
  std::string name() const { return name_; }
  int age() const { return age_; }
  void print() const {
    std::cout << "(" << name_ << ": " << age_ << ") ";
  }
  void set_name(const int name) { name_ = name; }
  void set_age(const int age) { age_ = age; }

private:
  std::string name_ = "Unknown";
  int age_ = -1;
};

void access_const_repeated_ptr_list(
    const proto::RepeatedPtrField<DogInfo>& list) {
  for (auto& dog: list) {
    dog.print();
    // dog.set_age(4);
  }
  std::cout << std::endl;
}

void test_RepeatedPtrField() {
  proto::RepeatedPtrField<DogInfo> list;
  list.AddAllocated(new DogInfo("snoopy", 3));
  list.AddAllocated(new DogInfo("panda", 5));
  list.AddAllocated(new DogInfo("hy", 7));
  for (auto& dog: list) {
    dog.print();
    dog.set_age(2);
  }
  std::cout << std::endl;
  access_const_repeated_ptr_list(list);

  list.GetMutable(0)->set_age(3);
  list.GetMutable(1)->set_age(5);
  list.GetMutable(2)->set_age(7);
  access_const_repeated_ptr_list(list);

  list.Add();
  access_const_repeated_ptr_list(list);

  list.GetMutable(3)->set_age(11);
  access_const_repeated_ptr_list(list);
  list.RemoveLast();
  access_const_repeated_ptr_list(list);
}

int main() {
  // test_RepeatedField();
  test_RepeatedPtrField();
  return 0;
}