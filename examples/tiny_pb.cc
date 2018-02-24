#include <memory>
#include <mutex>
#include <map>

#include "Proto/Descriptor.h"
#include "Proto/DescriptorsBuilder.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"

#include "tiny_pb.h"

namespace {

const ::proto::MessageDescriptor* FamilyInfo_descriptor_ = nullptr;
const ::proto::MessageReflection* FamilyInfo_reflection_ = nullptr;
const ::proto::MessageDescriptor* DogInfo_descriptor_ = nullptr;
const ::proto::MessageReflection* DogInfo_reflection_ = nullptr;

std::string GetProtoContent();

}  // namepsace

void static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny() {
  if (AA::FamilyInfo::default_instance_ == nullptr) {
    AA::FamilyInfo::default_instance_ = new AA::FamilyInfo();
    AA::FamilyInfo::default_instance_->InitAsDefaultInstance();
  }
  if (AA::BB::DogInfo::default_instance_ == nullptr) {
    AA::BB::DogInfo::default_instance_ = new AA::BB::DogInfo();
    AA::BB::DogInfo::default_instance_->InitAsDefaultInstance();
  }
}

void static_init_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny() {
  static bool already_called = false;
  if (already_called) return;
  already_called = true;

  ::proto::DescriptorsBuilder descriptors_builder(GetProtoContent());
  auto file_dscpt = descriptors_builder.BuildDescriptors();
  CHECK(file_dscpt != nullptr, "Build class descriptor failed.");
  ::proto::MessageFactory::RegisterParsedProtoFile(file_dscpt);

  static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny();

  // static init for class FamilyInfo
  static const int FamilyInfo_offsets_[2] = {
    PROTO_MESSAGE_FIELD_OFFSET(AA::FamilyInfo, address_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::FamilyInfo, numberdogs_),
  };
  FamilyInfo_descriptor_ = file_dscpt->FindMessageTypeByName("AA.FamilyInfo");
  CHECK(FamilyInfo_descriptor_ != nullptr, 
        "Can't find message descriptor for AA.FamilyInfo");
  FamilyInfo_reflection_ = 
      new ::proto::MessageReflection(
          FamilyInfo_descriptor_,
          AA::FamilyInfo::default_instance_,
          FamilyInfo_offsets_,
          PROTO_MESSAGE_FIELD_OFFSET(AA::FamilyInfo, has_bits_));
  ::proto::MessageFactory::RegisterGeneratedMessage(FamilyInfo_reflection_);

  // static init for class DogInfo
  static const int DogInfo_offsets_[9] = {
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, age_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, other_friend_type_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, name_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, sex_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, weight_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, best_friend_type_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, family_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, alias_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, neighbors_),
  };
  DogInfo_descriptor_ = file_dscpt->FindMessageTypeByName("AA.BB.DogInfo");
  CHECK(DogInfo_descriptor_ != nullptr, 
        "Can't find message descriptor for AA.BB.DogInfo");
  DogInfo_reflection_ = 
      new ::proto::MessageReflection(
          DogInfo_descriptor_,
          AA::BB::DogInfo::default_instance_,
          DogInfo_offsets_,
          PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, has_bits_));
  ::proto::MessageFactory::RegisterGeneratedMessage(DogInfo_reflection_);

}

// Force static_init_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny() to be called at initialization time.
struct static_init_forcer_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny {
  static_init_forcer_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny() {
    static_init_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny();
  }
} static_init_forcer_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny_obj_;


namespace AA {

// ******************** FamilyInfo ******************** //
// constructor
FamilyInfo::FamilyInfo() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
FamilyInfo::FamilyInfo(const FamilyInfo& other) {
  CopyFrom(other);
}

// move constructor
FamilyInfo::FamilyInfo(FamilyInfo&& other) {
  MoveFrom(std::move(other));
}

// copy assignment
FamilyInfo& FamilyInfo::operator=(const FamilyInfo& other) {
  CopyFrom(other);
  return *this;
}
// move assignment
FamilyInfo& FamilyInfo::operator=(FamilyInfo&& other) {
  MoveFrom(std::move(other));
  return *this;
}

// New()
::proto::Message* FamilyInfo::New() const {
  return reinterpret_cast<::proto::Message*>(new FamilyInfo());
}

// CopyFrom()
void FamilyInfo::CopyFrom(const FamilyInfo& other) {
  address_ = other.address();
  numberdogs_ = other.numberdogs();
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
}

// MoveFrom()
void FamilyInfo::MoveFrom(FamilyInfo&& other) {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
  address_ = std::move(other.mutable_address());
  numberdogs_ = other.numberdogs();
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    other.has_bits_[i] = 0;
  }
}

// Equals()
bool FamilyInfo::Equals(const FamilyInfo& other) const {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    if (has_bits_[i] != other.has_bits_[i]) {
      return false;
    }
  }
  if (address_ != other.address_) {
    return false;
  }
  if (numberdogs_ != other.numberdogs_) {
    return false;
  }
  return true;
}

// Serialize()
::proto::SerializedMessage* FamilyInfo::Serialize() const {
  return FamilyInfo_reflection_->Serialize(this);
}

// DeSerialize()
void FamilyInfo::DeSerialize(const char* buf, unsigned int size) {
  FamilyInfo_reflection_->DeSerialize(this, buf, size);
}

// Print()
void FamilyInfo::Print(int indent_num) const {
  PrintIndent(indent_num);
  std::cout << "FamilyInfo " << "{" << std::endl;
  if (has_address()) {
    PrintIndent(indent_num + 1);
    std::cout << "address: " << "\"" << address_ << "\"" << std::endl;
  }
  if (has_numberdogs()) {
    PrintIndent(indent_num + 1);
    std::cout << "numberdogs: " << numberdogs_ << std::endl;
  }
  PrintIndent(indent_num);
  std::cout << "}" << std::endl;
}

// InitAsDefaultInstance()
void FamilyInfo::InitAsDefaultInstance() {
}

// swapper
void FamilyInfo::Swap(FamilyInfo* other) {
  // store has_bits
  char* buf = new char[2 * sizeof(has_bits_)];
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    buf[i] = has_bits_[i];
    buf[i + sizeof(has_bits_)] = other->has_bits_[i];
  }

  std::string address_tmp__ = std::move(other->mutable_address());
  other->mutable_address() = std::move(address_);
  address_ = std::move(address_tmp__);

  int numberdogs_tmp__ = other->numberdogs();
  other->set_numberdogs(numberdogs_);
  set_numberdogs(numberdogs_tmp__);

  // swap has_bits
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = buf[i + sizeof(has_bits_)];
    other->has_bits_[i] = buf[i];
  }
  delete buf;
}

// default_instance()
const FamilyInfo& FamilyInfo::default_instance() {
  if (default_instance_ == nullptr) {
    static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny();
  }
  return *default_instance_;
}

FamilyInfo* FamilyInfo::default_instance_ = nullptr;

const ::proto::MessageDescriptor* FamilyInfo::GetDescriptor() const {
  return FamilyInfo_descriptor_;
}

const ::proto::MessageReflection* FamilyInfo::GetReflection() const {
  return FamilyInfo_reflection_;
}

// destructor
FamilyInfo::~FamilyInfo() {
}

// "address" = 1
bool FamilyInfo::has_address() const {
  return (has_bits_[0] & 0x2) != 0;
}

const std::string& FamilyInfo::address() const {
  return address_;
}

void FamilyInfo::set_address(const std::string& address) {
  address_ = address;
  has_bits_[0] |= 0x2;
}

void FamilyInfo::set_address(const char* address) {
  address_ = std::string(address);
  has_bits_[0] |= 0x2;
}

void FamilyInfo::set_address(const char* address, int size) {
  address_ = std::string(address, size);
  has_bits_[0] |= 0x2;
}

std::string FamilyInfo::mutable_address() {
  return address_;
}

void FamilyInfo::clear_address() {
  address_ = "";
  has_bits_[0] &= (~0x2);
}

// "numberdogs" = 2
bool FamilyInfo::has_numberdogs() const {
  return (has_bits_[0] & 0x4) != 0;
}

int FamilyInfo::numberdogs() const {
  return numberdogs_;
}

void FamilyInfo::set_numberdogs(int numberdogs) {
  numberdogs_ = numberdogs;
  has_bits_[0] |= 0x4;
}

void FamilyInfo::clear_numberdogs() {
  numberdogs_ = 0;
  has_bits_[0] &= (~0x4);
}

namespace BB {

// ******************** DogInfo ******************** //
// constructor
DogInfo::DogInfo() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
DogInfo::DogInfo(const DogInfo& other) {
  CopyFrom(other);
}

// move constructor
DogInfo::DogInfo(DogInfo&& other) {
  MoveFrom(std::move(other));
}

// copy assignment
DogInfo& DogInfo::operator=(const DogInfo& other) {
  CopyFrom(other);
  return *this;
}
// move assignment
DogInfo& DogInfo::operator=(DogInfo&& other) {
  MoveFrom(std::move(other));
  return *this;
}

// New()
::proto::Message* DogInfo::New() const {
  return reinterpret_cast<::proto::Message*>(new DogInfo());
}

// CopyFrom()
void DogInfo::CopyFrom(const DogInfo& other) {
  age_ = other.age();
  other_friend_type_ = other.other_friend_type();
  name_ = other.name();
  sex_ = other.sex();
  weight_ = other.weight();
  best_friend_type_ = other.best_friend_type();
  if (other.family_) {
    if (!family_) {
      family_ = new ::AA::FamilyInfo();
    }
    *family_ = other.family();
  }
  for (const std::string* p: other.alias().GetElements()) {
    alias_.AddAllocated(new std::string(*p));
  }
  for (const ::AA::FamilyInfo* p: other.neighbors().GetElements()) {
    neighbors_.AddAllocated(new ::AA::FamilyInfo(*p));
  }
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
}

// MoveFrom()
void DogInfo::MoveFrom(DogInfo&& other) {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
  age_ = other.age();
  other_friend_type_ = std::move(other.mutable_other_friend_type());
  name_ = std::move(other.mutable_name());
  sex_ = other.sex();
  weight_ = std::move(other.mutable_weight());
  best_friend_type_ = other.best_friend_type();
  if (other.family_) {
    if (family_) {
      delete family_;
    }
    family_ = other.family_;
    other.family_ = nullptr;
  }
  alias_ = std::move(other.mutable_alias());
  neighbors_ = std::move(other.mutable_neighbors());
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    other.has_bits_[i] = 0;
  }
}

// Equals()
bool DogInfo::Equals(const DogInfo& other) const {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    if (has_bits_[i] != other.has_bits_[i]) {
      return false;
    }
  }
  if (age_ != other.age_) {
    return false;
  }
  for (unsigned int i = 0; i < other_friend_type_.size(); i++) {
    if (other_friend_type_.at(i) != other.other_friend_type_.at(i)) {
      return false;
    }
  }
  if (name_ != other.name_) {
    return false;
  }
  if (sex_ != other.sex_) {
    return false;
  }
  for (unsigned int i = 0; i < weight_.size(); i++) {
    if (weight_.at(i) != other.weight_.at(i)) {
      return false;
    }
  }
  if (best_friend_type_ != other.best_friend_type_) {
    return false;
  }
  if (family_ && other.family_ &&
      !family_->Equals(*other.family_)) {
    return false;
  }
  for (unsigned int i = 0; i < alias_.size(); i++) {
    if (alias_.at(i) != other.alias_.at(i)) {
      return false;
    }
  }
  for (unsigned int i = 0; i < neighbors_.size(); i++) {
    if (!neighbors_.at(i).Equals(other.neighbors_.at(i))) {
      return false;
    }
  }
  return true;
}

// Serialize()
::proto::SerializedMessage* DogInfo::Serialize() const {
  return DogInfo_reflection_->Serialize(this);
}

// DeSerialize()
void DogInfo::DeSerialize(const char* buf, unsigned int size) {
  DogInfo_reflection_->DeSerialize(this, buf, size);
}

// Print()
void DogInfo::Print(int indent_num) const {
  PrintIndent(indent_num);
  std::cout << "DogInfo " << "{" << std::endl;
  if (has_age()) {
    PrintIndent(indent_num + 1);
    std::cout << "age: " << age_ << std::endl;
  }
  if (other_friend_type_size() > 0) {
    PrintIndent(indent_num + 1);
    std::cout << "other_friend_type: " << "[";
    const auto* enum_type_descriptor =
        (reinterpret_cast<const proto::EnumDescriptor*>(
            DogInfo_descriptor_->FindFieldByName("other_friend_type")->type_descriptor()));
    for (const auto& ele: other_friend_type_) {
        std::cout << enum_type_descriptor->EnumValueAsString(ele) << ", ";
    }
    std::cout << "]" << std::endl;
  }
  if (has_name()) {
    PrintIndent(indent_num + 1);
    std::cout << "name: " << "\"" << name_ << "\"" << std::endl;
  }
  if (has_sex()) {
    PrintIndent(indent_num + 1);
    std::string enum_value =
        (reinterpret_cast<const proto::EnumDescriptor*>(
            DogInfo_descriptor_->FindFieldByName("sex")->type_descriptor()))
                 ->EnumValueAsString(sex_);
    std::cout << "sex: " << enum_value << std::endl;
  }
  if (weight_size() > 0) {
    PrintIndent(indent_num + 1);
    std::cout << "weight: " << "[";
    for (const auto& ele: weight_) {
        std::cout << ele << ", ";
    }
    std::cout << "]" << std::endl;
  }
  if (has_best_friend_type()) {
    PrintIndent(indent_num + 1);
    std::string enum_value =
        (reinterpret_cast<const proto::EnumDescriptor*>(
            DogInfo_descriptor_->FindFieldByName("best_friend_type")->type_descriptor()))
                 ->EnumValueAsString(best_friend_type_);
    std::cout << "best_friend_type: " << enum_value << std::endl;
  }
  if (has_family()) {
    PrintIndent(indent_num + 1);
    std::cout << "family: " << "*" << std::endl;
    family_->Print(indent_num + 1);
  }
  if (alias_size() > 0) {
    PrintIndent(indent_num + 1);
    std::cout << "alias: " << "[";
    for (const auto& ele: alias_) {
        std::cout << "\"" << ele << "\"" << ", ";
    }
    std::cout << "]" << std::endl;
  }
  if (neighbors_size() > 0) {
    PrintIndent(indent_num + 1);
    std::cout << "neighbors: " << "[***]" << std::endl;
    for (const auto& ele: neighbors_) {
        ele.Print(indent_num + 1);
    }
  }
  PrintIndent(indent_num);
  std::cout << "}" << std::endl;
}

// InitAsDefaultInstance()
void DogInfo::InitAsDefaultInstance() {
  family_ = const_cast<::AA::FamilyInfo*>(&::AA::FamilyInfo::default_instance());
}

// swapper
void DogInfo::Swap(DogInfo* other) {
  // store has_bits
  char* buf = new char[2 * sizeof(has_bits_)];
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    buf[i] = has_bits_[i];
    buf[i + sizeof(has_bits_)] = other->has_bits_[i];
  }

  int age_tmp__ = other->age();
  other->set_age(age_);
  set_age(age_tmp__);

  ::proto::RepeatedField<::AA::FriendType> other_friend_type_tmp__ = std::move(other->mutable_other_friend_type());
  other->mutable_other_friend_type() = std::move(other_friend_type_);
  other_friend_type_ = std::move(other_friend_type_tmp__);

  std::string name_tmp__ = std::move(other->mutable_name());
  other->mutable_name() = std::move(name_);
  name_ = std::move(name_tmp__);

  DogInfo::Sex sex_tmp__ = other->sex();
  other->set_sex(sex_);
  set_sex(sex_tmp__);

  ::proto::RepeatedField<double> weight_tmp__ = std::move(other->mutable_weight());
  other->mutable_weight() = std::move(weight_);
  weight_ = std::move(weight_tmp__);

  ::AA::FriendType best_friend_type_tmp__ = other->best_friend_type();
  other->set_best_friend_type(best_friend_type_);
  set_best_friend_type(best_friend_type_tmp__);

  ::AA::FamilyInfo* family_tmp__ = other->release_family();
  other->set_allocated_family(this->release_family());
  set_allocated_family(family_tmp__);

  ::proto::RepeatedPtrField<std::string> alias_tmp__ = std::move(other->mutable_alias());
  other->mutable_alias() = std::move(alias_);
  alias_ = std::move(alias_tmp__);

  ::proto::RepeatedPtrField<::AA::FamilyInfo> neighbors_tmp__ = std::move(other->mutable_neighbors());
  other->mutable_neighbors() = std::move(neighbors_);
  neighbors_ = std::move(neighbors_tmp__);

  // swap has_bits
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = buf[i + sizeof(has_bits_)];
    other->has_bits_[i] = buf[i];
  }
  delete buf;
}

// default_instance()
const DogInfo& DogInfo::default_instance() {
  if (default_instance_ == nullptr) {
    static_init_default_instances_usr_local_google_home_hangyuan_Desktop_test_ProtoBuf_examples_tiny();
  }
  return *default_instance_;
}

DogInfo* DogInfo::default_instance_ = nullptr;

const ::proto::MessageDescriptor* DogInfo::GetDescriptor() const {
  return DogInfo_descriptor_;
}

const ::proto::MessageReflection* DogInfo::GetReflection() const {
  return DogInfo_reflection_;
}

// destructor
DogInfo::~DogInfo() {
  if (family_) {
    delete family_;
  }
}

// "age" = 1
bool DogInfo::has_age() const {
  return (has_bits_[0] & 0x2) != 0;
}

int DogInfo::age() const {
  return age_;
}

void DogInfo::set_age(int age) {
  age_ = age;
  has_bits_[0] |= 0x2;
}

void DogInfo::clear_age() {
  age_ = 0;
  has_bits_[0] &= (~0x2);
}

// "other_friend_type" = 2
int DogInfo::other_friend_type_size() const {
  return other_friend_type_.size();
}

::AA::FriendType DogInfo::other_friend_type(int index) const {
  return other_friend_type_.Get(index);
}

void DogInfo::set_other_friend_type(int index, ::AA::FriendType value) {
  if ((int)other_friend_type_.size() > index) {
    other_friend_type_.Set(index, value);
  }
}

void DogInfo::add_other_friend_type(::AA::FriendType value) {
   other_friend_type_.Add(value);
}

void DogInfo::clear_other_friend_type() {
  other_friend_type_ .Clear();
}

const ::proto::RepeatedField<::AA::FriendType>& DogInfo::other_friend_type() const {
  return other_friend_type_;
}

::proto::RepeatedField<::AA::FriendType>& DogInfo::mutable_other_friend_type() {
  return other_friend_type_;
}

// "name" = 3
bool DogInfo::has_name() const {
  return (has_bits_[0] & 0x8) != 0;
}

const std::string& DogInfo::name() const {
  return name_;
}

void DogInfo::set_name(const std::string& name) {
  name_ = name;
  has_bits_[0] |= 0x8;
}

void DogInfo::set_name(const char* name) {
  name_ = std::string(name);
  has_bits_[0] |= 0x8;
}

void DogInfo::set_name(const char* name, int size) {
  name_ = std::string(name, size);
  has_bits_[0] |= 0x8;
}

std::string DogInfo::mutable_name() {
  return name_;
}

void DogInfo::clear_name() {
  name_ = "";
  has_bits_[0] &= (~0x8);
}

// "sex" = 4
bool DogInfo::has_sex() const {
  return (has_bits_[0] & 0x10) != 0;
}

DogInfo::Sex DogInfo::sex() const {
  return sex_;
}

void DogInfo::set_sex(DogInfo::Sex sex) {
  sex_ = sex;
  has_bits_[0] |= 0x10;
}

void DogInfo::clear_sex() {
  sex_ = DogInfo::MALE;
  has_bits_[0] &= (~0x10);
}

// "weight" = 5
int DogInfo::weight_size() const {
  return weight_.size();
}

double DogInfo::weight(int index) const {
  return weight_.Get(index);
}

void DogInfo::set_weight(int index, double value) {
  if ((int)weight_.size() > index) {
    weight_.Set(index, value);
  }
}

void DogInfo::add_weight(double value) {
   weight_.Add(value);
}

void DogInfo::clear_weight() {
  weight_ .Clear();
}

const ::proto::RepeatedField<double>& DogInfo::weight() const {
  return weight_;
}

::proto::RepeatedField<double>& DogInfo::mutable_weight() {
  return weight_;
}

// "best_friend_type" = 10
bool DogInfo::has_best_friend_type() const {
  return (has_bits_[1] & 0x4) != 0;
}

::AA::FriendType DogInfo::best_friend_type() const {
  return best_friend_type_;
}

void DogInfo::set_best_friend_type(::AA::FriendType best_friend_type) {
  best_friend_type_ = best_friend_type;
  has_bits_[1] |= 0x4;
}

void DogInfo::clear_best_friend_type() {
  best_friend_type_ = ::AA::Human;
  has_bits_[1] &= (~0x4);
}

// "family" = 13
bool DogInfo::has_family() const {
  return (has_bits_[1] & 0x20) != 0;
}

const ::AA::FamilyInfo& DogInfo::family() const {
  if (has_family() && family_) {
    return *family_;
  }
  else {
    return ::AA::FamilyInfo::default_instance();
  }
}

::AA::FamilyInfo* DogInfo::mutable_family() {
  if (has_family() && family_) {
    return family_;
  }
  else {
    family_ = new ::AA::FamilyInfo();
    has_bits_[1] |= 0x20;
    return family_;
  }
}

void DogInfo::set_allocated_family(::AA::FamilyInfo* family) {
  if (family_) {
    delete family_;
  }
  family_ = family;
  if (family_) {
    has_bits_[1] |= 0x20;
  }
  else {
    has_bits_[1] &= (~0x20);
  }
}

::AA::FamilyInfo* DogInfo::release_family() {
  ::AA::FamilyInfo* family_tmp__ = family_;
  family_ = nullptr;
  has_bits_[1] &= (~0x20);
  return family_tmp__;
}

void DogInfo::clear_family() {
  if (family_) {
    delete family_;
  }
  family_ = nullptr;
  has_bits_[1] &= (~0x20);
}

// "alias" = 18
int DogInfo::alias_size() const {
  return alias_.size();
}

const std::string& DogInfo::alias(int index) const {
  return alias_.Get(index);
}

void DogInfo::set_alias(int index, const std::string& value) {
  if (index < (int)alias_.size()) {
    alias_.Set(index, value);
  }
}

void DogInfo::set_alias(int index, const char* value) {
  if (index < (int)alias_.size()) {
    alias_.Set(index, std::string(value));
  }
}

void DogInfo::set_alias(int index, const char* value, int size) {
  if (index < (int)alias_.size()) {
    alias_.Set(index, std::string(value, size));
  }
}

std::string* DogInfo::add_alias() {
  return alias_.Add();
}

void DogInfo::add_alias(const std::string& value) {
  alias_.AddAllocated(new std::string(value));
}

void DogInfo::add_alias(const char* value) {
  alias_.AddAllocated(new std::string(value));
}

void DogInfo::add_alias(const char* value, int size) {
  alias_.AddAllocated(new std::string(value, size));
}

std::string* DogInfo::mutable_alias(int index) {
  return alias_.GetMutable(index);
}

void DogInfo::clear_alias() {
  alias_.Clear();
}

const ::proto::RepeatedPtrField<std::string>& DogInfo::alias() const {
  return alias_;
}

::proto::RepeatedPtrField<std::string>& DogInfo::mutable_alias() {
  return alias_;
}

// "neighbors" = 25
int DogInfo::neighbors_size() const {
  return neighbors_.size();
}

const ::AA::FamilyInfo& DogInfo::neighbors(int index) const {
  return neighbors_.Get(index);
}

::AA::FamilyInfo* DogInfo::add_neighbors() {
  return neighbors_.Add();
}

::AA::FamilyInfo* DogInfo::mutable_neighbors(int index) {
  return neighbors_.GetMutable(index);
}

void DogInfo::clear_neighbors() {
  neighbors_.Clear();
}

const ::proto::RepeatedPtrField<::AA::FamilyInfo>& DogInfo::neighbors() const {
  return neighbors_;
}

::proto::RepeatedPtrField<::AA::FamilyInfo>& DogInfo::mutable_neighbors() {
  return neighbors_;
}

}  // namespace BB
}  // namespace AA

namespace {

std::string GetProtoContent() {
  return "// test1 proto\n"
"\n"
"package AA;\n"
"\n"
"message FamilyInfo {\n"
"	optional string address = 1 [default = \"Haimen Street\"];\n"
"	optional int32 numberdogs = 2 [default=0];\n"
"}\n"
"\n"
"enum FriendType{\n"
"  Human,\n"
"  Dog,\n"
"  Panda,\n"
"  Food,\n"
"}\n"
"\n"
"package AA.BB;\n"
"\n"
"message DogInfo {\n"
"	enum Sex {\n"
"		MALE,\n"
"		FEMALE,\n"
"	}\n"
"  optional int32 age =1;  // singluar primitive\n"
"  optional Sex sex = 4;   // singular enum\n"
"  optional AA.FriendType best_friend_type = 10;  // singular enum\n"
"  optional string name= 3;  // singular string\n"
"  optional AA.FamilyInfo family = 13;  // singular string\n"
"\n"
"  repeated double weight = 5;  // repeated primitive\n"
"  repeated string alias = 18;  // repeated string\n"
"  repeated AA.FriendType other_friend_type = 2;  // repeated enum\n"
"  repeated AA.FamilyInfo neighbors = 25;  // repeated message\n"
"}\n"
"\n"
"\n"
;
}

}  // namepsace

