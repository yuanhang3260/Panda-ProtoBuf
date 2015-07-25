#ifndef _TEST1_PB_H
#define _TEST1_PB_H

#include <string>
#include <vector>

namespace snoopy {

enum FriendType {
  Human,
  Animal,
};

class DogInfo {
 public:
  enum Sex {
    Male,
    Female,
  };

 private:
  int age;
  std::string name;
  dobule height;
  bool like = true;
  std::vector<long long> luckynumber;
  Sex sex;
};

class DogFriendInfo {
 public:
 private:
  std::string name;
  int years = 3;
  FriendType friend_type;
};

class DogFamily {
 public:
 private:
  std::vector<DogInfo> dogs;
};

}  // namespace snoopy

