#include <memory>

#include "Compiler/Message.h"
#include "Compiler/ProtoParser.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"

#include "simple_pb.h"

namespace {

std::shared_ptr<::proto::ProtoParser::Message> Pet_descriptor_;
std::shared_ptr<::proto::MessageReflection> Pet_reflection_;
std::shared_ptr<::proto::ProtoParser::Message> Student_descriptor_;
std::shared_ptr<::proto::MessageReflection> Student_reflection_;
std::shared_ptr<::proto::ProtoParser::Message> SchoolClass_descriptor_;
std::shared_ptr<::proto::MessageReflection> SchoolClass_reflection_;

}  // namepsace

void static_init_default_instances_samples_simple() {
  static bool already_called = false;
  if (already_called) return;
  already_called = true;

  if (HaiZhong::Pet::default_instance_ == NULL) {
    HaiZhong::Pet::default_instance_ = new HaiZhong::Pet();
    HaiZhong::Pet::default_instance_->InitAsDefaultInstance();
  }
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
  // static init for class Pet
  static const int Pet_offsets_[2] = {
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Pet, name_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Pet, type_),
  };
  i = 0;
  for (auto& field: parser.mutable_messages_list()[0]->mutable_fields_list()) {
    field->set_field_offset(Pet_offsets_[i++]);
  }
  Pet_descriptor_ = parser.mutable_messages_list()[0];
  Pet_reflection_.reset(
      new ::proto::MessageReflection(
          Pet_descriptor_,
          HaiZhong::Pet::default_instance_,
          PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Pet, has_bits_))
  );
  ::proto::MessageFactory::RegisterGeneratedMessage(Pet_reflection_);

  // static init for class Student
  static const int Student_offsets_[11] = {
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, name_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, age_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, xyz_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, def_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, ghi_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, graduated_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, weight_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, pet_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, scores_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, alias_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, sex_),
  };
  i = 0;
  for (auto& field: parser.mutable_messages_list()[1]->mutable_fields_list()) {
    field->set_field_offset(Student_offsets_[i++]);
  }
  Student_descriptor_ = parser.mutable_messages_list()[1];
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
  for (auto& field: parser.mutable_messages_list()[2]->mutable_fields_list()) {
    field->set_field_offset(SchoolClass_offsets_[i++]);
  }
  SchoolClass_descriptor_ = parser.mutable_messages_list()[2];
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

// -------------------- Pet --------------------- //
// constructor
Pet::Pet() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
Pet::Pet(const Pet& other) {
  name_ = other.name();
  type_ = other.type();
}

// move constructor
Pet::Pet(Pet&& other) {
  name_ = std::move(other.mutable_name());
  type_ = other.type();
  other.clear_type();
}

// copy assignment
Pet& Pet::operator=(const Pet& other) {
  name_ = other.name();
  type_ = other.type();
  return *this;
}

// move assignment
Pet& Pet::operator=(Pet&& other) {
  name_ = std::move(other.mutable_name());
  type_ = other.type();
  other.clear_type();
  return *this;
}

// New()
::proto::Message* Pet::New() {
  return reinterpret_cast<::proto::Message*>(new Pet());
}

// Serialize()
::proto::SerializedMessage* Pet::Serialize() const {
  return Pet_reflection_->Serialize(this);
}

// DeSerialize()
void Pet::DeSerialize(const char* buf, unsigned int size) {
  Pet_reflection_->DeSerialize(this, buf, size);
}

// InitAsDefaultInstance()
void Pet::InitAsDefaultInstance() {
}

// swapper
void Pet::Swap(Pet* other) {
  std::string name_tmp__ = std::move(other->mutable_name());
  other->mutable_name() = std::move(name_);
  name_ = std::move(name_tmp__);

  Pet::PetType type_tmp__ = other->type();
  other->set_type(type_);
  set_type(type_tmp__);
}

// default_instance()
const Pet& Pet::default_instance() {
  if (default_instance_ == NULL) {
    static_init_default_instances_samples_simple();
  }
  return *default_instance_;
}

Pet* Pet::default_instance_ = NULL;

// destructor
Pet::~Pet() {
}

// "name" = 1
bool Pet::has_name() const {
  return (has_bits_[0] & 0x2) != 0;
}

const std::string& Pet::name() const {
  return name_;
}

void Pet::set_name(const std::string& name) {
  name_ = name;
  has_bits_[0] |= 0x2;
}

void Pet::set_name(const char* name) {
  name_ = std::string(name);
  has_bits_[0] |= 0x2;
}

void Pet::set_name(const char* name, int size) {
  name_ = std::string(name, size);
  has_bits_[0] |= 0x2;
}

std::string Pet::mutable_name() {
  return name_;
}

void Pet::clear_name() {
  name_ = "";
  has_bits_[0] &= (~0x2);
}

// "type" = 2
bool Pet::has_type() const {
  return (has_bits_[0] & 0x4) != 0;
}

Pet::PetType Pet::type() const {
  return type_;
}

void Pet::set_type(Pet::PetType type) {
  type_ = type;
  has_bits_[0] |= 0x4;
}

void Pet::clear_type() {
  type_ = Pet::DOG;
  has_bits_[0] &= (~0x4);
}

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
  xyz_ = other.xyz();
  def_ = other.def();
  ghi_ = other.ghi();
  graduated_ = other.graduated();
  weight_ = other.weight();
  if (!pet_) {
    pet_ = new Pet();
  }
  *pet_ = other.pet();
  scores_ = other.scores();
  for (const std::string* p: other.alias().GetElements()) {
    alias_.AddAllocated(new std::string(*p));
  }
  sex_ = other.sex();
}

// move constructor
Student::Student(Student&& other) {
  name_ = std::move(other.mutable_name());
  age_ = other.age();
  other.clear_age();
  xyz_ = other.xyz();
  other.clear_xyz();
  def_ = other.def();
  other.clear_def();
  ghi_ = other.ghi();
  other.clear_ghi();
  graduated_ = other.graduated();
  other.clear_graduated();
  weight_ = other.weight();
  other.clear_weight();
  if (pet_ ) {
    delete pet_;
  }
  pet_ = other.release_pet();
  scores_ = std::move(other.mutable_scores());
  alias_ = std::move(other.mutable_alias());
  sex_ = other.sex();
  other.clear_sex();
}

// copy assignment
Student& Student::operator=(const Student& other) {
  name_ = other.name();
  age_ = other.age();
  xyz_ = other.xyz();
  def_ = other.def();
  ghi_ = other.ghi();
  graduated_ = other.graduated();
  weight_ = other.weight();
  if (!pet_) {
    pet_ = new Pet();
  }
  *pet_ = other.pet();
  scores_ = other.scores();
  for (const std::string* p: other.alias().GetElements()) {
    alias_.AddAllocated(new std::string(*p));
  }
  sex_ = other.sex();
  return *this;
}

// move assignment
Student& Student::operator=(Student&& other) {
  name_ = std::move(other.mutable_name());
  age_ = other.age();
  other.clear_age();
  xyz_ = other.xyz();
  other.clear_xyz();
  def_ = other.def();
  other.clear_def();
  ghi_ = other.ghi();
  other.clear_ghi();
  graduated_ = other.graduated();
  other.clear_graduated();
  weight_ = other.weight();
  other.clear_weight();
  if (pet_ ) {
    delete pet_;
  }
  pet_ = other.release_pet();
  scores_ = std::move(other.mutable_scores());
  alias_ = std::move(other.mutable_alias());
  sex_ = other.sex();
  other.clear_sex();
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
  pet_ = const_cast<Pet*>(&Pet::default_instance());
}

// swapper
void Student::Swap(Student* other) {
  std::string name_tmp__ = std::move(other->mutable_name());
  other->mutable_name() = std::move(name_);
  name_ = std::move(name_tmp__);

  unsigned int age_tmp__ = other->age();
  other->set_age(age_);
  set_age(age_tmp__);

  int xyz_tmp__ = other->xyz();
  other->set_xyz(xyz_);
  set_xyz(xyz_tmp__);

  unsigned long long def_tmp__ = other->def();
  other->set_def(def_);
  set_def(def_tmp__);

  long long ghi_tmp__ = other->ghi();
  other->set_ghi(ghi_);
  set_ghi(ghi_tmp__);

  bool graduated_tmp__ = other->graduated();
  other->set_graduated(graduated_);
  set_graduated(graduated_tmp__);

  double weight_tmp__ = other->weight();
  other->set_weight(weight_);
  set_weight(weight_tmp__);

  Pet* pet_tmp__ = other->release_pet();
  other->set_allocated_pet(this->release_pet());
  set_allocated_pet(pet_tmp__);

  ::proto::RepeatedField<int> scores_tmp__ = std::move(other->mutable_scores());
  other->mutable_scores() = std::move(scores_);
  scores_ = std::move(scores_tmp__);

  ::proto::RepeatedPtrField<std::string> alias_tmp__ = std::move(other->mutable_alias());
  other->mutable_alias() = std::move(alias_);
  alias_ = std::move(alias_tmp__);

  Student::Sex sex_tmp__ = other->sex();
  other->set_sex(sex_);
  set_sex(sex_tmp__);
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
  delete pet_;
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

// "xyz" = 3
bool Student::has_xyz() const {
  return (has_bits_[0] & 0x8) != 0;
}

int Student::xyz() const {
  return xyz_;
}

void Student::set_xyz(int xyz) {
  xyz_ = xyz;
  has_bits_[0] |= 0x8;
}

void Student::clear_xyz() {
  xyz_ = 0;
  has_bits_[0] &= (~0x8);
}

// "def" = 4
bool Student::has_def() const {
  return (has_bits_[0] & 0x10) != 0;
}

unsigned long long Student::def() const {
  return def_;
}

void Student::set_def(unsigned long long def) {
  def_ = def;
  has_bits_[0] |= 0x10;
}

void Student::clear_def() {
  def_ = 0;
  has_bits_[0] &= (~0x10);
}

// "ghi" = 5
bool Student::has_ghi() const {
  return (has_bits_[0] & 0x20) != 0;
}

long long Student::ghi() const {
  return ghi_;
}

void Student::set_ghi(long long ghi) {
  ghi_ = ghi;
  has_bits_[0] |= 0x20;
}

void Student::clear_ghi() {
  ghi_ = 0;
  has_bits_[0] &= (~0x20);
}

// "graduated" = 6
bool Student::has_graduated() const {
  return (has_bits_[0] & 0x40) != 0;
}

bool Student::graduated() const {
  return graduated_;
}

void Student::set_graduated(bool graduated) {
  graduated_ = graduated;
  has_bits_[0] |= 0x40;
}

void Student::clear_graduated() {
  graduated_ = false;
  has_bits_[0] &= (~0x40);
}

// "weight" = 7
bool Student::has_weight() const {
  return (has_bits_[0] & 0x80) != 0;
}

double Student::weight() const {
  return weight_;
}

void Student::set_weight(double weight) {
  weight_ = weight;
  has_bits_[0] |= 0x80;
}

void Student::clear_weight() {
  weight_ = 0;
  has_bits_[0] &= (~0x80);
}

// "pet" = 9
bool Student::has_pet() const {
  return (has_bits_[1] & 0x2) != 0;
}

const Pet& Student::pet() const {
  if (has_pet() && pet_) {
    return *pet_;
  }
  else {
    return Pet::default_instance();
  }
}

Pet* Student::mutable_pet() {
  if (has_pet() && pet_) {
    return pet_;
  }
  else {
    pet_ = new Pet();
    has_bits_[1] |= 0x2;
    return pet_;
  }
}

void Student::set_allocated_pet(Pet* pet) {
  if (pet_) {
    delete pet_;
  }
  pet_ = pet;
  if (pet_) {
    has_bits_[1] |= 0x2;
  }
  else {
    has_bits_[1] &= (~0x2);
  }
}

Pet* Student::release_pet() {
  Pet* pet_tmp__ = pet_;
  pet_ = nullptr;
  has_bits_[1] &= (~0x2);
  return pet_tmp__;
}

void Student::clear_pet() {
  if (pet_) {
    delete pet_;
  }
  pet_ = nullptr;
  has_bits_[1] &= (~0x2);
}

// "scores" = 11
int Student::scores_size() const {
  return scores_.size();
}

int Student::scores(int index) {
  return scores_.Get(index);
}

void Student::set_scores(int index, int value) {
  if ((int)scores_.size() > index) {
    scores_.Set(index, value);
  }
}

void Student::add_scores(int value) {
   scores_.Add(value);
}

void Student::clear_scores() {
  scores_ .Clear();
}

const ::proto::RepeatedField<int>& Student::scores() const {
  return scores_;
}

::proto::RepeatedField<int>& Student::mutable_scores() {
  return scores_;
}

// "alias" = 15
int Student::alias_size() const {
  return alias_.size();
}

const std::string& Student::alias(int index) {
  return alias_.Get(index);
}

void Student::set_alias(int index, const std::string& value) {
  if (index < (int)alias_.size()) {
    alias_.Set(index, value);
  }
}

void Student::set_alias(int index, const char* value) {
  if (index < (int)alias_.size()) {
    alias_.Set(index, std::string(value));
  }
}

void Student::set_alias(int index, const char* value, int size) {
  if (index < (int)alias_.size()) {
    alias_.Set(index, std::string(value, size));
  }
}

std::string* Student::add_alias() {
  return alias_.Add();
}

void Student::add_alias(const std::string& value) {
  alias_.AddAllocated(new std::string(value));
}

void Student::add_alias(const char* value) {
  alias_.AddAllocated(new std::string(value));
}

void Student::add_alias(const char* value, int size) {
  alias_.AddAllocated(new std::string(value, size));
}

std::string* Student::mutable_alias(int index) {
  return alias_.GetMutable(index);
}

void Student::clear_alias() {
  alias_.Clear();
}

const ::proto::RepeatedPtrField<std::string>& Student::alias() const {
  return alias_;
}

::proto::RepeatedPtrField<std::string>& Student::mutable_alias() {
  return alias_;
}

// "sex" = 25
bool Student::has_sex() const {
  return (has_bits_[3] & 0x2) != 0;
}

Student::Sex Student::sex() const {
  return sex_;
}

void Student::set_sex(Student::Sex sex) {
  sex_ = sex;
  has_bits_[3] |= 0x2;
}

void Student::clear_sex() {
  sex_ = Student::MALE;
  has_bits_[3] &= (~0x2);
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

