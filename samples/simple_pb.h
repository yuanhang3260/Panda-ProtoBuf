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
  // constructors and destructor //
  Student();
  ~Student();
  Student(const Student& other);  // copy constructor
  Student(Student&& other);  // move constructor
  Student& operator=(const Student& other);  // copy assignment
  Student& operator=(Student&& other);  // move assignment
  void Swap(Student* other);

  ::proto::Message* New() override;  // New()
  ::proto::SerializedMessage* Serialize() const override;  // Serialize()
  void DeSerialize(const char* buf, unsigned int size) override;  // DeSerialize()
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

 private:
  // has bits
  char has_bits_[1];
  // message fields
  std::string name_ = "";
  unsigned int age_;

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
  ::proto::SerializedMessage* Serialize() const override;  // Serialize()
  void DeSerialize(const char* buf, unsigned int size) override;  // DeSerialize()
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
  int number_;
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
