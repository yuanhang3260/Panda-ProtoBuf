#ifndef GENERATED_MESSAGE_REFLECTION_
#define GENERATED_MESSAGE_REFLECTION_

namespace proto {

#define PROTO_MESSAGE_FIELD_OFFSET(TYPE, FIELD)                \
  static_cast<int>(                                            \
      reinterpret_cast<const char*>(                           \
          &reinterpret_cast<const TYPE*>(16)->FIELD) -         \
      reinterpret_cast<const char*>(16))

#define CHECK(condition, error_msg)            \
  if (!condition) {                            \
    throw std::runtime_error(error_msg);       \
  }

class MessageReflection {

};

}  // namespace proto

#endif  /* GENERATED_MESSAGE_REFLECTION_ */