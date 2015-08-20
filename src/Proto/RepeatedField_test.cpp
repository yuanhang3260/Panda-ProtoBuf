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
  for (const auto& value: list) {
    std::cout << value;
  }
  std::cout << std::endl;
  
  for (proto::RepeatedField<int>::iterator it = list.begin();
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
  for (int i = 0; i < list.size(); i++) {
    it[i] = 9;
  }
  for (const auto& value: list) {
    std::cout << value;
  }
  std::cout << std::endl;
  return 0;
}