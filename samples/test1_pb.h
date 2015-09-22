#ifndef TEST1_PB_H_
#define TEST1_PB_H_

#include <string>
#include <vector>

#include "Proto/Message.h"
#include "Proto/RepeatedFields.h"
#include "Proto/SerializedMessage.h"

void static_init_samples_test1();
void static_init_default_instances_samples_test1();

namespace AA {
namespace BB {

enum FriendType {
  Human,
  Animal,
};

}  // namespace BB

class DogInfo: public ::proto::Message {
 public:
  enum Sex {
    Male,
    Female,
  };

  // constructors and destructor //
  DogInfo();
  ~DogInfo();
  DogInfo(const DogInfo& other);  // copy constructor
  DogInfo(DogInfo&& other);  // move constructor
  DogInfo& operator=(const DogInfo& other);  // copy assignment
  DogInfo& operator=(DogInfo&& other);  // move assignment
  void Swap(DogInfo* other);

  ::proto::Message* New() const override;  // New()
  void CopyFrom(const DogInfo& other);  // CopyFrom()
  void MoveFrom(DogInfo&& other);  // MoveFrom()
  // Serialize() and DeSerialize().
  ::proto::SerializedMessage* Serialize() const override;
  void DeSerialize(const char* buf, unsigned int size) override;
  static const DogInfo& default_instance();

  // --- Field accessors --- //

  // "age" = 1
  bool has_age() const;
  int age() const;
  void set_age(int age);
  void clear_age();

  // "name" = 2
  bool has_name() const;
  const std::string& name() const;
  void set_name(const std::string& name);
  void set_name(const char* name);
  void set_name(const char* name, int size);
  std::string mutable_name();
  void clear_name();

  // "height" = 3
  bool has_height() const;
  double height() const;
  void set_height(double height);
  void clear_height();

  // "like" = 4
  bool has_like() const;
  bool like() const;
  void set_like(bool like);
  void clear_like();

  // "luckynumber" = 5
  int luckynumber_size() const;
  long long luckynumber(int index) const;
  void set_luckynumber(int index, long long value);
  void add_luckynumber(long long value);
  void clear_luckynumber();
  const ::proto::RepeatedField<long long>& luckynumber() const;
  ::proto::RepeatedField<long long>& mutable_luckynumber();

  // "sex" = 6
  bool has_sex() const;
  DogInfo::Sex sex() const;
  void set_sex(DogInfo::Sex sex);
  void clear_sex();

 private:
  // has bits
  char has_bits_[1];
  // message fields
  int age_ = 0;
  std::string name_ = "";
  double height_ = 0;
  bool like_ = true;
  ::proto::RepeatedField<long long> luckynumber_;
  DogInfo::Sex sex_ = DogInfo::Male;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static DogInfo* default_instance_;

  friend void ::static_init_samples_test1();
  friend void ::static_init_default_instances_samples_test1();
};

namespace BB {
namespace CC {

class DogFriendInfo: public ::proto::Message {
 public:
  // constructors and destructor //
  DogFriendInfo();
  ~DogFriendInfo();
  DogFriendInfo(const DogFriendInfo& other);  // copy constructor
  DogFriendInfo(DogFriendInfo&& other);  // move constructor
  DogFriendInfo& operator=(const DogFriendInfo& other);  // copy assignment
  DogFriendInfo& operator=(DogFriendInfo&& other);  // move assignment
  void Swap(DogFriendInfo* other);

  ::proto::Message* New() const override;  // New()
  void CopyFrom(const DogFriendInfo& other);  // CopyFrom()
  void MoveFrom(DogFriendInfo&& other);  // MoveFrom()
  // Serialize() and DeSerialize().
  ::proto::SerializedMessage* Serialize() const override;
  void DeSerialize(const char* buf, unsigned int size) override;
  static const DogFriendInfo& default_instance();

  // --- Field accessors --- //

  // "name" = 1
  bool has_name() const;
  const std::string& name() const;
  void set_name(const std::string& name);
  void set_name(const char* name);
  void set_name(const char* name, int size);
  std::string mutable_name();
  void clear_name();

  // "years" = 2
  bool has_years() const;
  int years() const;
  void set_years(int years);
  void clear_years();

  // "friend_type" = 4
  bool has_friend_type() const;
  ::AA::BB::FriendType friend_type() const;
  void set_friend_type(::AA::BB::FriendType friend_type);
  void clear_friend_type();

 private:
  // has bits
  char has_bits_[1];
  // message fields
  std::string name_ = "";
  int years_ = 3;
  ::AA::BB::FriendType friend_type_ = ::AA::BB::Human;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static DogFriendInfo* default_instance_;

  friend void ::static_init_samples_test1();
  friend void ::static_init_default_instances_samples_test1();
};

}  // namespace CC
}  // namespace BB

class DogFriends: public ::proto::Message {
 public:
  // constructors and destructor //
  DogFriends();
  ~DogFriends();
  DogFriends(const DogFriends& other);  // copy constructor
  DogFriends(DogFriends&& other);  // move constructor
  DogFriends& operator=(const DogFriends& other);  // copy assignment
  DogFriends& operator=(DogFriends&& other);  // move assignment
  void Swap(DogFriends* other);

  ::proto::Message* New() const override;  // New()
  void CopyFrom(const DogFriends& other);  // CopyFrom()
  void MoveFrom(DogFriends&& other);  // MoveFrom()
  // Serialize() and DeSerialize().
  ::proto::SerializedMessage* Serialize() const override;
  void DeSerialize(const char* buf, unsigned int size) override;
  static const DogFriends& default_instance();

  // --- Field accessors --- //

  // "friends" = 1
  int friends_size() const;
  const BB::CC::DogFriendInfo& friends(int index) const;
  BB::CC::DogFriendInfo* add_friends();
  BB::CC::DogFriendInfo* mutable_friends(int index);
  void clear_friends();
  const ::proto::RepeatedPtrField<BB::CC::DogFriendInfo>& friends() const;
  ::proto::RepeatedPtrField<BB::CC::DogFriendInfo>& mutable_friends();

  // "bestDogFriend" = 2
  bool has_bestDogFriend() const;
  const DogInfo& bestDogFriend() const;
  DogInfo* mutable_bestDogFriend();
  void set_allocated_bestDogFriend(DogInfo* bestDogFriend);
  DogInfo* release_bestDogFriend();
  void clear_bestDogFriend();

 private:
  // has bits
  char has_bits_[1];
  // message fields
  ::proto::RepeatedPtrField<BB::CC::DogFriendInfo> friends_;
  DogInfo* bestDogFriend_ = nullptr;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static DogFriends* default_instance_;

  friend void ::static_init_samples_test1();
  friend void ::static_init_default_instances_samples_test1();
};

namespace DD {

class DogFamily: public ::proto::Message {
 public:
  // constructors and destructor //
  DogFamily();
  ~DogFamily();
  DogFamily(const DogFamily& other);  // copy constructor
  DogFamily(DogFamily&& other);  // move constructor
  DogFamily& operator=(const DogFamily& other);  // copy assignment
  DogFamily& operator=(DogFamily&& other);  // move assignment
  void Swap(DogFamily* other);

  ::proto::Message* New() const override;  // New()
  void CopyFrom(const DogFamily& other);  // CopyFrom()
  void MoveFrom(DogFamily&& other);  // MoveFrom()
  // Serialize() and DeSerialize().
  ::proto::SerializedMessage* Serialize() const override;
  void DeSerialize(const char* buf, unsigned int size) override;
  static const DogFamily& default_instance();

  // --- Field accessors --- //

  // "dogs" = 1
  int dogs_size() const;
  const ::AA::DogInfo& dogs(int index) const;
  ::AA::DogInfo* add_dogs();
  ::AA::DogInfo* mutable_dogs(int index);
  void clear_dogs();
  const ::proto::RepeatedPtrField<::AA::DogInfo>& dogs() const;
  ::proto::RepeatedPtrField<::AA::DogInfo>& mutable_dogs();

 private:
  // has bits
  char has_bits_[1];
  // message fields
  ::proto::RepeatedPtrField<::AA::DogInfo> dogs_;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static DogFamily* default_instance_;

  friend void ::static_init_samples_test1();
  friend void ::static_init_default_instances_samples_test1();
};

}  // namespace DD
}  // namespace AA


#endif  /* TEST1_PB_H_ */
