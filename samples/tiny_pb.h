#ifndef TINY_PB_H_
#define TINY_PB_H_

#include <string>
#include <vector>

#include "Proto/Message.h"
#include "Proto/RepeatedFields.h"
#include "Proto/SerializedMessage.h"

void static_init_samples_tiny();
void static_init_default_instances_samples_tiny();

namespace AA {

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
  std::string address_ = "Haimen Street";
  int numberdogs_ = 0;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static FamilyInfo* default_instance_;

  friend void ::static_init_samples_tiny();
  friend void ::static_init_default_instances_samples_tiny();
};

namespace BB {

class DogInfo: public ::proto::Message {
 public:
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

  // "name" = 3
  bool has_name() const;
  const std::string& name() const;
  void set_name(const std::string& name);
  void set_name(const char* name);
  void set_name(const char* name, int size);
  std::string mutable_name();
  void clear_name();

  // "weights" = 5
  int weights_size() const;
  double weights(int index);
  void set_weights(int index, double value);
  void add_weights(double value);
  void clear_weights();
  const ::proto::RepeatedField<double>& weights() const;
  ::proto::RepeatedField<double>& mutable_weights();

  // "family" = 13
  bool has_family() const;
  const ::AA::FamilyInfo& family() const;
  ::AA::FamilyInfo* mutable_family();
  void set_allocated_family(::AA::FamilyInfo* family);
  ::AA::FamilyInfo* release_family();
  void clear_family();

  // "alias" = 18
  int alias_size() const;
  const std::string& alias(int index);
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
  const ::AA::FamilyInfo& neighbors(int index);
  ::AA::FamilyInfo* add_neighbors();
  ::AA::FamilyInfo* mutable_neighbors(int index);
  void clear_neighbors();
  const ::proto::RepeatedPtrField<::AA::FamilyInfo>& neighbors() const;
  ::proto::RepeatedPtrField<::AA::FamilyInfo>& mutable_neighbors();

 private:
  // has bits
  char has_bits_[4];
  // message fields
  int age_;
  std::string name_ = "";
  ::proto::RepeatedField<double> weights_;
  ::AA::FamilyInfo* family_ = nullptr;
  ::proto::RepeatedPtrField<std::string> alias_;
  ::proto::RepeatedPtrField<::AA::FamilyInfo> neighbors_;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static DogInfo* default_instance_;

  friend void ::static_init_samples_tiny();
  friend void ::static_init_default_instances_samples_tiny();
};

}  // namespace BB
}  // namespace AA


#endif  /* TINY_PB_H_ */
