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
