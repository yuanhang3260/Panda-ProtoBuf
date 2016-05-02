#include <memory>
#include <mutex>
#include <map>

#include "Proto/Descriptor.h"
#include "Proto/DescriptorsBuilder.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"

#include "test1_pb.h"

namespace {

const ::proto::MessageDescriptor* DogInfo_descriptor_ = nullptr;
const ::proto::MessageReflection* DogInfo_reflection_ = nullptr;
const ::proto::MessageDescriptor* DogFriendInfo_descriptor_ = nullptr;
const ::proto::MessageReflection* DogFriendInfo_reflection_ = nullptr;
const ::proto::MessageDescriptor* DogFriends_descriptor_ = nullptr;
const ::proto::MessageReflection* DogFriends_reflection_ = nullptr;
const ::proto::MessageDescriptor* DogFamily_descriptor_ = nullptr;
const ::proto::MessageReflection* DogFamily_reflection_ = nullptr;

}  // namepsace

void static_init_default_instances_home_hy_Desktop_Projects_ProtoBuf_samples_test1() {
  if (AA::DogInfo::default_instance_ == nullptr) {
    AA::DogInfo::default_instance_ = new AA::DogInfo();
    AA::DogInfo::default_instance_->InitAsDefaultInstance();
  }
  if (AA::BB::CC::DogFriendInfo::default_instance_ == nullptr) {
    AA::BB::CC::DogFriendInfo::default_instance_ = new AA::BB::CC::DogFriendInfo();
    AA::BB::CC::DogFriendInfo::default_instance_->InitAsDefaultInstance();
  }
  if (AA::DogFriends::default_instance_ == nullptr) {
    AA::DogFriends::default_instance_ = new AA::DogFriends();
    AA::DogFriends::default_instance_->InitAsDefaultInstance();
  }
  if (AA::DD::DogFamily::default_instance_ == nullptr) {
    AA::DD::DogFamily::default_instance_ = new AA::DD::DogFamily();
    AA::DD::DogFamily::default_instance_->InitAsDefaultInstance();
  }
}

void static_init_home_hy_Desktop_Projects_ProtoBuf_samples_test1() {
  static bool already_called = false;
  if (already_called) return;
  already_called = true;

  ::proto::DescriptorsBuilder descriptors_builder(
      "/home/hy/Desktop/Projects/ProtoBuf/samples/test1.proto");
  auto file_dscpt = descriptors_builder.BuildDescriptors();
  CHECK(file_dscpt != nullptr, "static class initialization for "
        "/home/hy/Desktop/Projects/ProtoBuf/samples/test1.proto failed");
  ::proto::MessageFactory::RegisterParsedProtoFile(file_dscpt);

  static_init_default_instances_home_hy_Desktop_Projects_ProtoBuf_samples_test1();

  // static init for class DogInfo
  static const int DogInfo_offsets_[6] = {
    PROTO_MESSAGE_FIELD_OFFSET(AA::DogInfo, age_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::DogInfo, name_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::DogInfo, height_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::DogInfo, like_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::DogInfo, luckynumber_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::DogInfo, sex_),
  };
  DogInfo_descriptor_ = file_dscpt->FindMessageTypeByName("AA.DogInfo");
  CHECK(DogInfo_descriptor_ != nullptr, 
        "Can't find message descriptor for AA.DogInfo");
  DogInfo_reflection_ = 
      new ::proto::MessageReflection(
          DogInfo_descriptor_,
          AA::DogInfo::default_instance_,
          DogInfo_offsets_,
          PROTO_MESSAGE_FIELD_OFFSET(AA::DogInfo, has_bits_));
  ::proto::MessageFactory::RegisterGeneratedMessage(DogInfo_reflection_);

  // static init for class DogFriendInfo
  static const int DogFriendInfo_offsets_[3] = {
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::CC::DogFriendInfo, name_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::CC::DogFriendInfo, years_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::BB::CC::DogFriendInfo, friend_type_),
  };
  DogFriendInfo_descriptor_ = file_dscpt->FindMessageTypeByName("AA.BB.CC.DogFriendInfo");
  CHECK(DogFriendInfo_descriptor_ != nullptr, 
        "Can't find message descriptor for AA.BB.CC.DogFriendInfo");
  DogFriendInfo_reflection_ = 
      new ::proto::MessageReflection(
          DogFriendInfo_descriptor_,
          AA::BB::CC::DogFriendInfo::default_instance_,
          DogFriendInfo_offsets_,
          PROTO_MESSAGE_FIELD_OFFSET(AA::BB::CC::DogFriendInfo, has_bits_));
  ::proto::MessageFactory::RegisterGeneratedMessage(DogFriendInfo_reflection_);

  // static init for class DogFriends
  static const int DogFriends_offsets_[2] = {
    PROTO_MESSAGE_FIELD_OFFSET(AA::DogFriends, friends_),
    PROTO_MESSAGE_FIELD_OFFSET(AA::DogFriends, bestDogFriend_),
  };
  DogFriends_descriptor_ = file_dscpt->FindMessageTypeByName("AA.DogFriends");
  CHECK(DogFriends_descriptor_ != nullptr, 
        "Can't find message descriptor for AA.DogFriends");
  DogFriends_reflection_ = 
      new ::proto::MessageReflection(
          DogFriends_descriptor_,
          AA::DogFriends::default_instance_,
          DogFriends_offsets_,
          PROTO_MESSAGE_FIELD_OFFSET(AA::DogFriends, has_bits_));
  ::proto::MessageFactory::RegisterGeneratedMessage(DogFriends_reflection_);

  // static init for class DogFamily
  static const int DogFamily_offsets_[1] = {
    PROTO_MESSAGE_FIELD_OFFSET(AA::DD::DogFamily, dogs_),
  };
  DogFamily_descriptor_ = file_dscpt->FindMessageTypeByName("AA.DD.DogFamily");
  CHECK(DogFamily_descriptor_ != nullptr, 
        "Can't find message descriptor for AA.DD.DogFamily");
  DogFamily_reflection_ = 
      new ::proto::MessageReflection(
          DogFamily_descriptor_,
          AA::DD::DogFamily::default_instance_,
          DogFamily_offsets_,
          PROTO_MESSAGE_FIELD_OFFSET(AA::DD::DogFamily, has_bits_));
  ::proto::MessageFactory::RegisterGeneratedMessage(DogFamily_reflection_);

}

// Force static_init_home_hy_Desktop_Projects_ProtoBuf_samples_test1() to be called at initialization time.
struct static_init_forcer_home_hy_Desktop_Projects_ProtoBuf_samples_test1 {
  static_init_forcer_home_hy_Desktop_Projects_ProtoBuf_samples_test1() {
    static_init_home_hy_Desktop_Projects_ProtoBuf_samples_test1();
  }
} static_init_forcer_home_hy_Desktop_Projects_ProtoBuf_samples_test1_obj_;


namespace AA {

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
  name_ = other.name();
  height_ = other.height();
  like_ = other.like();
  luckynumber_ = other.luckynumber();
  sex_ = other.sex();
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
  name_ = std::move(other.mutable_name());
  height_ = other.height();
  like_ = other.like();
  luckynumber_ = std::move(other.mutable_luckynumber());
  sex_ = other.sex();
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
  if (name_ != other.name_) {
    return false;
  }
  if (height_ != other.height_) {
    return false;
  }
  if (like_ != other.like_) {
    return false;
  }
  for (unsigned int i = 0; i < luckynumber_.size(); i++) {
    if (luckynumber_.at(i) != other.luckynumber_.at(i)) {
      return false;
    }
  }
  if (sex_ != other.sex_) {
    return false;
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
  if (has_name()) {
    PrintIndent(indent_num + 1);
    std::cout << "name: " << "\"" << name_ << "\"" << std::endl;
  }
  if (has_height()) {
    PrintIndent(indent_num + 1);
    std::cout << "height: " << height_ << std::endl;
  }
  if (has_like()) {
    PrintIndent(indent_num + 1);
    std::cout << "like: " << like_ << std::endl;
  }
  if (luckynumber_size() > 0) {
    PrintIndent(indent_num + 1);
    std::cout << "luckynumber: " << "[";
    for (const auto& ele: luckynumber_) {
        std::cout << ele << ", ";
    }
    std::cout << "]" << std::endl;
  }
  if (has_sex()) {
    PrintIndent(indent_num + 1);
    std::string enum_value =
        (reinterpret_cast<const proto::EnumDescriptor*>(
            DogInfo_descriptor_->FindFieldByName("sex")->type_descriptor()))
                 ->EnumValueAsString(sex_);
    std::cout << "sex: " << enum_value << std::endl;
  }
  PrintIndent(indent_num);
  std::cout << "}" << std::endl;
}

// InitAsDefaultInstance()
void DogInfo::InitAsDefaultInstance() {
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

  std::string name_tmp__ = std::move(other->mutable_name());
  other->mutable_name() = std::move(name_);
  name_ = std::move(name_tmp__);

  double height_tmp__ = other->height();
  other->set_height(height_);
  set_height(height_tmp__);

  bool like_tmp__ = other->like();
  other->set_like(like_);
  set_like(like_tmp__);

  ::proto::RepeatedField<long long> luckynumber_tmp__ = std::move(other->mutable_luckynumber());
  other->mutable_luckynumber() = std::move(luckynumber_);
  luckynumber_ = std::move(luckynumber_tmp__);

  DogInfo::Sex sex_tmp__ = other->sex();
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
const DogInfo& DogInfo::default_instance() {
  if (default_instance_ == nullptr) {
    static_init_default_instances_home_hy_Desktop_Projects_ProtoBuf_samples_test1();
  }
  return *default_instance_;
}

DogInfo* DogInfo::default_instance_ = nullptr;

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

// "height" = 3
bool DogInfo::has_height() const {
  return (has_bits_[0] & 0x8) != 0;
}

double DogInfo::height() const {
  return height_;
}

void DogInfo::set_height(double height) {
  height_ = height;
  has_bits_[0] |= 0x8;
}

void DogInfo::clear_height() {
  height_ = 0;
  has_bits_[0] &= (~0x8);
}

// "like" = 4
bool DogInfo::has_like() const {
  return (has_bits_[0] & 0x10) != 0;
}

bool DogInfo::like() const {
  return like_;
}

void DogInfo::set_like(bool like) {
  like_ = like;
  has_bits_[0] |= 0x10;
}

void DogInfo::clear_like() {
  like_ = true;
  has_bits_[0] &= (~0x10);
}

// "luckynumber" = 5
int DogInfo::luckynumber_size() const {
  return luckynumber_.size();
}

long long DogInfo::luckynumber(int index) const {
  return luckynumber_.Get(index);
}

void DogInfo::set_luckynumber(int index, long long value) {
  if ((int)luckynumber_.size() > index) {
    luckynumber_.Set(index, value);
  }
}

void DogInfo::add_luckynumber(long long value) {
   luckynumber_.Add(value);
}

void DogInfo::clear_luckynumber() {
  luckynumber_ .Clear();
}

const ::proto::RepeatedField<long long>& DogInfo::luckynumber() const {
  return luckynumber_;
}

::proto::RepeatedField<long long>& DogInfo::mutable_luckynumber() {
  return luckynumber_;
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

namespace BB {
namespace CC {

// ******************** DogFriendInfo ******************** //
// constructor
DogFriendInfo::DogFriendInfo() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
DogFriendInfo::DogFriendInfo(const DogFriendInfo& other) {
  CopyFrom(other);
}

// move constructor
DogFriendInfo::DogFriendInfo(DogFriendInfo&& other) {
  MoveFrom(std::move(other));
}

// copy assignment
DogFriendInfo& DogFriendInfo::operator=(const DogFriendInfo& other) {
  CopyFrom(other);
  return *this;
}
// move assignment
DogFriendInfo& DogFriendInfo::operator=(DogFriendInfo&& other) {
  MoveFrom(std::move(other));
  return *this;
}

// New()
::proto::Message* DogFriendInfo::New() const {
  return reinterpret_cast<::proto::Message*>(new DogFriendInfo());
}

// CopyFrom()
void DogFriendInfo::CopyFrom(const DogFriendInfo& other) {
  name_ = other.name();
  years_ = other.years();
  friend_type_ = other.friend_type();
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
}

// MoveFrom()
void DogFriendInfo::MoveFrom(DogFriendInfo&& other) {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
  name_ = std::move(other.mutable_name());
  years_ = other.years();
  friend_type_ = other.friend_type();
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    other.has_bits_[i] = 0;
  }
}

// Equals()
bool DogFriendInfo::Equals(const DogFriendInfo& other) const {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    if (has_bits_[i] != other.has_bits_[i]) {
      return false;
    }
  }
  if (name_ != other.name_) {
    return false;
  }
  if (years_ != other.years_) {
    return false;
  }
  if (friend_type_ != other.friend_type_) {
    return false;
  }
  return true;
}

// Serialize()
::proto::SerializedMessage* DogFriendInfo::Serialize() const {
  return DogFriendInfo_reflection_->Serialize(this);
}

// DeSerialize()
void DogFriendInfo::DeSerialize(const char* buf, unsigned int size) {
  DogFriendInfo_reflection_->DeSerialize(this, buf, size);
}

// Print()
void DogFriendInfo::Print(int indent_num) const {
  PrintIndent(indent_num);
  std::cout << "DogFriendInfo " << "{" << std::endl;
  if (has_name()) {
    PrintIndent(indent_num + 1);
    std::cout << "name: " << "\"" << name_ << "\"" << std::endl;
  }
  if (has_years()) {
    PrintIndent(indent_num + 1);
    std::cout << "years: " << years_ << std::endl;
  }
  if (has_friend_type()) {
    PrintIndent(indent_num + 1);
    std::string enum_value =
        (reinterpret_cast<const proto::EnumDescriptor*>(
            DogFriendInfo_descriptor_->FindFieldByName("friend_type")->type_descriptor()))
                 ->EnumValueAsString(friend_type_);
    std::cout << "friend_type: " << enum_value << std::endl;
  }
  PrintIndent(indent_num);
  std::cout << "}" << std::endl;
}

// InitAsDefaultInstance()
void DogFriendInfo::InitAsDefaultInstance() {
}

// swapper
void DogFriendInfo::Swap(DogFriendInfo* other) {
  // store has_bits
  char* buf = new char[2 * sizeof(has_bits_)];
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    buf[i] = has_bits_[i];
    buf[i + sizeof(has_bits_)] = other->has_bits_[i];
  }

  std::string name_tmp__ = std::move(other->mutable_name());
  other->mutable_name() = std::move(name_);
  name_ = std::move(name_tmp__);

  int years_tmp__ = other->years();
  other->set_years(years_);
  set_years(years_tmp__);

  ::AA::BB::FriendType friend_type_tmp__ = other->friend_type();
  other->set_friend_type(friend_type_);
  set_friend_type(friend_type_tmp__);

  // swap has_bits
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = buf[i + sizeof(has_bits_)];
    other->has_bits_[i] = buf[i];
  }
  delete buf;
}

// default_instance()
const DogFriendInfo& DogFriendInfo::default_instance() {
  if (default_instance_ == nullptr) {
    static_init_default_instances_home_hy_Desktop_Projects_ProtoBuf_samples_test1();
  }
  return *default_instance_;
}

DogFriendInfo* DogFriendInfo::default_instance_ = nullptr;

// destructor
DogFriendInfo::~DogFriendInfo() {
}

// "name" = 1
bool DogFriendInfo::has_name() const {
  return (has_bits_[0] & 0x2) != 0;
}

const std::string& DogFriendInfo::name() const {
  return name_;
}

void DogFriendInfo::set_name(const std::string& name) {
  name_ = name;
  has_bits_[0] |= 0x2;
}

void DogFriendInfo::set_name(const char* name) {
  name_ = std::string(name);
  has_bits_[0] |= 0x2;
}

void DogFriendInfo::set_name(const char* name, int size) {
  name_ = std::string(name, size);
  has_bits_[0] |= 0x2;
}

std::string DogFriendInfo::mutable_name() {
  return name_;
}

void DogFriendInfo::clear_name() {
  name_ = "";
  has_bits_[0] &= (~0x2);
}

// "years" = 2
bool DogFriendInfo::has_years() const {
  return (has_bits_[0] & 0x4) != 0;
}

int DogFriendInfo::years() const {
  return years_;
}

void DogFriendInfo::set_years(int years) {
  years_ = years;
  has_bits_[0] |= 0x4;
}

void DogFriendInfo::clear_years() {
  years_ = 3;
  has_bits_[0] &= (~0x4);
}

// "friend_type" = 4
bool DogFriendInfo::has_friend_type() const {
  return (has_bits_[0] & 0x10) != 0;
}

::AA::BB::FriendType DogFriendInfo::friend_type() const {
  return friend_type_;
}

void DogFriendInfo::set_friend_type(::AA::BB::FriendType friend_type) {
  friend_type_ = friend_type;
  has_bits_[0] |= 0x10;
}

void DogFriendInfo::clear_friend_type() {
  friend_type_ = ::AA::BB::Human;
  has_bits_[0] &= (~0x10);
}

}  // namespace CC
}  // namespace BB

// ******************** DogFriends ******************** //
// constructor
DogFriends::DogFriends() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
DogFriends::DogFriends(const DogFriends& other) {
  CopyFrom(other);
}

// move constructor
DogFriends::DogFriends(DogFriends&& other) {
  MoveFrom(std::move(other));
}

// copy assignment
DogFriends& DogFriends::operator=(const DogFriends& other) {
  CopyFrom(other);
  return *this;
}
// move assignment
DogFriends& DogFriends::operator=(DogFriends&& other) {
  MoveFrom(std::move(other));
  return *this;
}

// New()
::proto::Message* DogFriends::New() const {
  return reinterpret_cast<::proto::Message*>(new DogFriends());
}

// CopyFrom()
void DogFriends::CopyFrom(const DogFriends& other) {
  for (const BB::CC::DogFriendInfo* p: other.friends().GetElements()) {
    friends_.AddAllocated(new BB::CC::DogFriendInfo(*p));
  }
  if (other.bestDogFriend_) {
    if (!bestDogFriend_) {
      bestDogFriend_ = new DogInfo();
    }
    *bestDogFriend_ = other.bestDogFriend();
  }
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
}

// MoveFrom()
void DogFriends::MoveFrom(DogFriends&& other) {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
  friends_ = std::move(other.mutable_friends());
  if (other.bestDogFriend_) {
    if (bestDogFriend_) {
      delete bestDogFriend_;
    }
    bestDogFriend_ = other.bestDogFriend_;
    other.bestDogFriend_ = nullptr;
  }
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    other.has_bits_[i] = 0;
  }
}

// Equals()
bool DogFriends::Equals(const DogFriends& other) const {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    if (has_bits_[i] != other.has_bits_[i]) {
      return false;
    }
  }
  for (unsigned int i = 0; i < friends_.size(); i++) {
    if (!friends_.at(i).Equals(other.friends_.at(i))) {
      return false;
    }
  }
  if (bestDogFriend_ && other.bestDogFriend_ &&
      !bestDogFriend_->Equals(*other.bestDogFriend_)) {
    return false;
  }
  return true;
}

// Serialize()
::proto::SerializedMessage* DogFriends::Serialize() const {
  return DogFriends_reflection_->Serialize(this);
}

// DeSerialize()
void DogFriends::DeSerialize(const char* buf, unsigned int size) {
  DogFriends_reflection_->DeSerialize(this, buf, size);
}

// Print()
void DogFriends::Print(int indent_num) const {
  PrintIndent(indent_num);
  std::cout << "DogFriends " << "{" << std::endl;
  if (friends_size() > 0) {
    PrintIndent(indent_num + 1);
    std::cout << "friends: " << "[***]" << std::endl;
    for (const auto& ele: friends_) {
        ele.Print(indent_num + 1);
    }
  }
  if (has_bestDogFriend()) {
    PrintIndent(indent_num + 1);
    std::cout << "bestDogFriend: " << "*" << std::endl;
    bestDogFriend_->Print(indent_num + 1);
  }
  PrintIndent(indent_num);
  std::cout << "}" << std::endl;
}

// InitAsDefaultInstance()
void DogFriends::InitAsDefaultInstance() {
  bestDogFriend_ = const_cast<DogInfo*>(&DogInfo::default_instance());
}

// swapper
void DogFriends::Swap(DogFriends* other) {
  // store has_bits
  char* buf = new char[2 * sizeof(has_bits_)];
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    buf[i] = has_bits_[i];
    buf[i + sizeof(has_bits_)] = other->has_bits_[i];
  }

  ::proto::RepeatedPtrField<BB::CC::DogFriendInfo> friends_tmp__ = std::move(other->mutable_friends());
  other->mutable_friends() = std::move(friends_);
  friends_ = std::move(friends_tmp__);

  DogInfo* bestDogFriend_tmp__ = other->release_bestDogFriend();
  other->set_allocated_bestDogFriend(this->release_bestDogFriend());
  set_allocated_bestDogFriend(bestDogFriend_tmp__);

  // swap has_bits
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = buf[i + sizeof(has_bits_)];
    other->has_bits_[i] = buf[i];
  }
  delete buf;
}

// default_instance()
const DogFriends& DogFriends::default_instance() {
  if (default_instance_ == nullptr) {
    static_init_default_instances_home_hy_Desktop_Projects_ProtoBuf_samples_test1();
  }
  return *default_instance_;
}

DogFriends* DogFriends::default_instance_ = nullptr;

// destructor
DogFriends::~DogFriends() {
  if (bestDogFriend_) {
    delete bestDogFriend_;
  }
}

// "friends" = 1
int DogFriends::friends_size() const {
  return friends_.size();
}

const BB::CC::DogFriendInfo& DogFriends::friends(int index) const {
  return friends_.Get(index);
}

BB::CC::DogFriendInfo* DogFriends::add_friends() {
  return friends_.Add();
}

BB::CC::DogFriendInfo* DogFriends::mutable_friends(int index) {
  return friends_.GetMutable(index);
}

void DogFriends::clear_friends() {
  friends_.Clear();
}

const ::proto::RepeatedPtrField<BB::CC::DogFriendInfo>& DogFriends::friends() const {
  return friends_;
}

::proto::RepeatedPtrField<BB::CC::DogFriendInfo>& DogFriends::mutable_friends() {
  return friends_;
}

// "bestDogFriend" = 2
bool DogFriends::has_bestDogFriend() const {
  return (has_bits_[0] & 0x4) != 0;
}

const DogInfo& DogFriends::bestDogFriend() const {
  if (has_bestDogFriend() && bestDogFriend_) {
    return *bestDogFriend_;
  }
  else {
    return DogInfo::default_instance();
  }
}

DogInfo* DogFriends::mutable_bestDogFriend() {
  if (has_bestDogFriend() && bestDogFriend_) {
    return bestDogFriend_;
  }
  else {
    bestDogFriend_ = new DogInfo();
    has_bits_[0] |= 0x4;
    return bestDogFriend_;
  }
}

void DogFriends::set_allocated_bestDogFriend(DogInfo* bestDogFriend) {
  if (bestDogFriend_) {
    delete bestDogFriend_;
  }
  bestDogFriend_ = bestDogFriend;
  if (bestDogFriend_) {
    has_bits_[0] |= 0x4;
  }
  else {
    has_bits_[0] &= (~0x4);
  }
}

DogInfo* DogFriends::release_bestDogFriend() {
  DogInfo* bestDogFriend_tmp__ = bestDogFriend_;
  bestDogFriend_ = nullptr;
  has_bits_[0] &= (~0x4);
  return bestDogFriend_tmp__;
}

void DogFriends::clear_bestDogFriend() {
  if (bestDogFriend_) {
    delete bestDogFriend_;
  }
  bestDogFriend_ = nullptr;
  has_bits_[0] &= (~0x4);
}

namespace DD {

// ******************** DogFamily ******************** //
// constructor
DogFamily::DogFamily() {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = 0;
  }
  default_instance_ = nullptr;
}

// copy constructor
DogFamily::DogFamily(const DogFamily& other) {
  CopyFrom(other);
}

// move constructor
DogFamily::DogFamily(DogFamily&& other) {
  MoveFrom(std::move(other));
}

// copy assignment
DogFamily& DogFamily::operator=(const DogFamily& other) {
  CopyFrom(other);
  return *this;
}
// move assignment
DogFamily& DogFamily::operator=(DogFamily&& other) {
  MoveFrom(std::move(other));
  return *this;
}

// New()
::proto::Message* DogFamily::New() const {
  return reinterpret_cast<::proto::Message*>(new DogFamily());
}

// CopyFrom()
void DogFamily::CopyFrom(const DogFamily& other) {
  for (const ::AA::DogInfo* p: other.dogs().GetElements()) {
    dogs_.AddAllocated(new ::AA::DogInfo(*p));
  }
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
}

// MoveFrom()
void DogFamily::MoveFrom(DogFamily&& other) {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = other.has_bits_[i];
  }
  dogs_ = std::move(other.mutable_dogs());
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    other.has_bits_[i] = 0;
  }
}

// Equals()
bool DogFamily::Equals(const DogFamily& other) const {
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    if (has_bits_[i] != other.has_bits_[i]) {
      return false;
    }
  }
  for (unsigned int i = 0; i < dogs_.size(); i++) {
    if (!dogs_.at(i).Equals(other.dogs_.at(i))) {
      return false;
    }
  }
  return true;
}

// Serialize()
::proto::SerializedMessage* DogFamily::Serialize() const {
  return DogFamily_reflection_->Serialize(this);
}

// DeSerialize()
void DogFamily::DeSerialize(const char* buf, unsigned int size) {
  DogFamily_reflection_->DeSerialize(this, buf, size);
}

// Print()
void DogFamily::Print(int indent_num) const {
  PrintIndent(indent_num);
  std::cout << "DogFamily " << "{" << std::endl;
  if (dogs_size() > 0) {
    PrintIndent(indent_num + 1);
    std::cout << "dogs: " << "[***]" << std::endl;
    for (const auto& ele: dogs_) {
        ele.Print(indent_num + 1);
    }
  }
  PrintIndent(indent_num);
  std::cout << "}" << std::endl;
}

// InitAsDefaultInstance()
void DogFamily::InitAsDefaultInstance() {
}

// swapper
void DogFamily::Swap(DogFamily* other) {
  // store has_bits
  char* buf = new char[2 * sizeof(has_bits_)];
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    buf[i] = has_bits_[i];
    buf[i + sizeof(has_bits_)] = other->has_bits_[i];
  }

  ::proto::RepeatedPtrField<::AA::DogInfo> dogs_tmp__ = std::move(other->mutable_dogs());
  other->mutable_dogs() = std::move(dogs_);
  dogs_ = std::move(dogs_tmp__);

  // swap has_bits
  for (unsigned int i = 0; i < sizeof(has_bits_); i++) {
    has_bits_[i] = buf[i + sizeof(has_bits_)];
    other->has_bits_[i] = buf[i];
  }
  delete buf;
}

// default_instance()
const DogFamily& DogFamily::default_instance() {
  if (default_instance_ == nullptr) {
    static_init_default_instances_home_hy_Desktop_Projects_ProtoBuf_samples_test1();
  }
  return *default_instance_;
}

DogFamily* DogFamily::default_instance_ = nullptr;

// destructor
DogFamily::~DogFamily() {
}

// "dogs" = 1
int DogFamily::dogs_size() const {
  return dogs_.size();
}

const ::AA::DogInfo& DogFamily::dogs(int index) const {
  return dogs_.Get(index);
}

::AA::DogInfo* DogFamily::add_dogs() {
  return dogs_.Add();
}

::AA::DogInfo* DogFamily::mutable_dogs(int index) {
  return dogs_.GetMutable(index);
}

void DogFamily::clear_dogs() {
  dogs_.Clear();
}

const ::proto::RepeatedPtrField<::AA::DogInfo>& DogFamily::dogs() const {
  return dogs_;
}

::proto::RepeatedPtrField<::AA::DogInfo>& DogFamily::mutable_dogs() {
  return dogs_;
}

}  // namespace DD
}  // namespace AA

