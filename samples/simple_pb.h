#ifndef SIMPLE_PB_H_
#define SIMPLE_PB_H_

#include <string>
#include <vector>

namespace AA {
namespace BB {

enum FriendType {
  Human,
  Animal,
};

}  // namespace BB

class DogInfo {
 public:
  enum Sex {
    Male,
    Female,
  };

  // constructors and destructor //
  DogInfo() = default;
  ~DogInfo();
  DogInfo(const DogInfo& other);  // copy constructor
  DogInfo(DogInfo&& other);  // move constructor
  DogInfo& operator=(const DogInfo& other);  // copy assignment
  DogInfo& operator=(DogInfo&& other);  // move assignment
  void Swap(DogInfo* other);

  // --- Field accessors --- //

  // "age" = 1
  int age() const;
  void set_age(int age);
  void clear_age();

  // "name" = 2
  const std::string& name() const;
  void set_name(const std::string& name);
  void set_name(const char* name);
  void set_name(const char* name, int size);
  std::string mutable_name();
  void clear_name();

  // "friend_type" = 4
  BB::FriendType friend_type() const;
  void set_friend_type(BB::FriendType friend_type);
  void clear_friend_type();

  // "sex" = 6
  DogInfo::Sex sex() const;
  void set_sex(DogInfo::Sex sex);
  void clear_sex();

 private:
  int age_;
  std::string name_;
  BB::FriendType friend_type_ = BB::Human;
  DogInfo::Sex sex_ = DogInfo::Male;
};

class FamilyInfo {
 public:
  // constructors and destructor //
  FamilyInfo() = default;
  ~FamilyInfo();
  FamilyInfo(const FamilyInfo& other);  // copy constructor
  FamilyInfo(FamilyInfo&& other);  // move constructor
  FamilyInfo& operator=(const FamilyInfo& other);  // copy assignment
  FamilyInfo& operator=(FamilyInfo&& other);  // move assignment
  void Swap(FamilyInfo* other);

  // --- Field accessors --- //

  // "address" = 1
  const std::string& address() const;
  void set_address(const std::string& address);
  void set_address(const char* address);
  void set_address(const char* address, int size);
  std::string mutable_address();
  void clear_address();

  // "numberdogs" = 2
  int numberdogs() const;
  void set_numberdogs(int numberdogs);
  void clear_numberdogs();

 private:
  std::string address_;
  int numberdogs_;
};

namespace BB {
namespace CC {

class DogFamilyInfo {
 public:
  // constructors and destructor //
  DogFamilyInfo() = default;
  ~DogFamilyInfo();
  DogFamilyInfo(const DogFamilyInfo& other);  // copy constructor
  DogFamilyInfo(DogFamilyInfo&& other);  // move constructor
  DogFamilyInfo& operator=(const DogFamilyInfo& other);  // copy assignment
  DogFamilyInfo& operator=(DogFamilyInfo&& other);  // move assignment
  void Swap(DogFamilyInfo* other);

  // --- Field accessors --- //

  // "family" = 1
  const AA::FamilyInfo& family() const;
  AA::FamilyInfo* mutable_family();
  void set_allocated_family(AA::FamilyInfo* family);
  AA::FamilyInfo* release_family();
  void clear_family();

  // "utility_fee" = 2
  int utility_fee_size() const;
  double utility_fee(int index);
  void set_utility_fee(int index, double value);
  void add_utility_fee(double value);
  void clear_utility_fee();
  const std::vector<double> utility_fee() const;
  std::vector<double> mutable_utility_fee();

  // "dogs" = 3
  int dogs_size() const;
  const AA::DogInfo& dogs(int index);
  void set_dogs(int index, const AA::DogInfo& value);
  void add_dogs(const AA::DogInfo& value);
  AA::DogInfo& mutable_dogs(int index);
  void clear_dogs();
  const std::vector<AA::DogInfo> dogs() const;
  std::vector<AA::DogInfo> mutable_dogs();

  // "locations" = 4
  int locations_size() const;
  const std::string& locations(int index);
  void set_locations(int index, const std::string& value);
  void set_locations(int index, const char* value);
  void set_locations(int index, const char* value, int size);
  void add_locations(const std::string& value);
  void add_locations(const char* value);
  void add_locations(const char* value, int size);
  std::string& mutable_locations(int index);
  void clear_locations();
  const std::vector<std::string> locations() const;
  std::vector<std::string> mutable_locations();

 private:
  AA::FamilyInfo* family_ = nullptr;
  std::vector<double> utility_fee_;
  std::vector<AA::DogInfo> dogs_;
  std::vector<std::string> locations_;
};

}  // namespace CC
}  // namespace BB
}  // namespace AA


#endif  /* SIMPLE_PB_H_ */
