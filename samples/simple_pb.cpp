#include <memory>

#include "Compiler/Message.h"
#include "Compiler/ProtoParser.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"

#include "simple_pb.h"

namespace {

std::shared_ptr<::proto::ProtoParser::Message> DogInfo_descriptor_;
std::shared_ptr<::proto::MessageReflection> DogInfo_reflection_;
std::shared_ptr<::proto::ProtoParser::Message> FamilyInfo_descriptor_;
std::shared_ptr<::proto::MessageReflection> FamilyInfo_reflection_;
std::shared_ptr<::proto::ProtoParser::Message> DogFamilyInfo_descriptor_;
std::shared_ptr<::proto::MessageReflection> DogFamilyInfo_reflection_;

}  // namepsace

void static_init_default_instances_samples_simple() {
  static bool already_called = false;
  if (already_called) return;
  already_called = true;

  if (AA::BB::DogInfo::default_instance_ == NULL) {
    AA::BB::DogInfo::default_instance_ = new AA::BB::DogInfo();
    AA::BB::DogInfo::default_instance_->InitAsDefaultInstance();
  }
  if (AA::FamilyInfo::default_instance_ == NULL) {
    AA::FamilyInfo::default_instance_ = new AA::FamilyInfo();
    AA::FamilyInfo::default_instance_->InitAsDefaultInstance();
  }
  if (AA::BB::CC::DogFamilyInfo::default_instance_ == NULL) {
    AA::BB::CC::DogFamilyInfo::default_instance_ = new AA::BB::CC::DogFamilyInfo();
    AA::BB::CC::DogFamilyInfo::default_instance_->InitAsDefaultInstance();
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
  // static init for class DogInfo
  static const int DogInfo_offsets_[4] = {
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, age_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, name_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, friend_type_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, sex_),
  };
  i = 0;
  for (auto& field: parser.mutable_messages_list()[0]->mutable_fields_list()) {
    field->set_field_offset(DogInfo_offsets_[i++]);
  }
  DogInfo_descriptor_ = parser.mutable_messages_list()[0];
  DogInfo_reflection_.reset(
      new ::proto::MessageReflection(
          DogInfo_descriptor_,
          AA::BB::DogInfo::default_instance_,
          PROTO_MESSAGE_FIELD_OFFSET(AA::BB::DogInfo, has_bits_))
  );
  ::proto::MessageFactory::RegisterGeneratedMessage(DogInfo_reflection_);

  // static init for class FamilyInfo
  static const int FamilyInfo_offsets_[2] = {
    PROTO_MESSAGE_FIELD_OFFSET(AA::FamilyInfo, address_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::FamilyInfo, numberdogs_),
  };
  i = 0;
  for (auto& field: parser.mutable_messages_list()[1]->mutable_fields_list()) {
    field->set_field_offset(FamilyInfo_offsets_[i++]);
  }
  FamilyInfo_descriptor_ = parser.mutable_messages_list()[1];
  FamilyInfo_reflection_.reset(
      new ::proto::MessageReflection(
          FamilyInfo_descriptor_,
          AA::FamilyInfo::default_instance_,
          PROTO_MESSAGE_FIELD_OFFSET(AA::FamilyInfo, has_bits_))
  );
  ::proto::MessageFactory::RegisterGeneratedMessage(FamilyInfo_reflection_);

  // static init for class DogFamilyInfo
  static const int DogFamilyInfo_offsets_[4] = {
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::CC::DogFamilyInfo, family_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::CC::DogFamilyInfo, utility_fee_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::CC::DogFamilyInfo, dogs_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::CC::DogFamilyInfo, locations_),
  };
  i = 0;
  for (auto& field: parser.mutable_messages_list()[2]->mutable_fields_list()) {
    field->set_field_offset(DogFamilyInfo_offsets_[i++]);
  }
  DogFamilyInfo_descriptor_ = parser.mutable_messages_list()[2];
  DogFamilyInfo_reflection_.reset(
      new ::proto::MessageReflection(
          DogFamilyInfo_descriptor_,
          AA::BB::CC::DogFamilyInfo::default_instance_,
          PROTO_MESSAGE_FIELD_OFFSET(AA::BB::CC::DogFamilyInfo, has_bits_))
  );
  ::proto::MessageFactory::RegisterGeneratedMessage(DogFamilyInfo_reflection_);

}

// Force static_init_samples_simple() to be called at initialization time.
struct static_init_forcer_samples_simple {
  static_init_forcer_samples_simple() {
    static_init_samples_simple();
  }
} static_init_forcer_samples_simple_obj_;


namespace AA {
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
  friend_type_ = other.friend_type();
  sex_ = other.sex();
}

// move constructor
DogInfo::DogInfo(DogInfo&& other) {
  age_ = other.age();
  other.clear_age();
  name_ = std::move(other.mutable_name());
  friend_type_ = other.friend_type();
  other.clear_friend_type();
  sex_ = other.sex();
  other.clear_sex();
}

// copy assignment
DogInfo& DogInfo::operator=(const DogInfo& other) {
  age_ = other.age();
  name_ = other.name();
  friend_type_ = other.friend_type();
  sex_ = other.sex();
  return *this;
}

// move assignment
DogInfo& DogInfo::operator=(DogInfo&& other) {
  age_ = other.age();
  other.clear_age();
  name_ = std::move(other.mutable_name());
  friend_type_ = other.friend_type();
  other.clear_friend_type();
  sex_ = other.sex();
  other.clear_sex();
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
}

// swapper
void DogInfo::Swap(DogInfo* other) {
  int age_tmp__ = other->age();
  other->set_age(age_);
  set_age(age_tmp__);

  std::string name_tmp__ = std::move(other->mutable_name());
  other->mutable_name() = std::move(name_);
  name_ = std::move(name_tmp__);

  ::AA::FriendType friend_type_tmp__ = other->friend_type();
  other->set_friend_type(friend_type_);
  set_friend_type(friend_type_tmp__);

  DogInfo::Sex sex_tmp__ = other->sex();
  other->set_sex(sex_);
  set_sex(sex_tmp__);
}

// default_instance()
const DogInfo& DogInfo::default_instance() {
  if (default_instance_ == NULL) {
    static_init_default_instances_samples_simple();
  }
  return *default_instance_;
}

DogInfo* DogInfo::default_instance_ = NULL;

// destructor
DogInfo::~DogInfo() {
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

// "name" = 2
bool DogInfo::has_name() const {
  return (has_bits_[0] & 0x4) != 0;
}

const std::string& DogInfo::name() const {
  return name_;
}

void DogInfo::set_name(const std::string& name) {
  name_ = name;
  has_bits_[0] |= 0x4;
}

void DogInfo::set_name(const char* name) {
  name_ = std::string(name);
  has_bits_[0] |= 0x4;
}

void DogInfo::set_name(const char* name, int size) {
  name_ = std::string(name, size);
  has_bits_[0] |= 0x4;
}

std::string DogInfo::mutable_name() {
  return name_;
}

void DogInfo::clear_name() {
  name_ = "";
  has_bits_[0] &= (~0x4);
}

// "friend_type" = 4
bool DogInfo::has_friend_type() const {
  return (has_bits_[0] & 0x10) != 0;
}

::AA::FriendType DogInfo::friend_type() const {
  return friend_type_;
}

void DogInfo::set_friend_type(::AA::FriendType friend_type) {
  friend_type_ = friend_type;
  has_bits_[0] |= 0x10;
}

void DogInfo::clear_friend_type() {
  friend_type_ = ::AA::Human;
  has_bits_[0] &= (~0x10);
}

// "sex" = 6
bool DogInfo::has_sex() const {
  return (has_bits_[0] & 0x40) != 0;
}

DogInfo::Sex DogInfo::sex() const {
  return sex_;
}

void DogInfo::set_sex(DogInfo::Sex sex) {
  sex_ = sex;
  has_bits_[0] |= 0x40;
}

void DogInfo::clear_sex() {
  sex_ = DogInfo::Male;
  has_bits_[0] &= (~0x40);
}

}  // namespace BB

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
    static_init_default_instances_samples_simple();
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
namespace CC {

// -------------------- DogFamilyInfo --------------------- //
// constructor
DogFamilyInfo::DogFamilyInfo() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
DogFamilyInfo::DogFamilyInfo(const DogFamilyInfo& other) {
  if (!family_) {
    family_ = new ::AA::FamilyInfo();
  }
  *family_ = other.family();
  utility_fee_ = other.utility_fee();
  for (const ::AA::BB::DogInfo* p: other.dogs().GetElements()) {
    dogs_.AddAllocated(new ::AA::BB::DogInfo(*p));
  }
  for (const std::string* p: other.locations().GetElements()) {
    locations_.AddAllocated(new std::string(*p));
  }
}

// move constructor
DogFamilyInfo::DogFamilyInfo(DogFamilyInfo&& other) {
  if (family_ ) {
    delete family_;
  }
  family_ = other.release_family();
  utility_fee_ = std::move(other.mutable_utility_fee());
  dogs_ = std::move(other.mutable_dogs());
  locations_ = std::move(other.mutable_locations());
}

// copy assignment
DogFamilyInfo& DogFamilyInfo::operator=(const DogFamilyInfo& other) {
  if (!family_) {
    family_ = new ::AA::FamilyInfo();
  }
  *family_ = other.family();
  utility_fee_ = other.utility_fee();
  for (const ::AA::BB::DogInfo* p: other.dogs().GetElements()) {
    dogs_.AddAllocated(new ::AA::BB::DogInfo(*p));
  }
  for (const std::string* p: other.locations().GetElements()) {
    locations_.AddAllocated(new std::string(*p));
  }
  return *this;
}

// move assignment
DogFamilyInfo& DogFamilyInfo::operator=(DogFamilyInfo&& other) {
  if (family_ ) {
    delete family_;
  }
  family_ = other.release_family();
  utility_fee_ = std::move(other.mutable_utility_fee());
  dogs_ = std::move(other.mutable_dogs());
  locations_ = std::move(other.mutable_locations());
  return *this;
}

// New()
::proto::Message* DogFamilyInfo::New() {
  return reinterpret_cast<::proto::Message*>(new DogFamilyInfo());
}

// Serialize()
::proto::SerializedMessage* DogFamilyInfo::Serialize() {
  return DogFamilyInfo_reflection_->Serialize(this);
}

// InitAsDefaultInstance()
void DogFamilyInfo::InitAsDefaultInstance() {
  family_ = const_cast<::AA::FamilyInfo*>(&::AA::FamilyInfo::default_instance());
}

// swapper
void DogFamilyInfo::Swap(DogFamilyInfo* other) {
  ::AA::FamilyInfo* family_tmp__ = other->release_family();
  other->set_allocated_family(this->release_family());
  set_allocated_family(family_tmp__);

  ::proto::RepeatedField<double> utility_fee_tmp__ = std::move(other->mutable_utility_fee());
  other->mutable_utility_fee() = std::move(utility_fee_);
  utility_fee_ = std::move(utility_fee_tmp__);

  ::proto::RepeatedPtrField<::AA::BB::DogInfo> dogs_tmp__ = std::move(other->mutable_dogs());
  other->mutable_dogs() = std::move(dogs_);
  dogs_ = std::move(dogs_tmp__);

  ::proto::RepeatedPtrField<std::string> locations_tmp__ = std::move(other->mutable_locations());
  other->mutable_locations() = std::move(locations_);
  locations_ = std::move(locations_tmp__);
}

// default_instance()
const DogFamilyInfo& DogFamilyInfo::default_instance() {
  if (default_instance_ == NULL) {
    static_init_default_instances_samples_simple();
  }
  return *default_instance_;
}

DogFamilyInfo* DogFamilyInfo::default_instance_ = NULL;

// destructor
DogFamilyInfo::~DogFamilyInfo() {
  delete family_;
}

// "family" = 1
bool DogFamilyInfo::has_family() const {
  return (has_bits_[0] & 0x2) != 0;
}

const ::AA::FamilyInfo& DogFamilyInfo::family() const {
  if (has_family() && family_) {
    return *family_;
  }
  else {
    return ::AA::FamilyInfo::default_instance();
  }
}

::AA::FamilyInfo* DogFamilyInfo::mutable_family() {
  if (has_family() && family_) {
    return family_;
  }
  else {
    family_ = new ::AA::FamilyInfo();
    has_bits_[0] |= 0x2;
    return family_;
  }
}

void DogFamilyInfo::set_allocated_family(::AA::FamilyInfo* family) {
  if (family_) {
    delete family_;
  }
  family_ = family;
  if (family_) {
    has_bits_[0] |= 0x2;
  }
  else {
    has_bits_[0] &= (~0x2);
  }
}

::AA::FamilyInfo* DogFamilyInfo::release_family() {
  ::AA::FamilyInfo* family_tmp__ = family_;
  family_ = nullptr;
  has_bits_[0] &= (~0x2);
  return family_tmp__;
}

void DogFamilyInfo::clear_family() {
  if (family_) {
    delete family_;
  }
  family_ = nullptr;
  has_bits_[0] &= (~0x2);
}

// "utility_fee" = 2
int DogFamilyInfo::utility_fee_size() const {
  return utility_fee_.size();
}

double DogFamilyInfo::utility_fee(int index) {
  return utility_fee_.Get(index);
}

void DogFamilyInfo::set_utility_fee(int index, double value) {
  if ((int)utility_fee_.size() > index) {
    utility_fee_.Set(index, value);
  }
}

void DogFamilyInfo::add_utility_fee(double value) {
   utility_fee_.Add(value);
}

void DogFamilyInfo::clear_utility_fee() {
  utility_fee_ .Clear();
}

const ::proto::RepeatedField<double>& DogFamilyInfo::utility_fee() const {
  return utility_fee_;
}

::proto::RepeatedField<double>& DogFamilyInfo::mutable_utility_fee() {
  return utility_fee_;
}

// "dogs" = 3
int DogFamilyInfo::dogs_size() const {
  return dogs_.size();
}

const ::AA::BB::DogInfo& DogFamilyInfo::dogs(int index) {
  return dogs_.Get(index);
}

::AA::BB::DogInfo* DogFamilyInfo::add_dogs() {
  return dogs_.Add();
}

::AA::BB::DogInfo* DogFamilyInfo::mutable_dogs(int index) {
  return dogs_.GetMutable(index);
}

void DogFamilyInfo::clear_dogs() {
  dogs_.Clear();
}

const ::proto::RepeatedPtrField<::AA::BB::DogInfo>& DogFamilyInfo::dogs() const {
  return dogs_;
}

::proto::RepeatedPtrField<::AA::BB::DogInfo>& DogFamilyInfo::mutable_dogs() {
  return dogs_;
}

// "locations" = 4
int DogFamilyInfo::locations_size() const {
  return locations_.size();
}

const std::string& DogFamilyInfo::locations(int index) {
  return locations_.Get(index);
}

void DogFamilyInfo::set_locations(int index, const std::string& value) {
  if (index < (int)locations_.size()) {
    locations_.Set(index, value);
  }
}

void DogFamilyInfo::set_locations(int index, const char* value) {
  if (index < (int)locations_.size()) {
    locations_.Set(index, std::string(value));
  }
}

void DogFamilyInfo::set_locations(int index, const char* value, int size) {
  if (index < (int)locations_.size()) {
    locations_.Set(index, std::string(value, size));
  }
}

std::string* DogFamilyInfo::add_locations() {
  return locations_.Add();
}

void DogFamilyInfo::add_locations(const std::string& value) {
  locations_.AddAllocated(new std::string(value));
}

void DogFamilyInfo::add_locations(const char* value) {
  locations_.AddAllocated(new std::string(value));
}

void DogFamilyInfo::add_locations(const char* value, int size) {
  locations_.AddAllocated(new std::string(value, size));
}

std::string* DogFamilyInfo::mutable_locations(int index) {
  return locations_.GetMutable(index);
}

void DogFamilyInfo::clear_locations() {
  locations_.Clear();
}

const ::proto::RepeatedPtrField<std::string>& DogFamilyInfo::locations() const {
  return locations_;
}

::proto::RepeatedPtrField<std::string>& DogFamilyInfo::mutable_locations() {
  return locations_;
}

}  // namespace CC
}  // namespace BB
}  // namespace AA

