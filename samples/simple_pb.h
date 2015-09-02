#ifndef SIMPLE_PB_H_
#define SIMPLE_PB_H_

#include <string>
#include <vector>

#include "Proto/Message.h"
#include "Proto/RepeatedFields.h"
#include "Proto/SerializedMessage.h"

void static_init_samples_simple();
void static_init_default_instances_samples_simple();

namespace HaiZhong {

class Pet: public ::proto::Message {
 public:
  enum PetType {
    DOG,
    PANDA,
  };

  // constructors and destructor //
  Pet();
  ~Pet();
  Pet(const Pet& other);  // copy constructor
  Pet(Pet&& other);  // move constructor
  Pet& operator=(const Pet& other);  // copy assignment
  Pet& operator=(Pet&& other);  // move assignment
  void Swap(Pet* other);

  ::proto::Message* New() override;  // New()
  // Serialize() and DeSerialize().
  ::proto::SerializedMessage* Serialize() const override;
  void DeSerialize(const char* buf, unsigned int size) override;
  static const Pet& default_instance();

  // --- Field accessors --- //

  // "name" = 1
  bool has_name() const;
  const std::string& name() const;
  void set_name(const std::string& name);
  void set_name(const char* name);
  void set_name(const char* name, int size);
  std::string mutable_name();
  void clear_name();

  // "type" = 2
  bool has_type() const;
  Pet::PetType type() const;
  void set_type(Pet::PetType type);
  void clear_type();

 private:
  // has bits
  char has_bits_[1];
  // message fields
  std::string name_ = "";
  Pet::PetType type_ = Pet::DOG;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static Pet* default_instance_;

  friend void ::static_init_samples_simple();
  friend void ::static_init_default_instances_samples_simple();
};

class Student: public ::proto::Message {
 public:
  enum Sex {
    MALE,
    FEMALE,
  };

  // constructors and destructor //
  Student();
  ~Student();
  Student(const Student& other);  // copy constructor
  Student(Student&& other);  // move constructor
  Student& operator=(const Student& other);  // copy assignment
  Student& operator=(Student&& other);  // move assignment
  void Swap(Student* other);

  ::proto::Message* New() override;  // New()
  // Serialize() and DeSerialize().
  ::proto::SerializedMessage* Serialize() const override;
  void DeSerialize(const char* buf, unsigned int size) override;
  static const Student& default_instance();

  // --- Field accessors --- //

  // "name" = 1
  bool has_name() const;
  const std::string& name() const;
  void set_name(const std::string& name);
  void set_name(const char* name);
  void set_name(const char* name, int size);
  std::string mutable_name();
  void clear_name();

  // "age" = 2
  bool has_age() const;
  unsigned int age() const;
  void set_age(unsigned int age);
  void clear_age();

  // "xyz" = 3
  bool has_xyz() const;
  int xyz() const;
  void set_xyz(int xyz);
  void clear_xyz();

  // "def" = 4
  bool has_def() const;
  unsigned long long def() const;
  void set_def(unsigned long long def);
  void clear_def();

  // "ghi" = 5
  bool has_ghi() const;
  long long ghi() const;
  void set_ghi(long long ghi);
  void clear_ghi();

  // "graduated" = 6
  bool has_graduated() const;
  bool graduated() const;
  void set_graduated(bool graduated);
  void clear_graduated();

  // "weight" = 7
  bool has_weight() const;
  double weight() const;
  void set_weight(double weight);
  void clear_weight();

  // "pet" = 9
  bool has_pet() const;
  const Pet& pet() const;
  Pet* mutable_pet();
  void set_allocated_pet(Pet* pet);
  Pet* release_pet();
  void clear_pet();

  // "scores" = 11
  int scores_size() const;
  int scores(int index);
  void set_scores(int index, int value);
  void add_scores(int value);
  void clear_scores();
  const ::proto::RepeatedField<int>& scores() const;
  ::proto::RepeatedField<int>& mutable_scores();

  // "alias" = 15
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

  // "sex" = 25
  bool has_sex() const;
  Student::Sex sex() const;
  void set_sex(Student::Sex sex);
  void clear_sex();

 private:
  // has bits
  char has_bits_[4];
  // message fields
  std::string name_ = "";
  unsigned int age_ = 0;
  int xyz_ = 0;
  unsigned long long def_ = 0;
  long long ghi_ = 0;
  bool graduated_ = false;
  double weight_ = 0;
  Pet* pet_ = nullptr;
  ::proto::RepeatedField<int> scores_;
  ::proto::RepeatedPtrField<std::string> alias_;
  Student::Sex sex_ = Student::MALE;

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static Student* default_instance_;

  friend void ::static_init_samples_simple();
  friend void ::static_init_default_instances_samples_simple();
};

class SchoolClass: public ::proto::Message {
 public:
  // constructors and destructor //
  SchoolClass();
  ~SchoolClass();
  SchoolClass(const SchoolClass& other);  // copy constructor
  SchoolClass(SchoolClass&& other);  // move constructor
  SchoolClass& operator=(const SchoolClass& other);  // copy assignment
  SchoolClass& operator=(SchoolClass&& other);  // move assignment
  void Swap(SchoolClass* other);

  ::proto::Message* New() override;  // New()
  // Serialize() and DeSerialize().
  ::proto::SerializedMessage* Serialize() const override;
  void DeSerialize(const char* buf, unsigned int size) override;
  static const SchoolClass& default_instance();

  // --- Field accessors --- //

  // "number" = 1
  bool has_number() const;
  int number() const;
  void set_number(int number);
  void clear_number();

  // "alias" = 2
  bool has_alias() const;
  const std::string& alias() const;
  void set_alias(const std::string& alias);
  void set_alias(const char* alias);
  void set_alias(const char* alias, int size);
  std::string mutable_alias();
  void clear_alias();

 private:
  // has bits
  char has_bits_[1];
  // message fields
  int number_ = 0;
  std::string alias_ = "";

  // InitAsDefaultInstance()
  void InitAsDefaultInstance() override;
  // default instance
  static SchoolClass* default_instance_;

  friend void ::static_init_samples_simple();
  friend void ::static_init_default_instances_samples_simple();
};

}  // namespace HaiZhong


#endif  /* SIMPLE_PB_H_ */
