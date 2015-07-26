#ifndef _TEST1_PB_H
#define _TEST1_PB_H

#include <string>
#include <vector>

namespace AA {
namespace BB {

enum FriendType {
  Human,
  Animal,
};

}  // namespace BB

class DogInfo {
 public:
  enum Sex {
    Male,
    Female,
  };

  // constructors and destructor
  DogInfo() = default;
  ~DogInfo();
  DogInfo(const DogInfo& other);  // copy constructor
  DogInfo(DogInfo&& other);  // move constructor
  DogInfo& operator=(const DogInfo& other);  // copy assignment
  DogInfo& operator=(DogInfo&& other);  // move assignment
  void Swap(DogInfo* other);

  // Access "age"
  int age() const;
  void set_age(const int age);

  // Access "name"
  const std::string& name() const;
  void set_name(const std::string& name);
  void set_name(const char* name);
  void set_name(const char* name, int size);
  std::string* mutable_name();
  void clear_name();

  // Access "height"
  dobule height() const;
  void set_height(const dobule height);

  // Access "like"
  bool like() const;
  void set_like(const bool like);

  // Access "luckynumber"
  int luckynumber_size() const;
  long long luckynumber(int index) const;
  void set_luckynumber(int index, long long luckynumber);
  void add_luckynumber(long long luckynumber);
  const std::vector<long long>& luckynumber() const;
  std::vector<long long>* mutable_luckynumber();

  // Access "sex"
  Sex sex() const;
  void set_sex(const Sex sex);
  Sex* mutable_sex();
  void set_allocated_sex(Sex* sex);
  Sex* release_sex();

 private:
  int age_;
  std::string name_;
  dobule height_;
  bool like_ = true;
  std::vector<long long> luckynumber_;
  Sex* sex_;
};

namespace BB {
namespace CC {

class DogFriendInfo {
 public:
  // constructors and destructor
  DogFriendInfo() = default;
  ~DogFriendInfo();
  DogFriendInfo(const DogFriendInfo& other);  // copy constructor
  DogFriendInfo(DogFriendInfo&& other);  // move constructor
  DogFriendInfo& operator=(const DogFriendInfo& other);  // copy assignment
  DogFriendInfo& operator=(DogFriendInfo&& other);  // move assignment
  void Swap(DogFriendInfo* other);

  // Access "name"
  const std::string& name() const;
  void set_name(const std::string& name);
  void set_name(const char* name);
  void set_name(const char* name, int size);
  std::string* mutable_name();
  void clear_name();

  // Access "years"
  int years() const;
  void set_years(const int years);

  // Access "friend_type"
  FriendType friend_type() const;
  void set_friend_type(const FriendType friend_type);

 private:
  std::string name_;
  int years_ = 3;
  FriendType friend_type_;
};

}  // namespace CC
}  // namespace BB

namespace DD {

class DogFamily {
 public:
  // constructors and destructor
  DogFamily() = default;
  ~DogFamily();
  DogFamily(const DogFamily& other);  // copy constructor
  DogFamily(DogFamily&& other);  // move constructor
  DogFamily& operator=(const DogFamily& other);  // copy assignment
  DogFamily& operator=(DogFamily&& other);  // move assignment
  void Swap(DogFamily* other);

  // Access "dogs"
  int dogs_size() const;
  const DogInfo& dogs(int index) const;
  void set_dogs(int index, const DogInfo& dogs);
  void add_dogs(const DogInfo& dogs);
  const std::vector<DogInfo>& dogs() const;
  std::vector<DogInfo>* mutable_dogs();

 private:
  std::vector<DogInfo> dogs_;
};

}  // namespace DD
}  // namespace AA


#endif  /* _TEST1_PB_H */
