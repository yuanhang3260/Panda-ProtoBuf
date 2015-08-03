#ifndef TEST1_PB_H_
#define TEST1_PB_H_

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

  // constructors and destructor //
  DogInfo() = default;
  ~DogInfo();
  DogInfo(const DogInfo& other);  // copy constructor
  DogInfo(DogInfo&& other);  // move constructor
  DogInfo& operator=(const DogInfo& other);  // copy assignment
  DogInfo& operator=(DogInfo&& other);  // move assignment
  void Swap(DogInfo* other);

  // --- Field accessors --- //

  // "age" = 1
  int age() const;
  void set_age(int age);
  void clear_age();

  // "name" = 2
  const std::string& name() const;
  void set_name(const std::string& name);
  void set_name(const char* name);
  void set_name(const char* name, int size);
  std::string mutable_name();
  void clear_name();

  // "height" = 3
  double height() const;
  void set_height(double height);
  void clear_height();

  // "like" = 4
  bool like() const;
  void set_like(bool like);
  void clear_like();

  // "luckynumber" = 5
  int luckynumber_size() const;
  long long luckynumber(int index);
  void set_luckynumber(int index, long long value);
  void add_luckynumber(long long value);
  void clear_luckynumber();
  const std::vector<long long> luckynumber() const;
  std::vector<long long> mutable_luckynumber();

  // "sex" = 6
  DogInfo::Sex sex() const;
  void set_sex(DogInfo::Sex sex);
  void clear_sex();

 private:
  int age_;
  std::string name_;
  double height_;
  bool like_ = true;
  std::vector<long long> luckynumber_;
  DogInfo::Sex sex_;
};

namespace BB {
namespace CC {

class DogFriendInfo {
 public:
  // constructors and destructor //
  DogFriendInfo() = default;
  ~DogFriendInfo();
  DogFriendInfo(const DogFriendInfo& other);  // copy constructor
  DogFriendInfo(DogFriendInfo&& other);  // move constructor
  DogFriendInfo& operator=(const DogFriendInfo& other);  // copy assignment
  DogFriendInfo& operator=(DogFriendInfo&& other);  // move assignment
  void Swap(DogFriendInfo* other);

  // --- Field accessors --- //

  // "name" = 1
  const std::string& name() const;
  void set_name(const std::string& name);
  void set_name(const char* name);
  void set_name(const char* name, int size);
  std::string mutable_name();
  void clear_name();

  // "years" = 2
  int years() const;
  void set_years(int years);
  void clear_years();

  // "friend_type" = 4
  AA::BB::FriendType friend_type() const;
  void set_friend_type(AA::BB::FriendType friend_type);
  void clear_friend_type();

 private:
  std::string name_;
  int years_ = 3;
  AA::BB::FriendType friend_type_;
};

}  // namespace CC
}  // namespace BB

class DogFriends {
 public:
  // constructors and destructor //
  DogFriends() = default;
  ~DogFriends();
  DogFriends(const DogFriends& other);  // copy constructor
  DogFriends(DogFriends&& other);  // move constructor
  DogFriends& operator=(const DogFriends& other);  // copy assignment
  DogFriends& operator=(DogFriends&& other);  // move assignment
  void Swap(DogFriends* other);

  // --- Field accessors --- //

  // "friends" = 1
  int friends_size() const;
  const BB::CC::DogFriendInfo& friends(int index);
  void set_friends(int index, const BB::CC::DogFriendInfo& value);
  void add_friends(const BB::CC::DogFriendInfo& value);
  BB::CC::DogFriendInfo& mutable_friends(int index);
  void clear_friends();
  const std::vector<BB::CC::DogFriendInfo> friends() const;
  std::vector<BB::CC::DogFriendInfo> mutable_friends();

  // "bestDogFriend" = 2
  const DogInfo& bestDogFriend() const;
  DogInfo* mutable_bestDogFriend();
  void set_allocated_bestDogFriend(DogInfo* bestDogFriend);
  DogInfo* release_bestDogFriend();
  void clear_bestDogFriend();

 private:
  std::vector<BB::CC::DogFriendInfo> friends_;
  DogInfo* bestDogFriend_ = nullptr;
};

namespace DD {

class DogFamily {
 public:
  // constructors and destructor //
  DogFamily() = default;
  ~DogFamily();
  DogFamily(const DogFamily& other);  // copy constructor
  DogFamily(DogFamily&& other);  // move constructor
  DogFamily& operator=(const DogFamily& other);  // copy assignment
  DogFamily& operator=(DogFamily&& other);  // move assignment
  void Swap(DogFamily* other);

  // --- Field accessors --- //

  // "dogs" = 1
  int dogs_size() const;
  const AA::DogInfo& dogs(int index);
  void set_dogs(int index, const AA::DogInfo& value);
  void add_dogs(const AA::DogInfo& value);
  AA::DogInfo& mutable_dogs(int index);
  void clear_dogs();
  const std::vector<AA::DogInfo> dogs() const;
  std::vector<AA::DogInfo> mutable_dogs();

 private:
  std::vector<AA::DogInfo> dogs_;
};

}  // namespace DD
}  // namespace AA


#endif  /* TEST1_PB_H_ */
