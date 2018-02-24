#include <memory>
#include <mutex>
#include <map>

#include "Proto/Descriptor.h"
#include "Proto/DescriptorsBuilder.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"

#include "simple_pb.h"

namespace {

const ::proto::MessageDescriptor* Pet_descriptor_ = nullptr;
const ::proto::MessageReflection* Pet_reflection_ = nullptr;
const ::proto::MessageDescriptor* Student_descriptor_ = nullptr;
const ::proto::MessageReflection* Student_reflection_ = nullptr;
const ::proto::MessageDescriptor* SchoolClass_descriptor_ = nullptr;
const ::proto::MessageReflection* SchoolClass_reflection_ = nullptr;
const ::proto::MessageDescriptor* StudentRequest_descriptor_ = nullptr;
const ::proto::MessageReflection* StudentRequest_reflection_ = nullptr;
const ::proto::MessageDescriptor* StudentResponse_descriptor_ = nullptr;
const ::proto::MessageReflection* StudentResponse_reflection_ = nullptr;

std::string GetProtoContent();

}  // namepsace

void static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_simple() {
  if (HaiZhong::Pet::default_instance_ == nullptr) {
    HaiZhong::Pet::default_instance_ = new HaiZhong::Pet();
    HaiZhong::Pet::default_instance_->InitAsDefaultInstance();
  }
  if (HaiZhong::Student::default_instance_ == nullptr) {
    HaiZhong::Student::default_instance_ = new HaiZhong::Student();
    HaiZhong::Student::default_instance_->InitAsDefaultInstance();
  }
  if (HaiZhong::SchoolClass::default_instance_ == nullptr) {
    HaiZhong::SchoolClass::default_instance_ = new HaiZhong::SchoolClass();
    HaiZhong::SchoolClass::default_instance_->InitAsDefaultInstance();
  }
  if (HaiZhong::StudentRequest::default_instance_ == nullptr) {
    HaiZhong::StudentRequest::default_instance_ = new HaiZhong::StudentRequest();
    HaiZhong::StudentRequest::default_instance_->InitAsDefaultInstance();
  }
  if (HaiZhong::StudentResponse::default_instance_ == nullptr) {
    HaiZhong::StudentResponse::default_instance_ = new HaiZhong::StudentResponse();
    HaiZhong::StudentResponse::default_instance_->InitAsDefaultInstance();
  }
}

void static_init_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_simple() {
  static bool already_called = false;
  if (already_called) return;
  already_called = true;

  ::proto::DescriptorsBuilder descriptors_builder(GetProtoContent());
  auto file_dscpt = descriptors_builder.BuildDescriptors();
  CHECK(file_dscpt != nullptr, "Build class descriptor failed.");
  ::proto::MessageFactory::RegisterParsedProtoFile(file_dscpt);

  static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_simple();

  // static init for class Pet
  static const int Pet_offsets_[3] = {
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Pet, name_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Pet, type_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Pet, friend_to_),
  };
  Pet_descriptor_ = file_dscpt->FindMessageTypeByName("HaiZhong.Pet");
  CHECK(Pet_descriptor_ != nullptr, 
        "Can't find message descriptor for HaiZhong.Pet");
  Pet_reflection_ = 
      new ::proto::MessageReflection(
          Pet_descriptor_,
          HaiZhong::Pet::default_instance_,
          Pet_offsets_,
          PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Pet, has_bits_));
  ::proto::MessageFactory::RegisterGeneratedMessage(Pet_reflection_);

  // static init for class Student
  static const int Student_offsets_[14] = {
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, name_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, age_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, xyz_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, def_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, ghi_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, graduated_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, weight_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, pets_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, scores_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, first_pet_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, alias_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, partner_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, symbol_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, sex_),
  };
  Student_descriptor_ = file_dscpt->FindMessageTypeByName("HaiZhong.Student");
  CHECK(Student_descriptor_ != nullptr, 
        "Can't find message descriptor for HaiZhong.Student");
  Student_reflection_ = 
      new ::proto::MessageReflection(
          Student_descriptor_,
          HaiZhong::Student::default_instance_,
          Student_offsets_,
          PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::Student, has_bits_));
  ::proto::MessageFactory::RegisterGeneratedMessage(Student_reflection_);

  // static init for class SchoolClass
  static const int SchoolClass_offsets_[4] = {
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::SchoolClass, number_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::SchoolClass, alias_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::SchoolClass, captain_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::SchoolClass, students_),
  };
  SchoolClass_descriptor_ = file_dscpt->FindMessageTypeByName("HaiZhong.SchoolClass");
  CHECK(SchoolClass_descriptor_ != nullptr, 
        "Can't find message descriptor for HaiZhong.SchoolClass");
  SchoolClass_reflection_ = 
      new ::proto::MessageReflection(
          SchoolClass_descriptor_,
          HaiZhong::SchoolClass::default_instance_,
          SchoolClass_offsets_,
          PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::SchoolClass, has_bits_));
  ::proto::MessageFactory::RegisterGeneratedMessage(SchoolClass_reflection_);

  // static init for class StudentRequest
  static const int StudentRequest_offsets_[4] = {
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::StudentRequest, op_type_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::StudentRequest, class_number_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::StudentRequest, student_name_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::StudentRequest, student_),
  };
  StudentRequest_descriptor_ = file_dscpt->FindMessageTypeByName("HaiZhong.StudentRequest");
  CHECK(StudentRequest_descriptor_ != nullptr, 
        "Can't find message descriptor for HaiZhong.StudentRequest");
  StudentRequest_reflection_ = 
      new ::proto::MessageReflection(
          StudentRequest_descriptor_,
          HaiZhong::StudentRequest::default_instance_,
          StudentRequest_offsets_,
          PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::StudentRequest, has_bits_));
  ::proto::MessageFactory::RegisterGeneratedMessage(StudentRequest_reflection_);

  // static init for class StudentResponse
  static const int StudentResponse_offsets_[2] = {
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::StudentResponse, return_code_),
    PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::StudentResponse, error_message_),
  };
  StudentResponse_descriptor_ = file_dscpt->FindMessageTypeByName("HaiZhong.StudentResponse");
  CHECK(StudentResponse_descriptor_ != nullptr, 
        "Can't find message descriptor for HaiZhong.StudentResponse");
  StudentResponse_reflection_ = 
      new ::proto::MessageReflection(
          StudentResponse_descriptor_,
          HaiZhong::StudentResponse::default_instance_,
          StudentResponse_offsets_,
          PROTO_MESSAGE_FIELD_OFFSET(HaiZhong::StudentResponse, has_bits_));
  ::proto::MessageFactory::RegisterGeneratedMessage(StudentResponse_reflection_);

}

// Force static_init_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_simple() to be called at initialization time.
struct static_init_forcer_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_simple {
  static_init_forcer_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_simple() {
    static_init_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_simple();
  }
} static_init_forcer_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_simple_obj_;


namespace HaiZhong {

// ******************** Pet ******************** //
// constructor
Pet::Pet() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
Pet::Pet(const Pet& other) {
  CopyFrom(other);
}

// move constructor
Pet::Pet(Pet&& other) {
  MoveFrom(std::move(other));
}

// copy assignment
Pet& Pet::operator=(const Pet& other) {
  CopyFrom(other);
  return *this;
}
// move assignment
Pet& Pet::operator=(Pet&& other) {
  MoveFrom(std::move(other));
  return *this;
}

// New()
::proto::Message* Pet::New() const {
  return reinterpret_cast<::proto::Message*>(new Pet());
}

// CopyFrom()
void Pet::CopyFrom(const Pet& other) {
  name_ = other.name();
  type_ = other.type();
  friend_to_ = other.friend_to();
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
}

// MoveFrom()
void Pet::MoveFrom(Pet&& other) {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
  name_ = std::move(other.mutable_name());
  type_ = other.type();
  friend_to_ = std::move(other.mutable_friend_to());
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    other.has_bits_[i] = 0;
  }
}

// Equals()
bool Pet::Equals(const Pet& other) const {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    if (has_bits_[i] != other.has_bits_[i]) {
      return false;
    }
  }
  if (name_ != other.name_) {
    return false;
  }
  if (type_ != other.type_) {
    return false;
  }
  for (unsigned int i = 0; i < friend_to_.size(); i++) {
    if (friend_to_.at(i) != other.friend_to_.at(i)) {
      return false;
    }
  }
  return true;
}

// Serialize()
::proto::SerializedMessage* Pet::Serialize() const {
  return Pet_reflection_->Serialize(this);
}

// DeSerialize()
void Pet::DeSerialize(const char* buf, unsigned int size) {
  Pet_reflection_->DeSerialize(this, buf, size);
}

// Print()
void Pet::Print(int indent_num) const {
  PrintIndent(indent_num);
  std::cout << "Pet " << "{" << std::endl;
  if (has_name()) {
    PrintIndent(indent_num + 1);
    std::cout << "name: " << "\"" << name_ << "\"" << std::endl;
  }
  if (has_type()) {
    PrintIndent(indent_num + 1);
    std::string enum_value =
        (reinterpret_cast<const proto::EnumDescriptor*>(
            Pet_descriptor_->FindFieldByName("type")->type_descriptor()))
                 ->EnumValueAsString(type_);
    std::cout << "type: " << enum_value << std::endl;
  }
  if (friend_to_size() > 0) {
    PrintIndent(indent_num + 1);
    std::cout << "friend_to: " << "[";
    const auto* enum_type_descriptor =
        (reinterpret_cast<const proto::EnumDescriptor*>(
            Pet_descriptor_->FindFieldByName("friend_to")->type_descriptor()));
    for (const auto& ele: friend_to_) {
        std::cout << enum_type_descriptor->EnumValueAsString(ele) << ", ";
    }
    std::cout << "]" << std::endl;
  }
  PrintIndent(indent_num);
  std::cout << "}" << std::endl;
}

// InitAsDefaultInstance()
void Pet::InitAsDefaultInstance() {
}

// swapper
void Pet::Swap(Pet* other) {
  // store has_bits
  char* buf = new char[2 * sizeof(has_bits_)];
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    buf[i] = has_bits_[i];
    buf[i + sizeof(has_bits_)] = other->has_bits_[i];
  }

  std::string name_tmp__ = std::move(other->mutable_name());
  other->mutable_name() = std::move(name_);
  name_ = std::move(name_tmp__);

  Pet::PetType type_tmp__ = other->type();
  other->set_type(type_);
  set_type(type_tmp__);

  ::proto::RepeatedField<Pet::PetType> friend_to_tmp__ = std::move(other->mutable_friend_to());
  other->mutable_friend_to() = std::move(friend_to_);
  friend_to_ = std::move(friend_to_tmp__);

  // swap has_bits
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = buf[i + sizeof(has_bits_)];
    other->has_bits_[i] = buf[i];
  }
  delete buf;
}

// default_instance()
const Pet& Pet::default_instance() {
  if (default_instance_ == nullptr) {
    static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_simple();
  }
  return *default_instance_;
}

Pet* Pet::default_instance_ = nullptr;

const ::proto::MessageDescriptor* Pet::GetDescriptor() const {
  return Pet_descriptor_;
}

const ::proto::MessageReflection* Pet::GetReflection() const {
  return Pet_reflection_;
}

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

// "friend_to" = 3
int Pet::friend_to_size() const {
  return friend_to_.size();
}

Pet::PetType Pet::friend_to(int index) const {
  return friend_to_.Get(index);
}

void Pet::set_friend_to(int index, Pet::PetType value) {
  if ((int)friend_to_.size() > index) {
    friend_to_.Set(index, value);
  }
}

void Pet::add_friend_to(Pet::PetType value) {
   friend_to_.Add(value);
}

void Pet::clear_friend_to() {
  friend_to_ .Clear();
}

const ::proto::RepeatedField<Pet::PetType>& Pet::friend_to() const {
  return friend_to_;
}

::proto::RepeatedField<Pet::PetType>& Pet::mutable_friend_to() {
  return friend_to_;
}

// ******************** Student ******************** //
// constructor
Student::Student() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
Student::Student(const Student& other) {
  CopyFrom(other);
}

// move constructor
Student::Student(Student&& other) {
  MoveFrom(std::move(other));
}

// copy assignment
Student& Student::operator=(const Student& other) {
  CopyFrom(other);
  return *this;
}
// move assignment
Student& Student::operator=(Student&& other) {
  MoveFrom(std::move(other));
  return *this;
}

// New()
::proto::Message* Student::New() const {
  return reinterpret_cast<::proto::Message*>(new Student());
}

// CopyFrom()
void Student::CopyFrom(const Student& other) {
  name_ = other.name();
  age_ = other.age();
  xyz_ = other.xyz();
  def_ = other.def();
  ghi_ = other.ghi();
  graduated_ = other.graduated();
  weight_ = other.weight();
  for (const Pet* p: other.pets().GetElements()) {
    pets_.AddAllocated(new Pet(*p));
  }
  scores_ = other.scores();
  if (other.first_pet_) {
    if (!first_pet_) {
      first_pet_ = new Pet();
    }
    *first_pet_ = other.first_pet();
  }
  for (const std::string* p: other.alias().GetElements()) {
    alias_.AddAllocated(new std::string(*p));
  }
  if (other.partner_) {
    if (!partner_) {
      partner_ = new Student();
    }
    *partner_ = other.partner();
  }
  symbol_ = other.symbol();
  sex_ = other.sex();
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
}

// MoveFrom()
void Student::MoveFrom(Student&& other) {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
  name_ = std::move(other.mutable_name());
  age_ = other.age();
  xyz_ = other.xyz();
  def_ = other.def();
  ghi_ = other.ghi();
  graduated_ = other.graduated();
  weight_ = other.weight();
  pets_ = std::move(other.mutable_pets());
  scores_ = std::move(other.mutable_scores());
  if (other.first_pet_) {
    if (first_pet_) {
      delete first_pet_;
    }
    first_pet_ = other.first_pet_;
    other.first_pet_ = nullptr;
  }
  alias_ = std::move(other.mutable_alias());
  if (other.partner_) {
    if (partner_) {
      delete partner_;
    }
    partner_ = other.partner_;
    other.partner_ = nullptr;
  }
  symbol_ = other.symbol();
  sex_ = other.sex();
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    other.has_bits_[i] = 0;
  }
}

// Equals()
bool Student::Equals(const Student& other) const {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    if (has_bits_[i] != other.has_bits_[i]) {
      return false;
    }
  }
  if (name_ != other.name_) {
    return false;
  }
  if (age_ != other.age_) {
    return false;
  }
  if (xyz_ != other.xyz_) {
    return false;
  }
  if (def_ != other.def_) {
    return false;
  }
  if (ghi_ != other.ghi_) {
    return false;
  }
  if (graduated_ != other.graduated_) {
    return false;
  }
  if (weight_ != other.weight_) {
    return false;
  }
  for (unsigned int i = 0; i < pets_.size(); i++) {
    if (!pets_.at(i).Equals(other.pets_.at(i))) {
      return false;
    }
  }
  for (unsigned int i = 0; i < scores_.size(); i++) {
    if (scores_.at(i) != other.scores_.at(i)) {
      return false;
    }
  }
  if (first_pet_ && other.first_pet_ &&
      !first_pet_->Equals(*other.first_pet_)) {
    return false;
  }
  for (unsigned int i = 0; i < alias_.size(); i++) {
    if (alias_.at(i) != other.alias_.at(i)) {
      return false;
    }
  }
  if (partner_ && other.partner_ &&
      !partner_->Equals(*other.partner_)) {
    return false;
  }
  if (symbol_ != other.symbol_) {
    return false;
  }
  if (sex_ != other.sex_) {
    return false;
  }
  return true;
}

// Serialize()
::proto::SerializedMessage* Student::Serialize() const {
  return Student_reflection_->Serialize(this);
}

// DeSerialize()
void Student::DeSerialize(const char* buf, unsigned int size) {
  Student_reflection_->DeSerialize(this, buf, size);
}

// Print()
void Student::Print(int indent_num) const {
  PrintIndent(indent_num);
  std::cout << "Student " << "{" << std::endl;
  if (has_name()) {
    PrintIndent(indent_num + 1);
    std::cout << "name: " << "\"" << name_ << "\"" << std::endl;
  }
  if (has_age()) {
    PrintIndent(indent_num + 1);
    std::cout << "age: " << age_ << std::endl;
  }
  if (has_xyz()) {
    PrintIndent(indent_num + 1);
    std::cout << "xyz: " << xyz_ << std::endl;
  }
  if (has_def()) {
    PrintIndent(indent_num + 1);
    std::cout << "def: " << def_ << std::endl;
  }
  if (has_ghi()) {
    PrintIndent(indent_num + 1);
    std::cout << "ghi: " << ghi_ << std::endl;
  }
  if (has_graduated()) {
    PrintIndent(indent_num + 1);
    std::cout << "graduated: " << graduated_ << std::endl;
  }
  if (has_weight()) {
    PrintIndent(indent_num + 1);
    std::cout << "weight: " << weight_ << std::endl;
  }
  if (pets_size() > 0) {
    PrintIndent(indent_num + 1);
    std::cout << "pets: " << "[***]" << std::endl;
    for (const auto& ele: pets_) {
        ele.Print(indent_num + 1);
    }
  }
  if (scores_size() > 0) {
    PrintIndent(indent_num + 1);
    std::cout << "scores: " << "[";
    for (const auto& ele: scores_) {
        std::cout << ele << ", ";
    }
    std::cout << "]" << std::endl;
  }
  if (has_first_pet()) {
    PrintIndent(indent_num + 1);
    std::cout << "first_pet: " << "*" << std::endl;
    first_pet_->Print(indent_num + 1);
  }
  if (alias_size() > 0) {
    PrintIndent(indent_num + 1);
    std::cout << "alias: " << "[";
    for (const auto& ele: alias_) {
        std::cout << "\"" << ele << "\"" << ", ";
    }
    std::cout << "]" << std::endl;
  }
  if (has_partner()) {
    PrintIndent(indent_num + 1);
    std::cout << "partner: " << "*" << std::endl;
    partner_->Print(indent_num + 1);
  }
  if (has_symbol()) {
    PrintIndent(indent_num + 1);
    std::cout << "symbol: " << symbol_ << std::endl;
  }
  if (has_sex()) {
    PrintIndent(indent_num + 1);
    std::string enum_value =
        (reinterpret_cast<const proto::EnumDescriptor*>(
            Student_descriptor_->FindFieldByName("sex")->type_descriptor()))
                 ->EnumValueAsString(sex_);
    std::cout << "sex: " << enum_value << std::endl;
  }
  PrintIndent(indent_num);
  std::cout << "}" << std::endl;
}

// InitAsDefaultInstance()
void Student::InitAsDefaultInstance() {
  first_pet_ = const_cast<Pet*>(&Pet::default_instance());
  partner_ = const_cast<Student*>(&Student::default_instance());
}

// swapper
void Student::Swap(Student* other) {
  // store has_bits
  char* buf = new char[2 * sizeof(has_bits_)];
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    buf[i] = has_bits_[i];
    buf[i + sizeof(has_bits_)] = other->has_bits_[i];
  }

  std::string name_tmp__ = std::move(other->mutable_name());
  other->mutable_name() = std::move(name_);
  name_ = std::move(name_tmp__);

  uint32 age_tmp__ = other->age();
  other->set_age(age_);
  set_age(age_tmp__);

  int32 xyz_tmp__ = other->xyz();
  other->set_xyz(xyz_);
  set_xyz(xyz_tmp__);

  uint64 def_tmp__ = other->def();
  other->set_def(def_);
  set_def(def_tmp__);

  int64 ghi_tmp__ = other->ghi();
  other->set_ghi(ghi_);
  set_ghi(ghi_tmp__);

  bool graduated_tmp__ = other->graduated();
  other->set_graduated(graduated_);
  set_graduated(graduated_tmp__);

  double weight_tmp__ = other->weight();
  other->set_weight(weight_);
  set_weight(weight_tmp__);

  ::proto::RepeatedPtrField<Pet> pets_tmp__ = std::move(other->mutable_pets());
  other->mutable_pets() = std::move(pets_);
  pets_ = std::move(pets_tmp__);

  ::proto::RepeatedField<int32> scores_tmp__ = std::move(other->mutable_scores());
  other->mutable_scores() = std::move(scores_);
  scores_ = std::move(scores_tmp__);

  Pet* first_pet_tmp__ = other->release_first_pet();
  other->set_allocated_first_pet(this->release_first_pet());
  set_allocated_first_pet(first_pet_tmp__);

  ::proto::RepeatedPtrField<std::string> alias_tmp__ = std::move(other->mutable_alias());
  other->mutable_alias() = std::move(alias_);
  alias_ = std::move(alias_tmp__);

  Student* partner_tmp__ = other->release_partner();
  other->set_allocated_partner(this->release_partner());
  set_allocated_partner(partner_tmp__);

  char symbol_tmp__ = other->symbol();
  other->set_symbol(symbol_);
  set_symbol(symbol_tmp__);

  Student::Sex sex_tmp__ = other->sex();
  other->set_sex(sex_);
  set_sex(sex_tmp__);

  // swap has_bits
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = buf[i + sizeof(has_bits_)];
    other->has_bits_[i] = buf[i];
  }
  delete buf;
}

// default_instance()
const Student& Student::default_instance() {
  if (default_instance_ == nullptr) {
    static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_simple();
  }
  return *default_instance_;
}

Student* Student::default_instance_ = nullptr;

const ::proto::MessageDescriptor* Student::GetDescriptor() const {
  return Student_descriptor_;
}

const ::proto::MessageReflection* Student::GetReflection() const {
  return Student_reflection_;
}

// destructor
Student::~Student() {
  if (first_pet_) {
    delete first_pet_;
  }
  if (partner_) {
    delete partner_;
  }
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

uint32 Student::age() const {
  return age_;
}

void Student::set_age(uint32 age) {
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

int32 Student::xyz() const {
  return xyz_;
}

void Student::set_xyz(int32 xyz) {
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

uint64 Student::def() const {
  return def_;
}

void Student::set_def(uint64 def) {
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

int64 Student::ghi() const {
  return ghi_;
}

void Student::set_ghi(int64 ghi) {
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

// "pets" = 9
int Student::pets_size() const {
  return pets_.size();
}

const Pet& Student::pets(int index) const {
  return pets_.Get(index);
}

Pet* Student::add_pets() {
  return pets_.Add();
}

Pet* Student::mutable_pets(int index) {
  return pets_.GetMutable(index);
}

void Student::clear_pets() {
  pets_.Clear();
}

const ::proto::RepeatedPtrField<Pet>& Student::pets() const {
  return pets_;
}

::proto::RepeatedPtrField<Pet>& Student::mutable_pets() {
  return pets_;
}

// "scores" = 11
int Student::scores_size() const {
  return scores_.size();
}

int32 Student::scores(int index) const {
  return scores_.Get(index);
}

void Student::set_scores(int index, int32 value) {
  if ((int)scores_.size() > index) {
    scores_.Set(index, value);
  }
}

void Student::add_scores(int32 value) {
   scores_.Add(value);
}

void Student::clear_scores() {
  scores_ .Clear();
}

const ::proto::RepeatedField<int32>& Student::scores() const {
  return scores_;
}

::proto::RepeatedField<int32>& Student::mutable_scores() {
  return scores_;
}

// "first_pet" = 12
bool Student::has_first_pet() const {
  return (has_bits_[1] & 0x10) != 0;
}

const Pet& Student::first_pet() const {
  if (has_first_pet() && first_pet_) {
    return *first_pet_;
  }
  else {
    return Pet::default_instance();
  }
}

Pet* Student::mutable_first_pet() {
  if (has_first_pet() && first_pet_) {
    return first_pet_;
  }
  else {
    first_pet_ = new Pet();
    has_bits_[1] |= 0x10;
    return first_pet_;
  }
}

void Student::set_allocated_first_pet(Pet* first_pet) {
  if (first_pet_) {
    delete first_pet_;
  }
  first_pet_ = first_pet;
  if (first_pet_) {
    has_bits_[1] |= 0x10;
  }
  else {
    has_bits_[1] &= (~0x10);
  }
}

Pet* Student::release_first_pet() {
  Pet* first_pet_tmp__ = first_pet_;
  first_pet_ = nullptr;
  has_bits_[1] &= (~0x10);
  return first_pet_tmp__;
}

void Student::clear_first_pet() {
  if (first_pet_) {
    delete first_pet_;
  }
  first_pet_ = nullptr;
  has_bits_[1] &= (~0x10);
}

// "alias" = 15
int Student::alias_size() const {
  return alias_.size();
}

const std::string& Student::alias(int index) const {
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

// "partner" = 18
bool Student::has_partner() const {
  return (has_bits_[2] & 0x4) != 0;
}

const Student& Student::partner() const {
  if (has_partner() && partner_) {
    return *partner_;
  }
  else {
    return Student::default_instance();
  }
}

Student* Student::mutable_partner() {
  if (has_partner() && partner_) {
    return partner_;
  }
  else {
    partner_ = new Student();
    has_bits_[2] |= 0x4;
    return partner_;
  }
}

void Student::set_allocated_partner(Student* partner) {
  if (partner_) {
    delete partner_;
  }
  partner_ = partner;
  if (partner_) {
    has_bits_[2] |= 0x4;
  }
  else {
    has_bits_[2] &= (~0x4);
  }
}

Student* Student::release_partner() {
  Student* partner_tmp__ = partner_;
  partner_ = nullptr;
  has_bits_[2] &= (~0x4);
  return partner_tmp__;
}

void Student::clear_partner() {
  if (partner_) {
    delete partner_;
  }
  partner_ = nullptr;
  has_bits_[2] &= (~0x4);
}

// "symbol" = 19
bool Student::has_symbol() const {
  return (has_bits_[2] & 0x8) != 0;
}

char Student::symbol() const {
  return symbol_;
}

void Student::set_symbol(char symbol) {
  symbol_ = symbol;
  has_bits_[2] |= 0x8;
}

void Student::clear_symbol() {
  symbol_ = 0;
  has_bits_[2] &= (~0x8);
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

// ******************** SchoolClass ******************** //
// constructor
SchoolClass::SchoolClass() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
SchoolClass::SchoolClass(const SchoolClass& other) {
  CopyFrom(other);
}

// move constructor
SchoolClass::SchoolClass(SchoolClass&& other) {
  MoveFrom(std::move(other));
}

// copy assignment
SchoolClass& SchoolClass::operator=(const SchoolClass& other) {
  CopyFrom(other);
  return *this;
}
// move assignment
SchoolClass& SchoolClass::operator=(SchoolClass&& other) {
  MoveFrom(std::move(other));
  return *this;
}

// New()
::proto::Message* SchoolClass::New() const {
  return reinterpret_cast<::proto::Message*>(new SchoolClass());
}

// CopyFrom()
void SchoolClass::CopyFrom(const SchoolClass& other) {
  number_ = other.number();
  alias_ = other.alias();
  if (other.captain_) {
    if (!captain_) {
      captain_ = new Student();
    }
    *captain_ = other.captain();
  }
  for (const Student* p: other.students().GetElements()) {
    students_.AddAllocated(new Student(*p));
  }
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
}

// MoveFrom()
void SchoolClass::MoveFrom(SchoolClass&& other) {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
  number_ = other.number();
  alias_ = std::move(other.mutable_alias());
  if (other.captain_) {
    if (captain_) {
      delete captain_;
    }
    captain_ = other.captain_;
    other.captain_ = nullptr;
  }
  students_ = std::move(other.mutable_students());
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    other.has_bits_[i] = 0;
  }
}

// Equals()
bool SchoolClass::Equals(const SchoolClass& other) const {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    if (has_bits_[i] != other.has_bits_[i]) {
      return false;
    }
  }
  if (number_ != other.number_) {
    return false;
  }
  if (alias_ != other.alias_) {
    return false;
  }
  if (captain_ && other.captain_ &&
      !captain_->Equals(*other.captain_)) {
    return false;
  }
  for (unsigned int i = 0; i < students_.size(); i++) {
    if (!students_.at(i).Equals(other.students_.at(i))) {
      return false;
    }
  }
  return true;
}

// Serialize()
::proto::SerializedMessage* SchoolClass::Serialize() const {
  return SchoolClass_reflection_->Serialize(this);
}

// DeSerialize()
void SchoolClass::DeSerialize(const char* buf, unsigned int size) {
  SchoolClass_reflection_->DeSerialize(this, buf, size);
}

// Print()
void SchoolClass::Print(int indent_num) const {
  PrintIndent(indent_num);
  std::cout << "SchoolClass " << "{" << std::endl;
  if (has_number()) {
    PrintIndent(indent_num + 1);
    std::cout << "number: " << number_ << std::endl;
  }
  if (has_alias()) {
    PrintIndent(indent_num + 1);
    std::cout << "alias: " << "\"" << alias_ << "\"" << std::endl;
  }
  if (has_captain()) {
    PrintIndent(indent_num + 1);
    std::cout << "captain: " << "*" << std::endl;
    captain_->Print(indent_num + 1);
  }
  if (students_size() > 0) {
    PrintIndent(indent_num + 1);
    std::cout << "students: " << "[***]" << std::endl;
    for (const auto& ele: students_) {
        ele.Print(indent_num + 1);
    }
  }
  PrintIndent(indent_num);
  std::cout << "}" << std::endl;
}

// InitAsDefaultInstance()
void SchoolClass::InitAsDefaultInstance() {
  captain_ = const_cast<Student*>(&Student::default_instance());
}

// swapper
void SchoolClass::Swap(SchoolClass* other) {
  // store has_bits
  char* buf = new char[2 * sizeof(has_bits_)];
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    buf[i] = has_bits_[i];
    buf[i + sizeof(has_bits_)] = other->has_bits_[i];
  }

  int32 number_tmp__ = other->number();
  other->set_number(number_);
  set_number(number_tmp__);

  std::string alias_tmp__ = std::move(other->mutable_alias());
  other->mutable_alias() = std::move(alias_);
  alias_ = std::move(alias_tmp__);

  Student* captain_tmp__ = other->release_captain();
  other->set_allocated_captain(this->release_captain());
  set_allocated_captain(captain_tmp__);

  ::proto::RepeatedPtrField<Student> students_tmp__ = std::move(other->mutable_students());
  other->mutable_students() = std::move(students_);
  students_ = std::move(students_tmp__);

  // swap has_bits
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = buf[i + sizeof(has_bits_)];
    other->has_bits_[i] = buf[i];
  }
  delete buf;
}

// default_instance()
const SchoolClass& SchoolClass::default_instance() {
  if (default_instance_ == nullptr) {
    static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_simple();
  }
  return *default_instance_;
}

SchoolClass* SchoolClass::default_instance_ = nullptr;

const ::proto::MessageDescriptor* SchoolClass::GetDescriptor() const {
  return SchoolClass_descriptor_;
}

const ::proto::MessageReflection* SchoolClass::GetReflection() const {
  return SchoolClass_reflection_;
}

// destructor
SchoolClass::~SchoolClass() {
  if (captain_) {
    delete captain_;
  }
}

// "number" = 1
bool SchoolClass::has_number() const {
  return (has_bits_[0] & 0x2) != 0;
}

int32 SchoolClass::number() const {
  return number_;
}

void SchoolClass::set_number(int32 number) {
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

// "captain" = 3
bool SchoolClass::has_captain() const {
  return (has_bits_[0] & 0x8) != 0;
}

const Student& SchoolClass::captain() const {
  if (has_captain() && captain_) {
    return *captain_;
  }
  else {
    return Student::default_instance();
  }
}

Student* SchoolClass::mutable_captain() {
  if (has_captain() && captain_) {
    return captain_;
  }
  else {
    captain_ = new Student();
    has_bits_[0] |= 0x8;
    return captain_;
  }
}

void SchoolClass::set_allocated_captain(Student* captain) {
  if (captain_) {
    delete captain_;
  }
  captain_ = captain;
  if (captain_) {
    has_bits_[0] |= 0x8;
  }
  else {
    has_bits_[0] &= (~0x8);
  }
}

Student* SchoolClass::release_captain() {
  Student* captain_tmp__ = captain_;
  captain_ = nullptr;
  has_bits_[0] &= (~0x8);
  return captain_tmp__;
}

void SchoolClass::clear_captain() {
  if (captain_) {
    delete captain_;
  }
  captain_ = nullptr;
  has_bits_[0] &= (~0x8);
}

// "students" = 5
int SchoolClass::students_size() const {
  return students_.size();
}

const Student& SchoolClass::students(int index) const {
  return students_.Get(index);
}

Student* SchoolClass::add_students() {
  return students_.Add();
}

Student* SchoolClass::mutable_students(int index) {
  return students_.GetMutable(index);
}

void SchoolClass::clear_students() {
  students_.Clear();
}

const ::proto::RepeatedPtrField<Student>& SchoolClass::students() const {
  return students_;
}

::proto::RepeatedPtrField<Student>& SchoolClass::mutable_students() {
  return students_;
}

// ******************** StudentRequest ******************** //
// constructor
StudentRequest::StudentRequest() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
StudentRequest::StudentRequest(const StudentRequest& other) {
  CopyFrom(other);
}

// move constructor
StudentRequest::StudentRequest(StudentRequest&& other) {
  MoveFrom(std::move(other));
}

// copy assignment
StudentRequest& StudentRequest::operator=(const StudentRequest& other) {
  CopyFrom(other);
  return *this;
}
// move assignment
StudentRequest& StudentRequest::operator=(StudentRequest&& other) {
  MoveFrom(std::move(other));
  return *this;
}

// New()
::proto::Message* StudentRequest::New() const {
  return reinterpret_cast<::proto::Message*>(new StudentRequest());
}

// CopyFrom()
void StudentRequest::CopyFrom(const StudentRequest& other) {
  op_type_ = other.op_type();
  class_number_ = other.class_number();
  student_name_ = other.student_name();
  if (other.student_) {
    if (!student_) {
      student_ = new Student();
    }
    *student_ = other.student();
  }
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
}

// MoveFrom()
void StudentRequest::MoveFrom(StudentRequest&& other) {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
  op_type_ = other.op_type();
  class_number_ = other.class_number();
  student_name_ = std::move(other.mutable_student_name());
  if (other.student_) {
    if (student_) {
      delete student_;
    }
    student_ = other.student_;
    other.student_ = nullptr;
  }
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    other.has_bits_[i] = 0;
  }
}

// Equals()
bool StudentRequest::Equals(const StudentRequest& other) const {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    if (has_bits_[i] != other.has_bits_[i]) {
      return false;
    }
  }
  if (op_type_ != other.op_type_) {
    return false;
  }
  if (class_number_ != other.class_number_) {
    return false;
  }
  if (student_name_ != other.student_name_) {
    return false;
  }
  if (student_ && other.student_ &&
      !student_->Equals(*other.student_)) {
    return false;
  }
  return true;
}

// Serialize()
::proto::SerializedMessage* StudentRequest::Serialize() const {
  return StudentRequest_reflection_->Serialize(this);
}

// DeSerialize()
void StudentRequest::DeSerialize(const char* buf, unsigned int size) {
  StudentRequest_reflection_->DeSerialize(this, buf, size);
}

// Print()
void StudentRequest::Print(int indent_num) const {
  PrintIndent(indent_num);
  std::cout << "StudentRequest " << "{" << std::endl;
  if (has_op_type()) {
    PrintIndent(indent_num + 1);
    std::string enum_value =
        (reinterpret_cast<const proto::EnumDescriptor*>(
            StudentRequest_descriptor_->FindFieldByName("op_type")->type_descriptor()))
                 ->EnumValueAsString(op_type_);
    std::cout << "op_type: " << enum_value << std::endl;
  }
  if (has_class_number()) {
    PrintIndent(indent_num + 1);
    std::cout << "class_number: " << class_number_ << std::endl;
  }
  if (has_student_name()) {
    PrintIndent(indent_num + 1);
    std::cout << "student_name: " << "\"" << student_name_ << "\"" << std::endl;
  }
  if (has_student()) {
    PrintIndent(indent_num + 1);
    std::cout << "student: " << "*" << std::endl;
    student_->Print(indent_num + 1);
  }
  PrintIndent(indent_num);
  std::cout << "}" << std::endl;
}

// InitAsDefaultInstance()
void StudentRequest::InitAsDefaultInstance() {
  student_ = const_cast<Student*>(&Student::default_instance());
}

// swapper
void StudentRequest::Swap(StudentRequest* other) {
  // store has_bits
  char* buf = new char[2 * sizeof(has_bits_)];
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    buf[i] = has_bits_[i];
    buf[i + sizeof(has_bits_)] = other->has_bits_[i];
  }

  StudentRequest::OpType op_type_tmp__ = other->op_type();
  other->set_op_type(op_type_);
  set_op_type(op_type_tmp__);

  int32 class_number_tmp__ = other->class_number();
  other->set_class_number(class_number_);
  set_class_number(class_number_tmp__);

  std::string student_name_tmp__ = std::move(other->mutable_student_name());
  other->mutable_student_name() = std::move(student_name_);
  student_name_ = std::move(student_name_tmp__);

  Student* student_tmp__ = other->release_student();
  other->set_allocated_student(this->release_student());
  set_allocated_student(student_tmp__);

  // swap has_bits
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = buf[i + sizeof(has_bits_)];
    other->has_bits_[i] = buf[i];
  }
  delete buf;
}

// default_instance()
const StudentRequest& StudentRequest::default_instance() {
  if (default_instance_ == nullptr) {
    static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_simple();
  }
  return *default_instance_;
}

StudentRequest* StudentRequest::default_instance_ = nullptr;

const ::proto::MessageDescriptor* StudentRequest::GetDescriptor() const {
  return StudentRequest_descriptor_;
}

const ::proto::MessageReflection* StudentRequest::GetReflection() const {
  return StudentRequest_reflection_;
}

// destructor
StudentRequest::~StudentRequest() {
  if (student_) {
    delete student_;
  }
}

// "op_type" = 1
bool StudentRequest::has_op_type() const {
  return (has_bits_[0] & 0x2) != 0;
}

StudentRequest::OpType StudentRequest::op_type() const {
  return op_type_;
}

void StudentRequest::set_op_type(StudentRequest::OpType op_type) {
  op_type_ = op_type;
  has_bits_[0] |= 0x2;
}

void StudentRequest::clear_op_type() {
  op_type_ = StudentRequest::ADD;
  has_bits_[0] &= (~0x2);
}

// "class_number" = 2
bool StudentRequest::has_class_number() const {
  return (has_bits_[0] & 0x4) != 0;
}

int32 StudentRequest::class_number() const {
  return class_number_;
}

void StudentRequest::set_class_number(int32 class_number) {
  class_number_ = class_number;
  has_bits_[0] |= 0x4;
}

void StudentRequest::clear_class_number() {
  class_number_ = 0;
  has_bits_[0] &= (~0x4);
}

// "student_name" = 3
bool StudentRequest::has_student_name() const {
  return (has_bits_[0] & 0x8) != 0;
}

const std::string& StudentRequest::student_name() const {
  return student_name_;
}

void StudentRequest::set_student_name(const std::string& student_name) {
  student_name_ = student_name;
  has_bits_[0] |= 0x8;
}

void StudentRequest::set_student_name(const char* student_name) {
  student_name_ = std::string(student_name);
  has_bits_[0] |= 0x8;
}

void StudentRequest::set_student_name(const char* student_name, int size) {
  student_name_ = std::string(student_name, size);
  has_bits_[0] |= 0x8;
}

std::string StudentRequest::mutable_student_name() {
  return student_name_;
}

void StudentRequest::clear_student_name() {
  student_name_ = "";
  has_bits_[0] &= (~0x8);
}

// "student" = 4
bool StudentRequest::has_student() const {
  return (has_bits_[0] & 0x10) != 0;
}

const Student& StudentRequest::student() const {
  if (has_student() && student_) {
    return *student_;
  }
  else {
    return Student::default_instance();
  }
}

Student* StudentRequest::mutable_student() {
  if (has_student() && student_) {
    return student_;
  }
  else {
    student_ = new Student();
    has_bits_[0] |= 0x10;
    return student_;
  }
}

void StudentRequest::set_allocated_student(Student* student) {
  if (student_) {
    delete student_;
  }
  student_ = student;
  if (student_) {
    has_bits_[0] |= 0x10;
  }
  else {
    has_bits_[0] &= (~0x10);
  }
}

Student* StudentRequest::release_student() {
  Student* student_tmp__ = student_;
  student_ = nullptr;
  has_bits_[0] &= (~0x10);
  return student_tmp__;
}

void StudentRequest::clear_student() {
  if (student_) {
    delete student_;
  }
  student_ = nullptr;
  has_bits_[0] &= (~0x10);
}

// ******************** StudentResponse ******************** //
// constructor
StudentResponse::StudentResponse() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
StudentResponse::StudentResponse(const StudentResponse& other) {
  CopyFrom(other);
}

// move constructor
StudentResponse::StudentResponse(StudentResponse&& other) {
  MoveFrom(std::move(other));
}

// copy assignment
StudentResponse& StudentResponse::operator=(const StudentResponse& other) {
  CopyFrom(other);
  return *this;
}
// move assignment
StudentResponse& StudentResponse::operator=(StudentResponse&& other) {
  MoveFrom(std::move(other));
  return *this;
}

// New()
::proto::Message* StudentResponse::New() const {
  return reinterpret_cast<::proto::Message*>(new StudentResponse());
}

// CopyFrom()
void StudentResponse::CopyFrom(const StudentResponse& other) {
  return_code_ = other.return_code();
  error_message_ = other.error_message();
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
}

// MoveFrom()
void StudentResponse::MoveFrom(StudentResponse&& other) {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
  return_code_ = other.return_code();
  error_message_ = std::move(other.mutable_error_message());
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    other.has_bits_[i] = 0;
  }
}

// Equals()
bool StudentResponse::Equals(const StudentResponse& other) const {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    if (has_bits_[i] != other.has_bits_[i]) {
      return false;
    }
  }
  if (return_code_ != other.return_code_) {
    return false;
  }
  if (error_message_ != other.error_message_) {
    return false;
  }
  return true;
}

// Serialize()
::proto::SerializedMessage* StudentResponse::Serialize() const {
  return StudentResponse_reflection_->Serialize(this);
}

// DeSerialize()
void StudentResponse::DeSerialize(const char* buf, unsigned int size) {
  StudentResponse_reflection_->DeSerialize(this, buf, size);
}

// Print()
void StudentResponse::Print(int indent_num) const {
  PrintIndent(indent_num);
  std::cout << "StudentResponse " << "{" << std::endl;
  if (has_return_code()) {
    PrintIndent(indent_num + 1);
    std::cout << "return_code: " << return_code_ << std::endl;
  }
  if (has_error_message()) {
    PrintIndent(indent_num + 1);
    std::cout << "error_message: " << "\"" << error_message_ << "\"" << std::endl;
  }
  PrintIndent(indent_num);
  std::cout << "}" << std::endl;
}

// InitAsDefaultInstance()
void StudentResponse::InitAsDefaultInstance() {
}

// swapper
void StudentResponse::Swap(StudentResponse* other) {
  // store has_bits
  char* buf = new char[2 * sizeof(has_bits_)];
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    buf[i] = has_bits_[i];
    buf[i + sizeof(has_bits_)] = other->has_bits_[i];
  }

  int32 return_code_tmp__ = other->return_code();
  other->set_return_code(return_code_);
  set_return_code(return_code_tmp__);

  std::string error_message_tmp__ = std::move(other->mutable_error_message());
  other->mutable_error_message() = std::move(error_message_);
  error_message_ = std::move(error_message_tmp__);

  // swap has_bits
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = buf[i + sizeof(has_bits_)];
    other->has_bits_[i] = buf[i];
  }
  delete buf;
}

// default_instance()
const StudentResponse& StudentResponse::default_instance() {
  if (default_instance_ == nullptr) {
    static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_simple();
  }
  return *default_instance_;
}

StudentResponse* StudentResponse::default_instance_ = nullptr;

const ::proto::MessageDescriptor* StudentResponse::GetDescriptor() const {
  return StudentResponse_descriptor_;
}

const ::proto::MessageReflection* StudentResponse::GetReflection() const {
  return StudentResponse_reflection_;
}

// destructor
StudentResponse::~StudentResponse() {
}

// "return_code" = 1
bool StudentResponse::has_return_code() const {
  return (has_bits_[0] & 0x2) != 0;
}

int32 StudentResponse::return_code() const {
  return return_code_;
}

void StudentResponse::set_return_code(int32 return_code) {
  return_code_ = return_code;
  has_bits_[0] |= 0x2;
}

void StudentResponse::clear_return_code() {
  return_code_ = -1;
  has_bits_[0] &= (~0x2);
}

// "error_message" = 2
bool StudentResponse::has_error_message() const {
  return (has_bits_[0] & 0x4) != 0;
}

const std::string& StudentResponse::error_message() const {
  return error_message_;
}

void StudentResponse::set_error_message(const std::string& error_message) {
  error_message_ = error_message;
  has_bits_[0] |= 0x4;
}

void StudentResponse::set_error_message(const char* error_message) {
  error_message_ = std::string(error_message);
  has_bits_[0] |= 0x4;
}

void StudentResponse::set_error_message(const char* error_message, int size) {
  error_message_ = std::string(error_message, size);
  has_bits_[0] |= 0x4;
}

std::string StudentResponse::mutable_error_message() {
  return error_message_;
}

void StudentResponse::clear_error_message() {
  error_message_ = "";
  has_bits_[0] &= (~0x4);
}

// *********************** StudentManagement_Stub *********************** //
class StudentManagement::Stub : public StudentManagement {
 public:
  Stub(const char* name, ::RPC::RpcClientChannel* channel, const ::RPC::RpcStubOptions options):
      StudentManagement() {
    ::RPC::RpcService::InitStub(name, channel, options);
  }

  virtual void AddStudent(
      RPC::Rpc* rpc, const ::HaiZhong::StudentRequest* request,
      ::HaiZhong::StudentResponse* response,
      Base::Closure* cb) {
    StartClientRpcCall(rpc, descriptor_, "AddStudent", request, response, cb);
  }

  virtual void DeleteStudent(
      RPC::Rpc* rpc, const ::HaiZhong::StudentRequest* request,
      ::HaiZhong::StudentResponse* response,
      Base::Closure* cb) {
    StartClientRpcCall(rpc, descriptor_, "DeleteStudent", request, response, cb);
  }
};

// *********************** StudentManagement *********************** //
StudentManagement::StudentManagement() : ::RPC::RpcService("HaiZhong.StudentManagement") {
}

const ::RPC::RpcDescriptor* StudentManagement::descriptor() { return descriptor_; }

StudentManagement* StudentManagement::NewStub(::RPC::RpcClientChannel* channel) {
  return new StudentManagement::Stub("HaiZhong.StudentManagement", channel, ::RPC::RpcStubOptions());
}

StudentManagement* StudentManagement::NewStub(
    ::RPC::RpcClientChannel* channel, const ::RPC::RpcStubOptions options) {
  return new StudentManagement::Stub("HaiZhong.StudentManagement", channel, options);
}

void StudentManagement::RegisterToServer(::RPC::RpcServer* server) {
  server->RegisterService("HaiZhong.StudentManagement", this);
  InternalRegisterHandlers(server);
}

void StudentManagement::DeRegisterFromServer(::RPC::RpcServer* server) {
  server->DeRegisterService("HaiZhong.StudentManagement");
  InternalDeRegisterHandlers(server);
}

void StudentManagement::InternalRegisterHandlers(::RPC::RpcServer* server) {
  server->RegisterRpcHandler("HaiZhong.StudentManagement.AddStudent",
      std::make_shared<::RPC::RpcHandler>(
          "HaiZhong.StudentManagement.AddStudent",  // full rpc name
          "AddStudent",  // method name
          &::HaiZhong::StudentRequest::default_instance(),  // request proto type
          &::HaiZhong::StudentResponse::default_instance(),  // response proto type
          nullptr,  // TODO: stream prototype
          std::bind(&StudentManagement::internal_AddStudent,
                    this, std::placeholders::_1)
      )
  );
  server->RegisterRpcHandler("HaiZhong.StudentManagement.DeleteStudent",
      std::make_shared<::RPC::RpcHandler>(
          "HaiZhong.StudentManagement.DeleteStudent",  // full rpc name
          "DeleteStudent",  // method name
          &::HaiZhong::StudentRequest::default_instance(),  // request proto type
          &::HaiZhong::StudentResponse::default_instance(),  // response proto type
          nullptr,  // TODO: stream prototype
          std::bind(&StudentManagement::internal_DeleteStudent,
                    this, std::placeholders::_1)
      )
  );
}

void StudentManagement::InternalDeRegisterHandlers(::RPC::RpcServer* server) {
  server->DeRegisterRpcHandler("HaiZhong.StudentManagement.AddStudent");
  server->DeRegisterRpcHandler("HaiZhong.StudentManagement.DeleteStudent");
}

void StudentManagement::internal_AddStudent(::RPC::Rpc* rpc) {
  AddStudent(
      rpc,
      static_cast<::HaiZhong::StudentRequest*>(rpc->internal_request()),
      static_cast<::HaiZhong::StudentResponse*>(rpc->internal_response()),
      rpc->cb_final()
  );
}

void StudentManagement::internal_DeleteStudent(::RPC::Rpc* rpc) {
  DeleteStudent(
      rpc,
      static_cast<::HaiZhong::StudentRequest*>(rpc->internal_request()),
      static_cast<::HaiZhong::StudentResponse*>(rpc->internal_response()),
      rpc->cb_final()
  );
}

void StudentManagement::AddStudent(
    ::RPC::Rpc* rpc, const ::HaiZhong::StudentRequest*,
    ::HaiZhong::StudentResponse*, ::Base::Closure* done) {
  UnInplemented(rpc, done);
}

void StudentManagement::DeleteStudent(
    ::RPC::Rpc* rpc, const ::HaiZhong::StudentRequest*,
    ::HaiZhong::StudentResponse*, ::Base::Closure* done) {
  UnInplemented(rpc, done);
}

static const RPC::RpcDescriptor* Init_HaiZhong_StudentManagement_Descriptor() {
  ::RPC::RpcDescriptor* descriptor =
      new ::RPC::RpcDescriptor("HaiZhong", "StudentManagement");
  // Add method AddStudent
  {
    ::RPC::RpcMethodDescriptor* method_decpt = descriptor->AddMethod("AddStudent");
    method_decpt->set_arg_type("HaiZhong.StudentRequest");
    method_decpt->set_return_type("HaiZhong.StudentResponse");
    method_decpt->add_option("deadline", "5");
    method_decpt->add_option("security_level", "NONE");
  }
  // Add method DeleteStudent
  {
    ::RPC::RpcMethodDescriptor* method_decpt = descriptor->AddMethod("DeleteStudent");
    method_decpt->set_arg_type("HaiZhong.StudentRequest");
    method_decpt->set_return_type("HaiZhong.StudentResponse");
    method_decpt->add_option("deadline", "3");
  }

  return descriptor;
}

const RPC::RpcDescriptor* StudentManagement::descriptor_ =
    Init_HaiZhong_StudentManagement_Descriptor();

}  // namespace HaiZhong

namespace {

std::string GetProtoContent() {
  return "// test1 proto\n"
"\n"
"package HaiZhong;\n"
"\n"
"message Pet {\n"
"	enum PetType {\n"
"		DOG,\n"
"		PANDA,\n"
"	}\n"
"	optional string name = 1;\n"
"	optional PetType type = 2;\n"
"  repeated PetType friend_to = 3;\n"
"}\n"
"\n"
"message Student {\n"
"	enum Sex {\n"
"		MALE,\n"
"		FEMALE,\n"
"	}\n"
"  optional string name = 1;\n"
"  optional uint32 age = 2;\n"
"  optional int32 xyz = 3;\n"
"  optional uint64 def = 4;\n"
"  optional int64 ghi = 5;\n"
"  optional bool graduated = 6;\n"
"  optional double weight = 7;\n"
"  optional Sex sex = 25;\n"
"  repeated int32 scores = 11;\n"
"  repeated string alias = 15;\n"
"  optional Pet first_pet = 12;\n"
"  repeated Pet pets = 9;\n"
"  optional Student partner = 18;\n"
"  optional char symbol = 19;\n"
"}\n"
"\n"
"message SchoolClass {\n"
"  optional int32 number = 1;\n"
"  optional string alias = 2;\n"
"  optional Student captain = 3;\n"
"	repeated Student students = 5;\n"
"}\n"
"\n"
"message StudentRequest {\n"
"	enum OpType {\n"
"		ADD,\n"
"		REMOVE,\n"
"		UPDATE,\n"
"	}\n"
"	required OpType op_type = 1;\n"
"	required int32 class_number = 2;\n"
"	optional string student_name = 3;\n"
"	optional Student student = 4;\n"
"}\n"
"\n"
"message StudentResponse {\n"
"	required int32 return_code = 1 [default = -1];\n"
"	optional string error_message = 2;\n"
"}\n"
"\n"
"service StudentManagement {\n"
"	rpc AddStudent(StudentRequest) returns (StudentResponse) {\n"
"		option deadline = 5;\n"
"		option security_level = NONE;\n"
"	}\n"
"	rpc DeleteStudent(StudentRequest, bool) returns (StudentResponse) {\n"
"		option deadline = 3;\n"
"	}\n"
"}\n"
"\n"
"\n"
"\n"
;
}

}  // namepsace

