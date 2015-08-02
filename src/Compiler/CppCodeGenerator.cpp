#include <iostream>
#include "../Utility/Strings.h"
#include "../Utility/Utils.h"
#include "CppCodeGenerator.h"

namespace PandaProto {
namespace Compiler {

std::map<FIELD_TYPE, std::string> pbCppTypeMap{
  {INT32, "int"},
  {INT64, "long long"},
  {UINT32, "unsigned int"},
  {UINT64, "unsigned long long"},
  {DOUBLE, "dobule"},
  {STRING, "std::string"},
  {BOOL, "bool"},
};

void CppCodeGenerator::GenerateCode() {
  GenerateHeader();
  GenerateCC();
}

void CppCodeGenerator::GenerateHeader() {
  std::string outfile;
  outfile = proto_file_.substr(0, proto_file_.length() - 6) + "_pb";

  // Open .h file
  if (!printer.Open(outfile + ".h")) {
    fprintf(stderr, "ERROR: Open output file %s.h failed\n", outfile.c_str());
  }

  std::vector<std::string> result = StringUtils::Split(outfile, '/');
  std::string filename = result[result.size() - 1];

  printer.Print("#ifndef " + StringUtils::Upper(filename) + "_H_\n");
  printer.Print("#define " + StringUtils::Upper(filename) + "_H_\n\n");
  printer.Print("#include <string>\n");
  printer.Print("#include <vector>\n\n");

  // Print global enums.
  for (auto& e: enums_map_) {
    EnumType* enum_p = e.second.get();
    CheckoutNameSpace(pkg_stack_, enum_p->pkg_stack());
    printer.Print("enum " + enum_p->name() + " {\n");
    for (auto& enumvalue: enum_p->enums()) {
      printer.Print("  " + enumvalue + ",\n");
    }
    printer.Print("};\n\n");
  }

  // Print classes.
  for (auto& message: messages_list_) {
    CheckoutNameSpace(pkg_stack_, message->pkg_stack());
    printer.Print("class " + message->name() + " {\n");

    // Public fields.
    printer.Print(" public:\n");
    // Print enums declearation.
    for (auto& e: message->enums_map()) {
      EnumType* enum_p = e.second.get();
      printer.Print("  enum " + enum_p->name() + " {\n");
      for (auto& enumvalue: enum_p->enums()) {
        printer.Print("    " + enumvalue + ",\n");
      }
      printer.Print("  };\n\n");
    }

    // Print constructors and destructor.
    std::map<std::string, std::string> msg_match{
       {"msg_name", message->name()},
    };
    printer.Print("  // constructors and destructor //\n");
    printer.Print("  " + message->name() + "() = default;\n");
    printer.Print("  ~" + message->name() + "();\n");
    printer.Print(
        "  ${msg_name}(const ${msg_name}& other);"
        "  // copy constructor\n",
        msg_match);
    printer.Print(
        "  ${msg_name}(${msg_name}&& other);  // move constructor\n",
        msg_match);
    printer.Print(
        "  ${msg_name}& operator=(const ${msg_name}& other);"
        "  // copy assignment\n",
        msg_match);
    printer.Print(
        "  ${msg_name}& operator=(${msg_name}&& other);  // move assignment\n",
        msg_match);
    printer.Print("  void Swap(${msg_name}* other);\n", msg_match);
    printer.Print("\n");

    // Print public methods.
    printer.Print("  // --- Field accessors --- //\n\n");
    for (auto& field: message->fields_list()) {
      std::string field_name = field->name();
      std::string type_name = field->type_name();
      if (field->type() != ENUMTYPE &&
          field->type() != MESSAGETYPE) {
        type_name = pbCppTypeMap.at(field->type());
      }
      else {
        // enum and message types: first get the correct namespace prefix
        type_name = GetNameSpacePrefix(message->pkg_stack(),
                                       field->type_class()->pkg_stack())
                    + type_name;
      }
      printer.Print("  // \"" + field->name() + "\" = " +
                     std::to_string(field->tag()) + "\n");
      
      // methods for required and optional fields
      if (field->modifier() != MessageField::REPEATED) {
        // get method and set method
        std::string get_method_line = "  ${type} ${field_name}() const;\n";
        std::string set_method_line =
              "  void set_${field_name}(const ${type} ${field_name});\n";
        if (field->type() == STRING) {
          get_method_line = "  const ${type}& ${field_name}() const;\n";
          set_method_line =
              "  void set_${field_name}(const ${type}& ${field_name});\n";
        }
        std::map<std::string, std::string> matches{
           {"field_name", field_name},
           {"type", type_name},
        };

        // define: int get_foo() const / const Bar& get_foo()
        printer.Print(get_method_line, matches);
        // define: void set_foo(int foo) / void set_foo(const Bar& foo)
        printer.Print(set_method_line, matches);
        
        // string type has other methods.
        if (field->type() == STRING) {
          // define: const set_foo(char* value)
          printer.Print(
              "  void set_${field_name}(const char* ${field_name});\n",
              matches);
          // define: const set_foo(char* value, int size)
          printer.Print(
              "  void set_${field_name}(const char* ${field_name}, int size);\n",
              matches);
          // define: string* mutable_foo()
          printer.Print(
              "  std::string* mutable_${field_name}();\n", matches);
          // define: void clear_foo()
          printer.Print(
              "  void clear_${field_name}();\n", matches);
        }
        // message type has other methods.
        if (field->type() == MESSAGETYPE &&
            field->modifier() != MessageField::REPEATED) {
          // define: Bar* mutable_foo()
          printer.Print("  {type}* mutable_${field_name}();\n", matches);
          // define: void set_allocated_foo(Bar* foo)
          printer.Print(
              "  void set_allocated_${field_name}(${type}* ${field_name});\n",
              matches);
          // define: Bar* release_foo()
          printer.Print("  ${type}* release_${field_name}();\n", matches);
        }
      }
      // methods of repeated field.
      else {
        // get method and set method
        std::map<std::string, std::string> matches{
           {"field_name", field_name},
           {"type", type_name},
        };
        // define: int foo_size() const
        printer.Print("  int ${field_name}_size() const;\n", matches);
        if (field->type() != MESSAGETYPE) {
          // define: Bar foo(int index) const
          printer.Print("  ${type} ${field_name}(int index) const;\n", matches);
          // define: void set_foo(int index, Bar& foo)
          printer.Print(
              "  void set_${field_name}(int index, ${type} ${field_name});\n",
              matches);
          // define: void add_foo(Bar& foo)
          printer.Print(
              "  void add_${field_name}(${type} ${field_name});\n", matches);
        }
        else {
          // define: const Bar& foo(int index) const
          printer.Print("  const ${type}& ${field_name}(int index) const;\n",
                        matches);
          // define: void set_foo(int index, Bar& foo)
          printer.Print(
            "  void set_${field_name}(int index, const ${type}& ${field_name});\n",
            matches);
          // define: void add_foo(Bar& foo)
          printer.Print(
              "  void add_${field_name}(const ${type}& ${field_name});\n",
              matches);
        } 
        // define: const std::vector<Bar> foo() const
        printer.Print(
            "  const std::vector<${type}>& ${field_name}() const;\n", matches);
        // define: std::vector<Bar>* mutable_foo()
        printer.Print(
            "  std::vector<${type}>* mutable_${field_name}();\n", matches);
      }

      printer.Print("\n");
    }

    // Private fields.
    printer.Print(" private:\n");
    for (auto& field: message->fields_list()) {
      std::string field_name = field->name();
      std::string type_name = field->type_name();
      if (field->type() != ENUMTYPE &&
          field->type() != MESSAGETYPE) {
        type_name = pbCppTypeMap.at(field->type());
      }
      else {
        // Add namespace prefix for enum and message types.
        type_name = GetNameSpacePrefix(message->pkg_stack(),
                                       field->type_class()->pkg_stack())
                    + type_name;
      }
      // repeated fields are "vector<Bar>* foo" type
      if (field->modifier() == MessageField::REPEATED) {
        type_name = "std::vector<" + type_name + ">";
      }

      std::string declearation_line = "  $ $_";
      if (field->type() == MESSAGETYPE &&
          field->modifier() != MessageField::REPEATED) {
        declearation_line = "  $* $_";
      }
      printer.Print(declearation_line,
                    std::vector<std::string>{type_name, field_name});
      if (!field->default_value().empty()) {
        printer.Print(" = " + field->default_value());
      }
      printer.Print(";\n");
    }
    printer.Print("};\n\n");
  }

  CheckoutNameSpace(pkg_stack_, std::vector<std::string>());
  printer.Print("\n#endif  /* " + StringUtils::Upper(filename) + "_H_ */\n");
  printer.Flush();

}

void CppCodeGenerator::GenerateCC() {
  std::string outfile;
  outfile = proto_file_.substr(0, proto_file_.length() - 6) + "_pb";

  // Open .cpp file
  if (!printer.Open(outfile + ".cpp")) {
    fprintf(stderr, "ERROR: Open output file %s.p failed\n", outfile.c_str());
  }

  // Include proto_name.pb.h file
  std::vector<std::string> result = StringUtils::Split(outfile, '/');
  std::string filename = result[result.size() - 1];
  printer.Print("#include \"" + filename + ".h\"\n\n");

  // Print class methods.
  for (auto& message: messages_list_) {
    // Check out name space.
    CheckoutNameSpace(pkg_stack_, message->pkg_stack());
    // Print constructors and destructor.
    DefineCopyConstructor(message.get());
    DefineMoveConstructor(message.get());
    DefineCopyAssigner(message.get());
    DefineMoveAssigner(message.get());
    DefineSwapper(message.get());
    DefineDestructor(message.get());
  }

  CheckoutNameSpace(pkg_stack_, std::vector<std::string>());
  printer.Flush();
}

void CppCodeGenerator::DefineDestructor(Message* message) {
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };

  printer.Print("// destructor\n");
  printer.Print("${msg_name}::~${msg_name}() {\n", msg_match);
  if (message->has_message_field()) {
    for (auto& field : message->fields_list()) {
      if (field->type() == MESSAGETYPE &&
          field->modifier() != MessageField::REPEATED) {
        printer.Print("  delete " + field->name() + "_;\n");
      }
    }
  }
  printer.Print("}\n\n");
}

void CppCodeGenerator::PrintCopyClassCode(Message* message) {
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };

  for (auto& field : message->fields_list()) {
    std::map<std::string, std::string> matches =
        GetFieldMatchMap(message, field.get());

    if (field->IsSingularMessageType()) {
      printer.Print(
        "  if (!${field_name}_) {\n"
        "    ${field_name}_ = new ${type_name}();\n"
        "  }\n"
        "  *${field_name}_ = other.${field_name}();\n", matches);
    }
    else {
      printer.Print("  ${field_name}_ = other.${field_name}();\n", matches);
    }
  }
  printer.Print("}\n\n");
}

void CppCodeGenerator::DefineCopyConstructor(Message* message) {
  printer.Print("// copy constructor\n");
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print(
      "${msg_name}::${msg_name}(const ${msg_name}& other) {\n",
      msg_match);
  
  PrintCopyClassCode(message);
}

void CppCodeGenerator::DefineCopyAssigner(Message* message) {
  printer.Print("// copy assignment\n");
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print(
      "${msg_name}& operator=(const ${msg_name}& other) {\n",
      msg_match);
  
  PrintCopyClassCode(message);
}

void CppCodeGenerator::PrintMoveClassCode(Message* message) {
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };

  for (auto& field : message->fields_list()) {
    std::map<std::string, std::string> matches =
        GetFieldMatchMap(message, field.get());

    if (field->modifier() == MessageField::REPEATED ||
        field->type() == STRING) {
      printer.Print(
        "  ${field_name}_ = std::move<*other.mutable_${field_name}()>;\n",
        matches);
    }
    else if (field->type() == MESSAGETYPE) {
      printer.Print(
        "  if (${field_name}_ ) {\n"
        "    delete ${field_name}_;\n"
        "  }\n"
        "  ${field_name}_ = other.release_${field_name}();\n",
        matches);
    }
    else {
      printer.Print("  ${field_name}_ = other.${field_name}();\n", matches);
      printer.Print("  other.clear_${field_name}();\n", matches);
    }
  }
  printer.Print("}\n\n");
}

void CppCodeGenerator::DefineMoveConstructor(Message* message) {
  printer.Print("// move constructor\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("${msg_name}::${msg_name}(${msg_name}&& other) {\n", msg_match);

  PrintMoveClassCode(message);
}

void CppCodeGenerator::DefineMoveAssigner(Message* message) {
  printer.Print("// move assignment\n");

  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };
  printer.Print("${msg_name}& operator=(${msg_name}&& other) {\n", msg_match);

  PrintMoveClassCode(message);
}

void CppCodeGenerator::DefineSwapper(Message* message) {
  std::map<std::string, std::string> msg_match{
     {"msg_name", message->name()},
  };

  printer.Print("// swapper\n");
  printer.Print("void ${msg_name}::Swap(${msg_name}* other) {", msg_match);
  for (auto& field : message->fields_list()) {
    printer.Print("\n");

    std::map<std::string, std::string> matches =
        GetFieldMatchMap(message, field.get());

    if (field->modifier() == MessageField::REPEATED ||
        field->type() == STRING) {  // repeated type / string type
      printer.Print(
          "  ${type_name} ${field_name}_tmp__ = std::move<*other->mutable_${field_name}()>;\n"
          "  *other->mutable_${field_name} = std::move<${field_name}_>;\n"
          "  ${field_name}_ = std::move<${field_name}_tmp__>;\n",
          matches);
    }
    else if (field->type() == MESSAGETYPE) {  // message type
      printer.Print(
          "  ${type_name}* ${field_name}_tmp__ = other->release_${field_name}();\n"
          "  other->set_allocated_${field_name}(this->release_${field_name}());\n"
          "  set_allocated_${field_name}(${field_name}_tmp__);\n",
          matches);
    }
    else {  // regular primitive type / enum type
      printer.Print(
          "  ${type_name} ${field_name}_tmp__ = other->${field_name}();\n"
          "  other->set_${field_name}(${field_name}_);\n"
          "  set_${field_name}(${field_name}_tmp__);\n",
          matches);
    }
  }
  printer.Print("}\n\n");
}

void CppCodeGenerator::DefineAccessors(Message* message) {
  for (auto& field : message->fields_list()) {
    if (field->IsSingularNumericType()) {
      DefineSingularNumericTypeAccessors(message, field.get());
    }
  }
}

void CppCodeGenerator::DefineSingularNumericTypeAccessors(
    Message* message,
    MessageField* field) {
  std::map<std::string, std::string> matches = GetFieldMatchMap(message, field);
  
  // TODO: implement has_foo()
  // printer.Print("bool ${msg_name}::has_${field_name}() {\n");
  
  // Implement - int32 foo() const;
  printer.Print("${type_name} ${msg_name}::${field_name}() const {\n"
                "  return ${field_name}_;\n"
                "}",
                matches);
  
  // Implement - void set_foo(int32 value);
  printer.Print("void ${msg_name}::set_${field_name}(${type_name} ${field_name}) {\n"
                "  ${field_name}_ = ${field_name};\n"
                "}",
                matches);
  
  // TODO: clear_foo() requires setting has-bits.
  // Implement - void clear_foo(int32 value);
  printer.Print("void ${msg_name}::clear_${field_name}() {\n"
                "  ${field_name}_ = ${default_value};\n"
                "}",
                matches);
}

std::map<std::string, std::string>
CppCodeGenerator::GetFieldMatchMap(Message* message, MessageField* field) {
  std::string field_name = field->name();
  std::string type_name;
  std::string default_value = field->default_value();
  if (field->type() != ENUMTYPE &&
      field->type() != MESSAGETYPE) {
    type_name = pbCppTypeMap.at(field->type());
  }
  else {
    // Add namespace prefix for enum and message types.
    type_name = GetNameSpacePrefix(message->pkg_stack(),
                                   field->type_class()->pkg_stack())
                + field->type_name();
  }
  std::map<std::string, std::string> matches {
    {"msg_name", message->name()},
    {"field_name", field_name},
    {"type_name", type_name},
    {"default_value", default_value},
  };
  return matches;
}

void CppCodeGenerator::CheckoutNameSpace(
    std::vector<std::string>& context_stk,
    const std::vector<std::string>& target_stk) {
  int len = Utils::Min(context_stk.size(), target_stk.size());
  int i, index = len;
  for (i = 0; i < len; i++) {
    if (context_stk[i] != target_stk[i]) {
      index = i;
      break;
    }
  }
  bool printed = false;
  for (i = context_stk.size() - 1; i >= index; i--) {
    printer.Print("}  // namespace " + context_stk[i] + "\n");
    printed = true;
  }
  context_stk.resize(index);
  if (printed) {
    printer.Print("\n");
  }
  
  printed = false;
  for (i = index; i < (int)target_stk.size(); i++) {
    printer.Print("namespace " + target_stk[i] + " {\n");
    context_stk.push_back(target_stk[i]);
    printed = true;
  }
  if (printed) {
    printer.Print("\n");
  }
}

std::string CppCodeGenerator::GetNameSpacePrefix(
    const std::vector<std::string>& context_stk, // AA.DD
    const std::vector<std::string>& target_stk) { // AA
  int len = Utils::Min(context_stk.size(), target_stk.size());
  int i, index = len;
  for (i = 0; i < len; i++) {
    if (context_stk[i] != target_stk[i]) {
      index = i;
      break;
    }
  }

  // std::cout << "index = " << index << ", " << context_stk.size()
  //           << ", " << target_stk.size() << std::endl;
  // for (auto& s: context_stk) {
  //   std::cout << s << ".";
  // }
  // std::cout << std::endl;
  // for (auto& s: target_stk) {
  //   std::cout << s << ".";
  // }
  // std::cout << std::endl;

  std::string prefix = "";
  if (index < (int)context_stk.size()) {
    for (i = 0; i < (int)target_stk.size(); i++) {
      prefix += (target_stk[i] + "::");
    }
  }
  else {
    prefix = "::";
    for (i = context_stk.size(); i < (int)target_stk.size(); i++) {
      prefix += (target_stk[i] + "::");
    }
  }
  std::cout << "prefix = " << prefix << std::endl;
  return prefix;
}

}  // Compiler
}  // PandaProto
