#include "Proto/DescriptorsBuilder.h"

#include <memory>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Base/Log.h"
#include "UnitTest/UnitTest.h"
#include "Compiler/EnumType.h"
#include "Compiler/Message.h"
#include "Compiler/MessageField.h"
#include "Compiler/ProtoParser.h"
#include "Compiler/ServiceType.h"
#include "Compiler/Type.h"

namespace proto {

class DescriptorsBuilderTest: public UnitTest {
 private:
  std::unique_ptr<DescriptorsBuilder> builder_;
 
 public:
  void Test_BuildForTinyProto() {
    const char* const kProtoFilePath = "samples/tiny.proto";
    builder_.reset(new DescriptorsBuilder(kProtoFilePath));

    std::shared_ptr<ProtoFileDescriptor> file_(builder_->BuildDescriptors());
    AssertTrue(file_.get() != nullptr);
    AssertEqual(2, file_->num_enums(), "num_enums");
    AssertEqual(1, file_->num_nested_enums(), "num_nested_enums");
    AssertEqual(2, file_->num_messages(), "num_messages");
    AssertEqual(0, file_->num_services(), "num_services");

    // Check enum types.
    auto enum_dscpt = file_->FindEnumTypeByName("AA.FriendType");
    AssertTrue(enum_dscpt != nullptr);
    AssertEqual(2, enum_dscpt->NumberEnums(), "AA.FriendType NumberEnums");
    AssertTrue(enum_dscpt->ContainsEnum("Human"), "AA.FriendType has Human");
    AssertTrue(enum_dscpt->ContainsEnum("Animal"), "AA.FriendType has Animal");
    AssertFalse(enum_dscpt->is_nested(), "AA.FriendType not nested");

    enum_dscpt = file_->FindEnumTypeByName("AA.BB.DogInfo.Sex");
    AssertTrue(enum_dscpt != nullptr);
    AssertEqual(2, enum_dscpt->NumberEnums(), "AA.BB.DogInfo.Sex NumberEnums");
    AssertTrue(enum_dscpt->ContainsEnum("MALE"), "AA.BB.DogInfo.Sex has MALE");
    AssertTrue(enum_dscpt->ContainsEnum("FEMALE"), "AA.BB.DogInfo.Sex has FEMALE");
    AssertTrue(enum_dscpt->is_nested(), "AA.BB.DogInfo.Sex nested");

    // Check message types.
    auto msg_dscpt = file_->FindMessageTypeByName("AA.FamilyInfo");
    AssertTrue(msg_dscpt != nullptr);
    AssertEqual(2, msg_dscpt->num_fields(), "AA.FamilyInfo num_fields");
    AssertEqual(0, msg_dscpt->num_nested_enums(), "AA.FamilyInfo num_nested_enums");
    AssertEqual(msg_dscpt->FindFieldByName("address"),
                msg_dscpt->FindFieldByTag(1));
    AssertEqual(msg_dscpt->FindFieldByName("numberdogs"),
                msg_dscpt->FindFieldByTag(2));
    auto field_dscpt = msg_dscpt->FindFieldByName("address");
    AssertTrue(field_dscpt->has_default_value());
    AssertEqual(std::string("\"Haimen Street\""), field_dscpt->default_value());
    field_dscpt = msg_dscpt->FindFieldByName("numberdogs");
    AssertTrue(field_dscpt->has_default_value());
    AssertEqual(std::string("0"), field_dscpt->default_value());

    msg_dscpt = file_->FindMessageTypeByName("AA.BB.DogInfo");
    AssertTrue(msg_dscpt != nullptr);
    AssertEqual(8, msg_dscpt->num_fields(), "AA.BB. num_fields");
    AssertEqual(1, msg_dscpt->num_nested_enums(), "AA.BB. num_nested_enums");
    AssertTrue(msg_dscpt->FindFieldByName("age") ==
                msg_dscpt->FindFieldByTag(1));
    AssertTrue(msg_dscpt->FindFieldByName("name") ==
               msg_dscpt->FindFieldByTag(3));
    AssertTrue(msg_dscpt->FindFieldByName("alias") ==
               msg_dscpt->FindFieldByTag(18));
    AssertTrue(msg_dscpt->FindFieldByName("weight") ==
               msg_dscpt->FindFieldByTag(5));
    AssertTrue(msg_dscpt->FindFieldByName("family") ==
               msg_dscpt->FindFieldByTag(13));
    AssertTrue(msg_dscpt->FindFieldByName("neighbors") ==
               msg_dscpt->FindFieldByTag(25));
    AssertTrue(msg_dscpt->FindFieldByName("sex") ==
               msg_dscpt->FindFieldByTag(4));
    AssertTrue(msg_dscpt->FindFieldByName("friend_type") ==
               msg_dscpt->FindFieldByTag(10));

    field_dscpt = msg_dscpt->FindFieldByName("family");
    AssertFalse(field_dscpt->has_default_value());
    AssertTrue(field_dscpt->type_descriptor() ==
               file_->FindMessageTypeByName("AA.FamilyInfo"));
    AssertTrue(field_dscpt->container_message() ==
               file_->FindMessageTypeByName("AA.BB.DogInfo"));
    field_dscpt = msg_dscpt->FindFieldByName("neighbors");
    AssertFalse(field_dscpt->has_default_value());
    AssertTrue(field_dscpt->type_descriptor() ==
               file_->FindMessageTypeByName("AA.FamilyInfo"));
    AssertTrue(field_dscpt->container_message() ==
               file_->FindMessageTypeByName("AA.BB.DogInfo"));
    field_dscpt = msg_dscpt->FindFieldByName("sex");
    AssertFalse(field_dscpt->has_default_value(), "sex default value");
    AssertTrue(field_dscpt->type_descriptor() ==
               file_->FindEnumTypeByName("AA.BB.DogInfo.Sex"));
    AssertTrue(field_dscpt->container_message() ==
               file_->FindMessageTypeByName("AA.BB.DogInfo"));
    field_dscpt = msg_dscpt->FindFieldByName("friend_type");
    AssertFalse(field_dscpt->has_default_value(), "friend_type default value");
    AssertTrue(field_dscpt->type_descriptor() ==
               file_->FindEnumTypeByName("AA.FriendType"));
    AssertTrue(field_dscpt->container_message() ==
               file_->FindMessageTypeByName("AA.BB.DogInfo"));
  }

  void Test_BuildForSimpleProto() {
    const char* const kProtoFilePath = "samples/simple.proto";
    builder_.reset(new DescriptorsBuilder(kProtoFilePath));

    std::shared_ptr<ProtoFileDescriptor> file_(builder_->BuildDescriptors());
    AssertTrue(file_.get() != nullptr);
    AssertEqual(3, file_->num_enums(), "num_enums");
    AssertEqual(3, file_->num_nested_enums(), "num_nested_enums");
    AssertEqual(5, file_->num_messages(), "num_messages");
    // TODO(hangyuan) : service descriptors to build.
    AssertEqual(0, file_->num_services(), "num_services");
  }
};

}  // namespace proto

int main(int argc, char** argv) {
  proto::DescriptorsBuilderTest test;
  test.setup();

  test.Test_BuildForTinyProto();
  test.Test_BuildForSimpleProto();

  test.teardown();
  std::cout << "\033[2;32mAll Passed ^_^\033[0m" << std::endl;
  return 0;
}