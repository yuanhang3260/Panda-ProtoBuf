#include "simple_pb.h"

namespace AA {

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
}

// swapper
void DogInfo::Swap(DogInfo* other) {
  int age_tmp__ = other->age();
  other->set_age(age_);
  set_age(age_tmp__);

  std::string name_tmp__ = std::move(other->mutable_name());
  other->mutable_name() = std::move(name_);
  name_ = std::move(name_tmp__);

  BB::FriendType friend_type_tmp__ = other->friend_type();
  other->set_friend_type(friend_type_);
  set_friend_type(friend_type_tmp__);

  DogInfo::Sex sex_tmp__ = other->sex();
  other->set_sex(sex_);
  set_sex(sex_tmp__);
}

// destructor
DogInfo::~DogInfo() {
}

// "age" = 1
int DogInfo::age() const {
  return age_;
}

void DogInfo::set_age(int age) {
  age_ = age;
}

void DogInfo::clear_age() {
  age_ = 0;
}

// "name" = 2
const std::string& DogInfo::name() const {
  return name_;
}

void DogInfo::set_name(const std::string& name) {
  name_ = name;
}

void DogInfo::set_name(const char* name) {
  name_ = std::string(name);
}

void DogInfo::set_name(const char* name, int size) {
  name_ = std::string(name, size);
}

std::string DogInfo::mutable_name() {
  return name_;
}

void DogInfo::clear_name() {
  name_ = "";
}

// "friend_type" = 4
BB::FriendType DogInfo::friend_type() const {
  return friend_type_;
}

void DogInfo::set_friend_type(BB::FriendType friend_type) {
  friend_type_ = friend_type;
}

void DogInfo::clear_friend_type() {
  friend_type_ = BB::Human;
}

// "sex" = 6
DogInfo::Sex DogInfo::sex() const {
  return sex_;
}

void DogInfo::set_sex(DogInfo::Sex sex) {
  sex_ = sex;
}

void DogInfo::clear_sex() {
  sex_ = DogInfo::Male;
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
}

// move assignment
FamilyInfo& FamilyInfo::operator=(FamilyInfo&& other) {
  address_ = std::move(other.mutable_address());
  numberdogs_ = other.numberdogs();
  other.clear_numberdogs();
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

// destructor
FamilyInfo::~FamilyInfo() {
}

// "address" = 1
const std::string& FamilyInfo::address() const {
  return address_;
}

void FamilyInfo::set_address(const std::string& address) {
  address_ = address;
}

void FamilyInfo::set_address(const char* address) {
  address_ = std::string(address);
}

void FamilyInfo::set_address(const char* address, int size) {
  address_ = std::string(address, size);
}

std::string FamilyInfo::mutable_address() {
  return address_;
}

void FamilyInfo::clear_address() {
  address_ = "";
}

// "numberdogs" = 2
int FamilyInfo::numberdogs() const {
  return numberdogs_;
}

void FamilyInfo::set_numberdogs(int numberdogs) {
  numberdogs_ = numberdogs;
}

void FamilyInfo::clear_numberdogs() {
  numberdogs_ = 0;
}

namespace BB {
namespace CC {

// copy constructor
DogFamilyInfo::DogFamilyInfo(const DogFamilyInfo& other) {
  if (!family_) {
    family_ = new AA::FamilyInfo();
  }
  *family_ = other.family();
  utility_fee_ = other.utility_fee();
  dogs_ = other.dogs();
  locations_ = other.locations();
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
    family_ = new AA::FamilyInfo();
  }
  *family_ = other.family();
  utility_fee_ = other.utility_fee();
  dogs_ = other.dogs();
  locations_ = other.locations();
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
}

// swapper
void DogFamilyInfo::Swap(DogFamilyInfo* other) {
  AA::FamilyInfo* family_tmp__ = other->release_family();
  other->set_allocated_family(this->release_family());
  set_allocated_family(family_tmp__);

  std::vector<double> utility_fee_tmp__ = std::move(other->mutable_utility_fee());
  other->mutable_utility_fee() = std::move(utility_fee_);
  utility_fee_ = std::move(utility_fee_tmp__);

  std::vector<AA::DogInfo> dogs_tmp__ = std::move(other->mutable_dogs());
  other->mutable_dogs() = std::move(dogs_);
  dogs_ = std::move(dogs_tmp__);

  std::vector<std::string> locations_tmp__ = std::move(other->mutable_locations());
  other->mutable_locations() = std::move(locations_);
  locations_ = std::move(locations_tmp__);
}

// destructor
DogFamilyInfo::~DogFamilyInfo() {
  delete family_;
}

// "family" = 1
const AA::FamilyInfo& DogFamilyInfo::family() const {
  return *family_;
}

AA::FamilyInfo* DogFamilyInfo::mutable_family() {
  return family_;
}

void DogFamilyInfo::set_allocated_family(AA::FamilyInfo* family) {
  family_ = family;
}

AA::FamilyInfo* DogFamilyInfo::release_family() {
  AA::FamilyInfo* family_tmp__ = family_;
  family_ = nullptr;
  return family_tmp__;
}

void DogFamilyInfo::clear_family() {
  if (family_) {
    delete family_;
  }
  family_ = nullptr;
}

// "utility_fee" = 2
int DogFamilyInfo::utility_fee_size() const {
  return utility_fee_.size();
}

double DogFamilyInfo::utility_fee(int index) {
  return utility_fee_[index];
}

void DogFamilyInfo::set_utility_fee(int index, double value) {
  if (utility_fee_.size() > index) {
    utility_fee_[index] = value;
  }
}

void DogFamilyInfo::add_utility_fee(double value) {
    utility_fee_.push_back(value);
}

void DogFamilyInfo::clear_utility_fee() {
  utility_fee_ .clear();
}

const std::vector<double> DogFamilyInfo::utility_fee() const {
  return utility_fee_;
}

std::vector<double> DogFamilyInfo::mutable_utility_fee() {
  return utility_fee_;
}

// "dogs" = 3
int DogFamilyInfo::dogs_size() const {
  return dogs_.size();
}

const AA::DogInfo& DogFamilyInfo::dogs(int index) {
  return dogs_[index];
}

void DogFamilyInfo::set_dogs(int index, const AA::DogInfo& value) {
  if (index < dogs_.size()) {
    dogs_[index] = value;
  }
}

void DogFamilyInfo::add_dogs(const AA::DogInfo& value) {
    dogs_.push_back(value);
}

AA::DogInfo& DogFamilyInfo::mutable_dogs(int index) {
  return dogs_[index];
}

void DogFamilyInfo::clear_dogs() {
  dogs_.clear();
}

const std::vector<AA::DogInfo> DogFamilyInfo::dogs() const {
  return dogs_;
}

std::vector<AA::DogInfo> DogFamilyInfo::mutable_dogs() {
  return dogs_;
}

// "locations" = 4
int DogFamilyInfo::locations_size() const {
  return locations_.size();
}

const std::string& DogFamilyInfo::locations(int index) {
  return locations_[index];
}

void DogFamilyInfo::set_locations(int index, const std::string& value) {
  if (index < locations_.size()) {
    locations_[index] = value;
  }
}

void DogFamilyInfo::set_locations(int index, const char* value) {
  if (index < locations_.size()) {
    locations_[index] = std::string(value);
  }
}

void DogFamilyInfo::set_locations(int index, const char* value, int size) {
  if (index < locations_.size()) {
    locations_[index] = std::string(value, size);
  }
}

void DogFamilyInfo::add_locations(const std::string& value) {
    locations_.push_back(value);
}

void DogFamilyInfo::add_locations(const char* value) {
    locations_.push_back(std::string(value));
}

void DogFamilyInfo::add_locations(const char* value, int size) {
    locations_.push_back(std::string(value, size));
}

std::string& DogFamilyInfo::mutable_locations(int index) {
  return locations_[index];
}

void DogFamilyInfo::clear_locations() {
  locations_.clear();
}

const std::vector<std::string> DogFamilyInfo::locations() const {
  return locations_;
}

std::vector<std::string> DogFamilyInfo::mutable_locations() {
  return locations_;
}

}  // namespace CC
}  // namespace BB
}  // namespace AA

