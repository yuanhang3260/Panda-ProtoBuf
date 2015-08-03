#include "test1_pb.h"

namespace AA {

// copy constructor
DogInfo::DogInfo(const DogInfo& other) {
  age_ = other.age();
  name_ = other.name();
  height_ = other.height();
  like_ = other.like();
  luckynumber_ = other.luckynumber();
  sex_ = other.sex();
}

// move constructor
DogInfo::DogInfo(DogInfo&& other) {
  age_ = other.age();
  other.clear_age();
  name_ = std::move(other.mutable_name());
  height_ = other.height();
  other.clear_height();
  like_ = other.like();
  other.clear_like();
  luckynumber_ = std::move(other.mutable_luckynumber());
  sex_ = other.sex();
  other.clear_sex();
}

// copy assignment
DogInfo& DogInfo::operator=(const DogInfo& other) {
  age_ = other.age();
  name_ = other.name();
  height_ = other.height();
  like_ = other.like();
  luckynumber_ = other.luckynumber();
  sex_ = other.sex();
}

// move assignment
DogInfo& DogInfo::operator=(DogInfo&& other) {
  age_ = other.age();
  other.clear_age();
  name_ = std::move(other.mutable_name());
  height_ = other.height();
  other.clear_height();
  like_ = other.like();
  other.clear_like();
  luckynumber_ = std::move(other.mutable_luckynumber());
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

  double height_tmp__ = other->height();
  other->set_height(height_);
  set_height(height_tmp__);

  bool like_tmp__ = other->like();
  other->set_like(like_);
  set_like(like_tmp__);

  std::vector<long long> luckynumber_tmp__ = std::move(other->mutable_luckynumber());
  other->mutable_luckynumber() = std::move(luckynumber_);
  luckynumber_ = std::move(luckynumber_tmp__);

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

// "height" = 3
double DogInfo::height() const {
  return height_;
}

void DogInfo::set_height(double height) {
  height_ = height;
}

void DogInfo::clear_height() {
  height_ = 0;
}

// "like" = 4
bool DogInfo::like() const {
  return like_;
}

void DogInfo::set_like(bool like) {
  like_ = like;
}

void DogInfo::clear_like() {
  like_ = true;
}

// "luckynumber" = 5
int DogInfo::luckynumber_size() const {
  return luckynumber_.size();
}

long long DogInfo::luckynumber(int index) {
  return luckynumber_[index];
}

void DogInfo::set_luckynumber(int index, long long value) {
  if (luckynumber_.size() > index) {
    luckynumber_[index] = value;
  }
}

void DogInfo::add_luckynumber(long long value) {
    luckynumber_.push_back(value);
}

void DogInfo::clear_luckynumber() {
  luckynumber_ .clear();
}

const std::vector<long long> DogInfo::luckynumber() const {
  return luckynumber_;
}

std::vector<long long> DogInfo::mutable_luckynumber() {
  return luckynumber_;
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

namespace BB {
namespace CC {

// copy constructor
DogFriendInfo::DogFriendInfo(const DogFriendInfo& other) {
  name_ = other.name();
  years_ = other.years();
  friend_type_ = other.friend_type();
}

// move constructor
DogFriendInfo::DogFriendInfo(DogFriendInfo&& other) {
  name_ = std::move(other.mutable_name());
  years_ = other.years();
  other.clear_years();
  friend_type_ = other.friend_type();
  other.clear_friend_type();
}

// copy assignment
DogFriendInfo& DogFriendInfo::operator=(const DogFriendInfo& other) {
  name_ = other.name();
  years_ = other.years();
  friend_type_ = other.friend_type();
}

// move assignment
DogFriendInfo& DogFriendInfo::operator=(DogFriendInfo&& other) {
  name_ = std::move(other.mutable_name());
  years_ = other.years();
  other.clear_years();
  friend_type_ = other.friend_type();
  other.clear_friend_type();
}

// swapper
void DogFriendInfo::Swap(DogFriendInfo* other) {
  std::string name_tmp__ = std::move(other->mutable_name());
  other->mutable_name() = std::move(name_);
  name_ = std::move(name_tmp__);

  int years_tmp__ = other->years();
  other->set_years(years_);
  set_years(years_tmp__);

  ::AA::BB::FriendType friend_type_tmp__ = other->friend_type();
  other->set_friend_type(friend_type_);
  set_friend_type(friend_type_tmp__);
}

// destructor
DogFriendInfo::~DogFriendInfo() {
}

// "name" = 1
const std::string& DogFriendInfo::name() const {
  return name_;
}

void DogFriendInfo::set_name(const std::string& name) {
  name_ = name;
}

void DogFriendInfo::set_name(const char* name) {
  name_ = std::string(name);
}

void DogFriendInfo::set_name(const char* name, int size) {
  name_ = std::string(name, size);
}

std::string DogFriendInfo::mutable_name() {
  return name_;
}

void DogFriendInfo::clear_name() {
  name_ = "";
}

// "years" = 2
int DogFriendInfo::years() const {
  return years_;
}

void DogFriendInfo::set_years(int years) {
  years_ = years;
}

void DogFriendInfo::clear_years() {
  years_ = 3;
}

// "friend_type" = 4
::AA::BB::FriendType DogFriendInfo::friend_type() const {
  return friend_type_;
}

void DogFriendInfo::set_friend_type(::AA::BB::FriendType friend_type) {
  friend_type_ = friend_type;
}

void DogFriendInfo::clear_friend_type() {
  friend_type_ = ::AA::BB::Human;
}

}  // namespace CC
}  // namespace BB

// copy constructor
DogFriends::DogFriends(const DogFriends& other) {
  friends_ = other.friends();
  if (!bestDogFriend_) {
    bestDogFriend_ = new DogInfo();
  }
  *bestDogFriend_ = other.bestDogFriend();
}

// move constructor
DogFriends::DogFriends(DogFriends&& other) {
  friends_ = std::move(other.mutable_friends());
  if (bestDogFriend_ ) {
    delete bestDogFriend_;
  }
  bestDogFriend_ = other.release_bestDogFriend();
}

// copy assignment
DogFriends& DogFriends::operator=(const DogFriends& other) {
  friends_ = other.friends();
  if (!bestDogFriend_) {
    bestDogFriend_ = new DogInfo();
  }
  *bestDogFriend_ = other.bestDogFriend();
}

// move assignment
DogFriends& DogFriends::operator=(DogFriends&& other) {
  friends_ = std::move(other.mutable_friends());
  if (bestDogFriend_ ) {
    delete bestDogFriend_;
  }
  bestDogFriend_ = other.release_bestDogFriend();
}

// swapper
void DogFriends::Swap(DogFriends* other) {
  std::vector<BB::CC::DogFriendInfo> friends_tmp__ = std::move(other->mutable_friends());
  other->mutable_friends() = std::move(friends_);
  friends_ = std::move(friends_tmp__);

  DogInfo* bestDogFriend_tmp__ = other->release_bestDogFriend();
  other->set_allocated_bestDogFriend(this->release_bestDogFriend());
  set_allocated_bestDogFriend(bestDogFriend_tmp__);
}

// destructor
DogFriends::~DogFriends() {
  delete bestDogFriend_;
}

// "friends" = 1
int DogFriends::friends_size() const {
  return friends_.size();
}

const BB::CC::DogFriendInfo& DogFriends::friends(int index) {
  return friends_[index];
}

void DogFriends::set_friends(int index, const BB::CC::DogFriendInfo& value) {
  if (index < friends_.size()) {
    friends_[index] = value;
  }
}

void DogFriends::add_friends(const BB::CC::DogFriendInfo& value) {
    friends_.push_back(value);
}

BB::CC::DogFriendInfo& DogFriends::mutable_friends(int index) {
  return friends_[index];
}

void DogFriends::clear_friends() {
  friends_.clear();
}

const std::vector<BB::CC::DogFriendInfo> DogFriends::friends() const {
  return friends_;
}

std::vector<BB::CC::DogFriendInfo> DogFriends::mutable_friends() {
  return friends_;
}

// "bestDogFriend" = 2
const DogInfo& DogFriends::bestDogFriend() const {
  return *bestDogFriend_;
}

DogInfo* DogFriends::mutable_bestDogFriend() {
  return bestDogFriend_;
}

void DogFriends::set_allocated_bestDogFriend(DogInfo* bestDogFriend) {
  bestDogFriend_ = bestDogFriend;
}

DogInfo* DogFriends::release_bestDogFriend() {
  DogInfo* bestDogFriend_tmp__ = bestDogFriend_;
  bestDogFriend_ = nullptr;
  return bestDogFriend_tmp__;
}

void DogFriends::clear_bestDogFriend() {
  if (bestDogFriend_) {
    delete bestDogFriend_;
  }
  bestDogFriend_ = nullptr;
}

namespace DD {

// copy constructor
DogFamily::DogFamily(const DogFamily& other) {
  dogs_ = other.dogs();
}

// move constructor
DogFamily::DogFamily(DogFamily&& other) {
  dogs_ = std::move(other.mutable_dogs());
}

// copy assignment
DogFamily& DogFamily::operator=(const DogFamily& other) {
  dogs_ = other.dogs();
}

// move assignment
DogFamily& DogFamily::operator=(DogFamily&& other) {
  dogs_ = std::move(other.mutable_dogs());
}

// swapper
void DogFamily::Swap(DogFamily* other) {
  std::vector<::AA::DogInfo> dogs_tmp__ = std::move(other->mutable_dogs());
  other->mutable_dogs() = std::move(dogs_);
  dogs_ = std::move(dogs_tmp__);
}

// destructor
DogFamily::~DogFamily() {
}

// "dogs" = 1
int DogFamily::dogs_size() const {
  return dogs_.size();
}

const ::AA::DogInfo& DogFamily::dogs(int index) {
  return dogs_[index];
}

void DogFamily::set_dogs(int index, const ::AA::DogInfo& value) {
  if (index < dogs_.size()) {
    dogs_[index] = value;
  }
}

void DogFamily::add_dogs(const ::AA::DogInfo& value) {
    dogs_.push_back(value);
}

::AA::DogInfo& DogFamily::mutable_dogs(int index) {
  return dogs_[index];
}

void DogFamily::clear_dogs() {
  dogs_.clear();
}

const std::vector<::AA::DogInfo> DogFamily::dogs() const {
  return dogs_;
}

std::vector<::AA::DogInfo> DogFamily::mutable_dogs() {
  return dogs_;
}

}  // namespace DD
}  // namespace AA

