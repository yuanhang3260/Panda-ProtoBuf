#include "WireFormat.h"

namespace proto {

void WireFormat::WriteVariant32(uint32 value,
                                Utility::StringBuilder* outstream) {
  while (value > 0) {
    char b = static_cast<char>(tag & kTagByteSize);
    value = value >> kTagByteSize;
    if (value > 0) {
      b |= kVariantNotEndBit;
    }
    outstream->Append(b);
  }
}

void WireFormat::WriteVariant64(uint64 value,
                                Utility::StringBuilder* outstream) {
  while (value > 0) {
    char b = static_cast<char>(tag & kTagByteSize);
    value = value >> kTagByteSize;
    if (value > 0) {
      b |= kVariantNotEndBit;
    }
    outstream->Append(b);
  }
}

void WireFormat::EncodeTag(uint32 tag, WireType wire_type,
                           Utility::StringBuilder* outstream) {
  WriteVariant32((tag << kWireTypeBits) | wire_type);
}

void WireFormat::EncodeUInt32(uint32 tag, uint32 value,
                              Utility::StringBuilder* outstream) {
  EncodeTag(tag, WIRETYPE_VARIANT, outstream);
  WriteVariant32(value, outstream);
}

void WireFormat::EncodeUInt64(uint32 tag, uint64 value,
                              Utility::StringBuilder* outstream) {
  EncodeTag(tag, WIRETYPE_VARIANT, outstream);
  WriteVariant64(value, outstream);
}

}  // namespace proto
