#ifndef TINY_PB_H_
#define TINY_PB_H_

#include <string>
#include <vector>

#include "Proto/Message.h"
#include "Proto/Descriptor.h"
#include "Proto/RepeatedFields.h"
#include "Proto/SerializedMessage.h"

void static_init_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny();
void static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny();

namespace AA {

enum FriendType {
  Human,
  Dog,
  Panda,
  Food,
};

class FamilyInfo: public ::proto::Message {
 public:
  // constructors and destructor //
  FamilyInfo();
  ~FamilyInfo();
  FamilyInfo(const FamilyInfo& other);  // copy constructor
  FamilyInfo(FamilyInfo&& other);  // move constructor
  FamilyInfo& operator=(const FamilyInfo& other); // copy assignment
  FamilyInfo& operator=(FamilyInfo&& other);  // move assignment
  void Swap(FamilyInfo* other);  // Swap
  ::proto::Message* New() const override;  // New()
  void CopyFrom(const FamilyInfo& other);  // CopyFrom()
  void MoveFrom(FamilyInfo&& other);  // MoveFrom()
  bool Equals(const FamilyInfo& other) const;  // Compare
  // Serialize() and DeSerialize().
  ::proto::SerializedMessage* Serialize() const override;
  void DeSerialize(const char* buf, unsigned int size) override;
  static const FamilyInfo& default_instance();
  const ::proto::MessageDescriptor* GetDescriptor() const override;
  const ::proto::MessageReflection* GetReflection() const override;
  void Print(int indent_num=0) const override;

  // --- Field accessors --- //

  // "address" = 1
  bool has_address() const;
  const std::string& address() const;
  void set_address(const std::string& address);
  void set_address(const char* address);
  void set_address(const char* address, int size);
  std::string mutable_address();
  void clear_address();

  // "numberdogs" = 2
  bool has_numberdogs() const;
  int numberdogs() const;
  void set_numberdogs(int numberdogs);
  void clear_numberdogs();

 private:
  // has bits
  char has_bits_[1];
  // message fields
  std::string address_ = "Haimen Street";
  int numberdogs_ = 0;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static FamilyInfo* default_instance_;

  friend void ::static_init_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny();
  friend void ::static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny();
};

namespace BB {

class DogInfo: public ::proto::Message {
 public:
  enum Sex {
    MALE,
    FEMALE,
  };

  // constructors and destructor //
  DogInfo();
  ~DogInfo();
  DogInfo(const DogInfo& other);  // copy constructor
  DogInfo(DogInfo&& other);  // move constructor
  DogInfo& operator=(const DogInfo& other); // copy assignment
  DogInfo& operator=(DogInfo&& other);  // move assignment
  void Swap(DogInfo* other);  // Swap
  ::proto::Message* New() const override;  // New()
  void CopyFrom(const DogInfo& other);  // CopyFrom()
  void MoveFrom(DogInfo&& other);  // MoveFrom()
  bool Equals(const DogInfo& other) const;  // Compare
  // Serialize() and DeSerialize().
  ::proto::SerializedMessage* Serialize() const override;
  void DeSerialize(const char* buf, unsigned int size) override;
  static const DogInfo& default_instance();
  const ::proto::MessageDescriptor* GetDescriptor() const override;
  const ::proto::MessageReflection* GetReflection() const override;
  void Print(int indent_num=0) const override;

  // --- Field accessors --- //

  // "age" = 1
  bool has_age() const;
  int age() const;
  void set_age(int age);
  void clear_age();

  // "other_friend_type" = 2
  int other_friend_type_size() const;
  ::AA::FriendType other_friend_type(int index) const;
  void set_other_friend_type(int index, ::AA::FriendType value);
  void add_other_friend_type(::AA::FriendType value);
  void clear_other_friend_type();
  const ::proto::RepeatedField<::AA::FriendType>& other_friend_type() const;
  ::proto::RepeatedField<::AA::FriendType>& mutable_other_friend_type();

  // "name" = 3
  bool has_name() const;
  const std::string& name() const;
  void set_name(const std::string& name);
  void set_name(const char* name);
  void set_name(const char* name, int size);
  std::string mutable_name();
  void clear_name();

  // "sex" = 4
  bool has_sex() const;
  DogInfo::Sex sex() const;
  void set_sex(DogInfo::Sex sex);
  void clear_sex();

  // "weight" = 5
  int weight_size() const;
  double weight(int index) const;
  void set_weight(int index, double value);
  void add_weight(double value);
  void clear_weight();
  const ::proto::RepeatedField<double>& weight() const;
  ::proto::RepeatedField<double>& mutable_weight();

  // "best_friend_type" = 10
  bool has_best_friend_type() const;
  ::AA::FriendType best_friend_type() const;
  void set_best_friend_type(::AA::FriendType best_friend_type);
  void clear_best_friend_type();

  // "family" = 13
  bool has_family() const;
  const ::AA::FamilyInfo& family() const;
  ::AA::FamilyInfo* mutable_family();
  void set_allocated_family(::AA::FamilyInfo* family);
  ::AA::FamilyInfo* release_family();
  void clear_family();

  // "alias" = 18
  int alias_size() const;
  const std::string& alias(int index) const;
  void set_alias(int index, const std::string& value);
  void set_alias(int index, const char* value);
  void set_alias(int index, const char* value, int size);
  std::string* add_alias();
  void add_alias(const std::string& value);
  void add_alias(const char* value);
  void add_alias(const char* value, int size);
  std::string* mutable_alias(int index);
  void clear_alias();
  const ::proto::RepeatedPtrField<std::string>& alias() const;
  ::proto::RepeatedPtrField<std::string>& mutable_alias();

  // "neighbors" = 25
  int neighbors_size() const;
  const ::AA::FamilyInfo& neighbors(int index) const;
  ::AA::FamilyInfo* add_neighbors();
  ::AA::FamilyInfo* mutable_neighbors(int index);
  void clear_neighbors();
  const ::proto::RepeatedPtrField<::AA::FamilyInfo>& neighbors() const;
  ::proto::RepeatedPtrField<::AA::FamilyInfo>& mutable_neighbors();

 private:
  // has bits
  char has_bits_[4];
  // message fields
  int age_ = 0;
  ::proto::RepeatedField<::AA::FriendType> other_friend_type_;
  std::string name_ = "";
  DogInfo::Sex sex_ = DogInfo::MALE;
  ::proto::RepeatedField<double> weight_;
  ::AA::FriendType best_friend_type_ = ::AA::Human;
  ::AA::FamilyInfo* family_ = nullptr;
  ::proto::RepeatedPtrField<std::string> alias_;
  ::proto::RepeatedPtrField<::AA::FamilyInfo> neighbors_;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static DogInfo* default_instance_;

  friend void ::static_init_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny();
  friend void ::static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny();
};

}  // namespace BB
}  // namespace AA


#endif  /* TINY_PB_H_ */
