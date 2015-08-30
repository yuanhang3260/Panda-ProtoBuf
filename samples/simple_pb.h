#ifndef SIMPLE_PB_H_
#define SIMPLE_PB_H_

#include <string>
#include <vector>

#include "Proto/Message.h"
#include "Proto/RepeatedFields.h"
#include "Proto/SerializedMessage.h"

void static_init_samples_simple();
void static_init_default_instances_samples_simple();

namespace AA {

enum FriendType {
  Human,
  Animal,
};

namespace BB {

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

  ::proto::Message* New() override;  // New()
  ::proto::SerializedMessage* Serialize() override;  // Serialize()
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

  // "friend_type" = 4
  bool has_friend_type() const;
  ::AA::FriendType friend_type() const;
  void set_friend_type(::AA::FriendType friend_type);
  void clear_friend_type();

  // "sex" = 6
  bool has_sex() const;
  DogInfo::Sex sex() const;
  void set_sex(DogInfo::Sex sex);
  void clear_sex();

 private:
  // has bits
  char has_bits_[1];
  // message fields
  int age_;
  std::string name_ = "";
  ::AA::FriendType friend_type_ = ::AA::Human;
  DogInfo::Sex sex_ = DogInfo::Male;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static DogInfo* default_instance_;

  friend void ::static_init_samples_simple();
  friend void ::static_init_default_instances_samples_simple();
};

}  // namespace BB

class FamilyInfo: public ::proto::Message {
 public:
  // constructors and destructor //
  FamilyInfo();
  ~FamilyInfo();
  FamilyInfo(const FamilyInfo& other);  // copy constructor
  FamilyInfo(FamilyInfo&& other);  // move constructor
  FamilyInfo& operator=(const FamilyInfo& other);  // copy assignment
  FamilyInfo& operator=(FamilyInfo&& other);  // move assignment
  void Swap(FamilyInfo* other);

  ::proto::Message* New() override;  // New()
  ::proto::SerializedMessage* Serialize() override;  // Serialize()
  static const FamilyInfo& default_instance();

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
  std::string address_ = "";
  int numberdogs_;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static FamilyInfo* default_instance_;

  friend void ::static_init_samples_simple();
  friend void ::static_init_default_instances_samples_simple();
};

namespace BB {
namespace CC {

class DogFamilyInfo: public ::proto::Message {
 public:
  // constructors and destructor //
  DogFamilyInfo();
  ~DogFamilyInfo();
  DogFamilyInfo(const DogFamilyInfo& other);  // copy constructor
  DogFamilyInfo(DogFamilyInfo&& other);  // move constructor
  DogFamilyInfo& operator=(const DogFamilyInfo& other);  // copy assignment
  DogFamilyInfo& operator=(DogFamilyInfo&& other);  // move assignment
  void Swap(DogFamilyInfo* other);

  ::proto::Message* New() override;  // New()
  ::proto::SerializedMessage* Serialize() override;  // Serialize()
  static const DogFamilyInfo& default_instance();

  // --- Field accessors --- //

  // "family" = 1
  bool has_family() const;
  const ::AA::FamilyInfo& family() const;
  ::AA::FamilyInfo* mutable_family();
  void set_allocated_family(::AA::FamilyInfo* family);
  ::AA::FamilyInfo* release_family();
  void clear_family();

  // "utility_fee" = 2
  int utility_fee_size() const;
  double utility_fee(int index);
  void set_utility_fee(int index, double value);
  void add_utility_fee(double value);
  void clear_utility_fee();
  const ::proto::RepeatedField<double>& utility_fee() const;
  ::proto::RepeatedField<double>& mutable_utility_fee();

  // "dogs" = 3
  int dogs_size() const;
  const ::AA::BB::DogInfo& dogs(int index);
  ::AA::BB::DogInfo* add_dogs();
  ::AA::BB::DogInfo* mutable_dogs(int index);
  void clear_dogs();
  const ::proto::RepeatedPtrField<::AA::BB::DogInfo>& dogs() const;
  ::proto::RepeatedPtrField<::AA::BB::DogInfo>& mutable_dogs();

  // "locations" = 4
  int locations_size() const;
  const std::string& locations(int index);
  void set_locations(int index, const std::string& value);
  void set_locations(int index, const char* value);
  void set_locations(int index, const char* value, int size);
  std::string* add_locations();
  void add_locations(const std::string& value);
  void add_locations(const char* value);
  void add_locations(const char* value, int size);
  std::string* mutable_locations(int index);
  void clear_locations();
  const ::proto::RepeatedPtrField<std::string>& locations() const;
  ::proto::RepeatedPtrField<std::string>& mutable_locations();

 private:
  // has bits
  char has_bits_[1];
  // message fields
  ::AA::FamilyInfo* family_ = nullptr;
  ::proto::RepeatedField<double> utility_fee_;
  ::proto::RepeatedPtrField<::AA::BB::DogInfo> dogs_;
  ::proto::RepeatedPtrField<std::string> locations_;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static DogFamilyInfo* default_instance_;

  friend void ::static_init_samples_simple();
  friend void ::static_init_default_instances_samples_simple();
};

}  // namespace CC
}  // namespace BB
}  // namespace AA


#endif  /* SIMPLE_PB_H_ */
