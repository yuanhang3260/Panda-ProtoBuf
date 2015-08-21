#include <iostream>

#include "RepeatedFields.h"

int main() {
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

  for (proto::RepeatedField<int>::const_iterator it = list.begin();
       it != list.end();
       it++) {
    std::cout << *it;
    *it = 7;
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
  return 0;
}