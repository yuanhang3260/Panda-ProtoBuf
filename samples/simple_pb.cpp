#include <memory>

#include "Compiler/Message.h"
#include "Compiler/ProtoParser.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"

#include "simple_pb.h"

namespace {

std::shared_ptr<::proto::ProtoParser::Message> Student_descriptor_;
std::shared_ptr<::proto::MessageReflection> Student_reflection_;
std::shared_ptr<::proto::ProtoParser::Message> SchoolClass_descriptor_;
std::shared_ptr<::proto::MessageReflection> SchoolClass_reflection_;

}  // namepsace

void static_init_default_instances_samples_simple() {
  static bool already_called = false;
  if (already_called) return;
  already_called = true;

  if (HaiZhong::Student::default_instance_ == NULL) {
    HaiZhong::Student::default_instance_ = new HaiZhong::Student();
    HaiZhong::Student::default_instance_->InitAsDefaultInstance();
  }
  if (HaiZhong::SchoolClass::default_instance_ == NULL) {
    HaiZhong::SchoolClass::default_instance_ = new HaiZhong::SchoolClass();
    HaiZhong::SchoolClass::default_instance_->InitAsDefaultInstance();
  }
}

void static_init_samples_simple() {
  ::proto::ProtoParser::ProtoParser parser(
      ::proto::ProtoParser::CPP,
      "./samples/simple.proto");
  CHECK(parser.ParseProto(),
        "static class initialization for ./samples/simple.proto failed");

  static_init_default_instances_samples_simple();

  int i = 0;
  // static init for class Student
  static const int Student_offsets_[2] = {
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, name_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, age_),
  };
  i = 0;
  for (auto& field: parser.mutable_messages_list()[0]->mutable_fields_list()) {
    field->set_field_offset(Student_offsets_[i++]);
  }
  Student_descriptor_ = parser.mutable_messages_list()[0];
  Student_reflection_.reset(
      new ::proto::MessageReflection(
          Student_descriptor_,
          HaiZhong::Student::default_instance_,
          PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, has_bits_))
  );
  ::proto::MessageFactory::RegisterGeneratedMessage(Student_reflection_);

  // static init for class SchoolClass
  static const int SchoolClass_offsets_[2] = {
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::SchoolClass, number_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::SchoolClass, alias_),
  };
  i = 0;
  for (auto& field: parser.mutable_messages_list()[1]->mutable_fields_list()) {
    field->set_field_offset(SchoolClass_offsets_[i++]);
  }
  SchoolClass_descriptor_ = parser.mutable_messages_list()[1];
  SchoolClass_reflection_.reset(
      new ::proto::MessageReflection(
          SchoolClass_descriptor_,
          HaiZhong::SchoolClass::default_instance_,
          PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::SchoolClass, has_bits_))
  );
  ::proto::MessageFactory::RegisterGeneratedMessage(SchoolClass_reflection_);

}

// Force static_init_samples_simple() to be called at initialization time.
struct static_init_forcer_samples_simple {
  static_init_forcer_samples_simple() {
    static_init_samples_simple();
  }
} static_init_forcer_samples_simple_obj_;


namespace HaiZhong {

// -------------------- Student --------------------- //
// constructor
Student::Student() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
Student::Student(const Student& other) {
  name_ = other.name();
  age_ = other.age();
}

// move constructor
Student::Student(Student&& other) {
  name_ = std::move(other.mutable_name());
  age_ = other.age();
  other.clear_age();
}

// copy assignment
Student& Student::operator=(const Student& other) {
  name_ = other.name();
  age_ = other.age();
  return *this;
}

// move assignment
Student& Student::operator=(Student&& other) {
  name_ = std::move(other.mutable_name());
  age_ = other.age();
  other.clear_age();
  return *this;
}

// New()
::proto::Message* Student::New() {
  return reinterpret_cast<::proto::Message*>(new Student());
}

// Serialize()
::proto::SerializedMessage* Student::Serialize() const {
  return Student_reflection_->Serialize(this);
}

// DeSerialize()
void Student::DeSerialize(const char* buf, unsigned int size) {
  Student_reflection_->DeSerialize(this, buf, size);
}

// InitAsDefaultInstance()
void Student::InitAsDefaultInstance() {
}

// swapper
void Student::Swap(Student* other) {
  std::string name_tmp__ = std::move(other->mutable_name());
  other->mutable_name() = std::move(name_);
  name_ = std::move(name_tmp__);

  unsigned int age_tmp__ = other->age();
  other->set_age(age_);
  set_age(age_tmp__);
}

// default_instance()
const Student& Student::default_instance() {
  if (default_instance_ == NULL) {
    static_init_default_instances_samples_simple();
  }
  return *default_instance_;
}

Student* Student::default_instance_ = NULL;

// destructor
Student::~Student() {
}

// "name" = 1
bool Student::has_name() const {
  return (has_bits_[0] & 0x2) != 0;
}

const std::string& Student::name() const {
  return name_;
}

void Student::set_name(const std::string& name) {
  name_ = name;
  has_bits_[0] |= 0x2;
}

void Student::set_name(const char* name) {
  name_ = std::string(name);
  has_bits_[0] |= 0x2;
}

void Student::set_name(const char* name, int size) {
  name_ = std::string(name, size);
  has_bits_[0] |= 0x2;
}

std::string Student::mutable_name() {
  return name_;
}

void Student::clear_name() {
  name_ = "";
  has_bits_[0] &= (~0x2);
}

// "age" = 2
bool Student::has_age() const {
  return (has_bits_[0] & 0x4) != 0;
}

unsigned int Student::age() const {
  return age_;
}

void Student::set_age(unsigned int age) {
  age_ = age;
  has_bits_[0] |= 0x4;
}

void Student::clear_age() {
  age_ = 0;
  has_bits_[0] &= (~0x4);
}

// -------------------- SchoolClass --------------------- //
// constructor
SchoolClass::SchoolClass() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
SchoolClass::SchoolClass(const SchoolClass& other) {
  number_ = other.number();
  alias_ = other.alias();
}

// move constructor
SchoolClass::SchoolClass(SchoolClass&& other) {
  number_ = other.number();
  other.clear_number();
  alias_ = std::move(other.mutable_alias());
}

// copy assignment
SchoolClass& SchoolClass::operator=(const SchoolClass& other) {
  number_ = other.number();
  alias_ = other.alias();
  return *this;
}

// move assignment
SchoolClass& SchoolClass::operator=(SchoolClass&& other) {
  number_ = other.number();
  other.clear_number();
  alias_ = std::move(other.mutable_alias());
  return *this;
}

// New()
::proto::Message* SchoolClass::New() {
  return reinterpret_cast<::proto::Message*>(new SchoolClass());
}

// Serialize()
::proto::SerializedMessage* SchoolClass::Serialize() const {
  return SchoolClass_reflection_->Serialize(this);
}

// DeSerialize()
void SchoolClass::DeSerialize(const char* buf, unsigned int size) {
  SchoolClass_reflection_->DeSerialize(this, buf, size);
}

// InitAsDefaultInstance()
void SchoolClass::InitAsDefaultInstance() {
}

// swapper
void SchoolClass::Swap(SchoolClass* other) {
  int number_tmp__ = other->number();
  other->set_number(number_);
  set_number(number_tmp__);

  std::string alias_tmp__ = std::move(other->mutable_alias());
  other->mutable_alias() = std::move(alias_);
  alias_ = std::move(alias_tmp__);
}

// default_instance()
const SchoolClass& SchoolClass::default_instance() {
  if (default_instance_ == NULL) {
    static_init_default_instances_samples_simple();
  }
  return *default_instance_;
}

SchoolClass* SchoolClass::default_instance_ = NULL;

// destructor
SchoolClass::~SchoolClass() {
}

// "number" = 1
bool SchoolClass::has_number() const {
  return (has_bits_[0] & 0x2) != 0;
}

int SchoolClass::number() const {
  return number_;
}

void SchoolClass::set_number(int number) {
  number_ = number;
  has_bits_[0] |= 0x2;
}

void SchoolClass::clear_number() {
  number_ = 0;
  has_bits_[0] &= (~0x2);
}

// "alias" = 2
bool SchoolClass::has_alias() const {
  return (has_bits_[0] & 0x4) != 0;
}

const std::string& SchoolClass::alias() const {
  return alias_;
}

void SchoolClass::set_alias(const std::string& alias) {
  alias_ = alias;
  has_bits_[0] |= 0x4;
}

void SchoolClass::set_alias(const char* alias) {
  alias_ = std::string(alias);
  has_bits_[0] |= 0x4;
}

void SchoolClass::set_alias(const char* alias, int size) {
  alias_ = std::string(alias, size);
  has_bits_[0] |= 0x4;
}

std::string SchoolClass::mutable_alias() {
  return alias_;
}

void SchoolClass::clear_alias() {
  alias_ = "";
  has_bits_[0] &= (~0x4);
}

}  // namespace HaiZhong

