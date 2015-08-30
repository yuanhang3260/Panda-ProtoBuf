#include <memory>

#include "Compiler/Message.h"
#include "Compiler/ProtoParser.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"

#include "tiny_pb.h"

namespace {

std::shared_ptr<::proto::ProtoParser::Message> FamilyInfo_descriptor_;
std::shared_ptr<::proto::MessageReflection> FamilyInfo_reflection_;
std::shared_ptr<::proto::ProtoParser::Message> DogInfo_descriptor_;
std::shared_ptr<::proto::MessageReflection> DogInfo_reflection_;

}  // namepsace

void static_init_default_instances_samples_tiny() {
  static bool already_called = false;
  if (already_called) return;
  already_called = true;

  if (AA::FamilyInfo::default_instance_ == NULL) {
    AA::FamilyInfo::default_instance_ = new AA::FamilyInfo();
    AA::FamilyInfo::default_instance_->InitAsDefaultInstance();
  }
  if (AA::BB::DogInfo::default_instance_ == NULL) {
    AA::BB::DogInfo::default_instance_ = new AA::BB::DogInfo();
    AA::BB::DogInfo::default_instance_->InitAsDefaultInstance();
  }
}

void static_init_samples_tiny() {
  ::proto::ProtoParser::ProtoParser parser(
      ::proto::ProtoParser::CPP,
      "./samples/tiny.proto");
  CHECK(parser.ParseProto(),
        "static class initialization for ./samples/tiny.proto failed");

  static_init_default_instances_samples_tiny();

  int i = 0;
  // static init for class FamilyInfo
  static const int FamilyInfo_offsets_[2] = {
    PROTO_MESSAGE_FIELD_OFFSET(AA::FamilyInfo, address_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::FamilyInfo, numberdogs_),
  };
  i = 0;
  for (auto& field: parser.mutable_messages_list()[0]->mutable_fields_list()) {
    field->set_field_offset(FamilyInfo_offsets_[i++]);
  }
  FamilyInfo_descriptor_ = parser.mutable_messages_list()[0];
  FamilyInfo_reflection_.reset(
      new ::proto::MessageReflection(
          FamilyInfo_descriptor_,
          AA::FamilyInfo::default_instance_,
          PROTO_MESSAGE_FIELD_OFFSET(AA::FamilyInfo, has_bits_))
  );
  ::proto::MessageFactory::RegisterGeneratedMessage(FamilyInfo_reflection_);

  // static init for class DogInfo
  static const int DogInfo_offsets_[7] = {
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, age_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, name_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, sex_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, weights_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, family_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, alias_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, neighbors_),
  };
  i = 0;
  for (auto& field: parser.mutable_messages_list()[1]->mutable_fields_list()) {
    field->set_field_offset(DogInfo_offsets_[i++]);
  }
  DogInfo_descriptor_ = parser.mutable_messages_list()[1];
  DogInfo_reflection_.reset(
      new ::proto::MessageReflection(
          DogInfo_descriptor_,
          AA::BB::DogInfo::default_instance_,
          PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, has_bits_))
  );
  ::proto::MessageFactory::RegisterGeneratedMessage(DogInfo_reflection_);

}

// Force static_init_samples_tiny() to be called at initialization time.
struct static_init_forcer_samples_tiny {
  static_init_forcer_samples_tiny() {
    static_init_samples_tiny();
  }
} static_init_forcer_samples_tiny_obj_;


namespace AA {

// -------------------- FamilyInfo --------------------- //
// constructor
FamilyInfo::FamilyInfo() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
FamilyInfo::FamilyInfo(const FamilyInfo& other) {
  address_ = other.address();
  numberdogs_ = other.numberdogs();
}

// move constructor
FamilyInfo::FamilyInfo(FamilyInfo&& other) {
  address_ = std::move(other.mutable_address());
  numberdogs_ = other.numberdogs();
  other.clear_numberdogs();
}

// copy assignment
FamilyInfo& FamilyInfo::operator=(const FamilyInfo& other) {
  address_ = other.address();
  numberdogs_ = other.numberdogs();
  return *this;
}

// move assignment
FamilyInfo& FamilyInfo::operator=(FamilyInfo&& other) {
  address_ = std::move(other.mutable_address());
  numberdogs_ = other.numberdogs();
  other.clear_numberdogs();
  return *this;
}

// New()
::proto::Message* FamilyInfo::New() {
  return reinterpret_cast<::proto::Message*>(new FamilyInfo());
}

// Serialize()
::proto::SerializedMessage* FamilyInfo::Serialize() {
  return FamilyInfo_reflection_->Serialize(this);
}

// InitAsDefaultInstance()
void FamilyInfo::InitAsDefaultInstance() {
}

// swapper
void FamilyInfo::Swap(FamilyInfo* other) {
  std::string address_tmp__ = std::move(other->mutable_address());
  other->mutable_address() = std::move(address_);
  address_ = std::move(address_tmp__);

  int numberdogs_tmp__ = other->numberdogs();
  other->set_numberdogs(numberdogs_);
  set_numberdogs(numberdogs_tmp__);
}

// default_instance()
const FamilyInfo& FamilyInfo::default_instance() {
  if (default_instance_ == NULL) {
    static_init_default_instances_samples_tiny();
  }
  return *default_instance_;
}

FamilyInfo* FamilyInfo::default_instance_ = NULL;

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

// -------------------- DogInfo --------------------- //
// constructor
DogInfo::DogInfo() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
DogInfo::DogInfo(const DogInfo& other) {
  age_ = other.age();
  name_ = other.name();
  sex_ = other.sex();
  weights_ = other.weights();
  if (!family_) {
    family_ = new ::AA::FamilyInfo();
  }
  *family_ = other.family();
  for (const std::string* p: other.alias().GetElements()) {
    alias_.AddAllocated(new std::string(*p));
  }
  for (const ::AA::FamilyInfo* p: other.neighbors().GetElements()) {
    neighbors_.AddAllocated(new ::AA::FamilyInfo(*p));
  }
}

// move constructor
DogInfo::DogInfo(DogInfo&& other) {
  age_ = other.age();
  other.clear_age();
  name_ = std::move(other.mutable_name());
  sex_ = std::move(other.mutable_sex());
  weights_ = std::move(other.mutable_weights());
  if (family_ ) {
    delete family_;
  }
  family_ = other.release_family();
  alias_ = std::move(other.mutable_alias());
  neighbors_ = std::move(other.mutable_neighbors());
}

// copy assignment
DogInfo& DogInfo::operator=(const DogInfo& other) {
  age_ = other.age();
  name_ = other.name();
  sex_ = other.sex();
  weights_ = other.weights();
  if (!family_) {
    family_ = new ::AA::FamilyInfo();
  }
  *family_ = other.family();
  for (const std::string* p: other.alias().GetElements()) {
    alias_.AddAllocated(new std::string(*p));
  }
  for (const ::AA::FamilyInfo* p: other.neighbors().GetElements()) {
    neighbors_.AddAllocated(new ::AA::FamilyInfo(*p));
  }
  return *this;
}

// move assignment
DogInfo& DogInfo::operator=(DogInfo&& other) {
  age_ = other.age();
  other.clear_age();
  name_ = std::move(other.mutable_name());
  sex_ = std::move(other.mutable_sex());
  weights_ = std::move(other.mutable_weights());
  if (family_ ) {
    delete family_;
  }
  family_ = other.release_family();
  alias_ = std::move(other.mutable_alias());
  neighbors_ = std::move(other.mutable_neighbors());
  return *this;
}

// New()
::proto::Message* DogInfo::New() {
  return reinterpret_cast<::proto::Message*>(new DogInfo());
}

// Serialize()
::proto::SerializedMessage* DogInfo::Serialize() {
  return DogInfo_reflection_->Serialize(this);
}

// InitAsDefaultInstance()
void DogInfo::InitAsDefaultInstance() {
  family_ = const_cast<::AA::FamilyInfo*>(&::AA::FamilyInfo::default_instance());
}

// swapper
void DogInfo::Swap(DogInfo* other) {
  int age_tmp__ = other->age();
  other->set_age(age_);
  set_age(age_tmp__);

  std::string name_tmp__ = std::move(other->mutable_name());
  other->mutable_name() = std::move(name_);
  name_ = std::move(name_tmp__);

  ::proto::RepeatedField<DogInfo::Sex> sex_tmp__ = std::move(other->mutable_sex());
  other->mutable_sex() = std::move(sex_);
  sex_ = std::move(sex_tmp__);

  ::proto::RepeatedField<double> weights_tmp__ = std::move(other->mutable_weights());
  other->mutable_weights() = std::move(weights_);
  weights_ = std::move(weights_tmp__);

  ::AA::FamilyInfo* family_tmp__ = other->release_family();
  other->set_allocated_family(this->release_family());
  set_allocated_family(family_tmp__);

  ::proto::RepeatedPtrField<std::string> alias_tmp__ = std::move(other->mutable_alias());
  other->mutable_alias() = std::move(alias_);
  alias_ = std::move(alias_tmp__);

  ::proto::RepeatedPtrField<::AA::FamilyInfo> neighbors_tmp__ = std::move(other->mutable_neighbors());
  other->mutable_neighbors() = std::move(neighbors_);
  neighbors_ = std::move(neighbors_tmp__);
}

// default_instance()
const DogInfo& DogInfo::default_instance() {
  if (default_instance_ == NULL) {
    static_init_default_instances_samples_tiny();
  }
  return *default_instance_;
}

DogInfo* DogInfo::default_instance_ = NULL;

// destructor
DogInfo::~DogInfo() {
  delete family_;
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
int DogInfo::sex_size() const {
  return sex_.size();
}

DogInfo::Sex DogInfo::sex(int index) {
  return sex_.Get(index);
}

void DogInfo::set_sex(int index, DogInfo::Sex value) {
  if ((int)sex_.size() > index) {
    sex_.Set(index, value);
  }
}

void DogInfo::add_sex(DogInfo::Sex value) {
   sex_.Add(value);
}

void DogInfo::clear_sex() {
  sex_ .Clear();
}

const ::proto::RepeatedField<DogInfo::Sex>& DogInfo::sex() const {
  return sex_;
}

::proto::RepeatedField<DogInfo::Sex>& DogInfo::mutable_sex() {
  return sex_;
}

// "weights" = 5
int DogInfo::weights_size() const {
  return weights_.size();
}

double DogInfo::weights(int index) {
  return weights_.Get(index);
}

void DogInfo::set_weights(int index, double value) {
  if ((int)weights_.size() > index) {
    weights_.Set(index, value);
  }
}

void DogInfo::add_weights(double value) {
   weights_.Add(value);
}

void DogInfo::clear_weights() {
  weights_ .Clear();
}

const ::proto::RepeatedField<double>& DogInfo::weights() const {
  return weights_;
}

::proto::RepeatedField<double>& DogInfo::mutable_weights() {
  return weights_;
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

const std::string& DogInfo::alias(int index) {
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

const ::AA::FamilyInfo& DogInfo::neighbors(int index) {
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

