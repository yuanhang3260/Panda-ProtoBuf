#ifndef WIRE_FORMAT
#define WIRE_FORMAT

#include "../Utility/StringBuilder.h"

namespace proto {

class WireFormat {
 public:
  typedef int int32;
  typedef long long int64;
  typedef unsigned int uint32;
  typedef unsigned long long uint64;

  enum WireType {
    WIRETYPE_VARIANT            = 0,
    WIRETYPE_FIXD64             = 1,
    WIRETYPE_LENGTH_DIMITED     = 2,
    WIRETYPE_START_GROUP        = 3,
    WIRETYPE_END_GROUP          = 4,
    WIRETYPE_FIXD32             = 5,
  };

  static const int kWireTypeBits = 3;

  static const int kTagByteSize = 7;
  static const int kTagByteMask = 0x7f;
  static const int kTagLowByteSize = 4;
  static const int kTagLowByteMask = 0xf;

  static const char kVariantNotEndBit = 0x80;

  static void EncodeTag(uint32 tag, WireType wire_type,
                        Utility::StringBuilder* outstream);

  static void EncodeUInt32(uint32 tag, uint32 value,
                           Utility::StringBuilder* outstream);
  static void EncodeUInt64(uint32 tag, uint64 value,
                           Utility::StringBuilder* outstream);

 private:
  static void WriteVariant32(uint32 value, Utility::StringBuilder* outstream);
  static void WriteVariant64(uint64 value, Utility::StringBuilder* outstream);
};

}  // namespace proto

#endif  /* WIRE_FORMAT */