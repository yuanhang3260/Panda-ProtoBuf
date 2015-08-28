#include <exception>

#include "WireFormat.h"

namespace proto {

void WireFormat::WriteVariant32(uint32 value,
                                Utility::StringBuilder* outstream) {
  WriteVariant64(static_cast<uint64>(value), outstream);
}

void WireFormat::WriteVariant64(uint64 value,
                                Utility::StringBuilder* outstream) {
  while (value > 0) {
    char b = static_cast<char>(value & kTagByteSize);
    value = value >> kTagByteSize;
    if (value > 0) {
      b |= kVariantNotEndBit;
    }
    outstream->Append(b);
  }
}

void WireFormat::WriteFixed64(uint64 value,
                              Utility::StringBuilder* outstream) {
  outstream->Append(reinterpret_cast<const char*>(&value), sizeof(uint64));
}

void WireFormat::EncodeTag(uint32 tag, WireType wire_type,
                           Utility::StringBuilder* outstream) {
  WriteVariant32((tag << kWireTypeBits) | wire_type, outstream);
}

uint32 WireFormat::ZigZag32(int32 value) {
  return (value << 1) ^ (value >> 31);
}

uint64 WireFormat::ZigZag64(int64 value) {
  return (value << 1) ^ (value >> 63);
}

int32 WireFormat::UnZigZag32(uint32 value) {
  return (value >> 1) ^ -static_cast<int32>(value & 1);
}

int64 WireFormat::UnZigZag64(uint64 value) {
  return (value >> 1) ^ -static_cast<int64>(value & 1);
}

uint64 RawCastDoubleToUint64(double value) {
  union { double f; uint64 i; };
  f = value;
  return i;
}

double RawCastUint64ToDouble(uint64 value) {
  union { double f; uint64 i; };
  i = value;
  return f;
}

// -------------------- Decode functions ------------------------ //
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

void WireFormat::EncodeSInt32(uint32 tag, int32 value,
                              Utility::StringBuilder* outstream) {
  EncodeTag(tag, WIRETYPE_VARIANT, outstream);
  WriteVariant32(ZigZag32(value), outstream);
}

void WireFormat::EncodeSInt64(uint32 tag, int64 value,
                              Utility::StringBuilder* outstream) {
  EncodeTag(tag, WIRETYPE_VARIANT, outstream);
  WriteVariant64(ZigZag64(value), outstream);
}

void WireFormat::EncodeDouble(uint32 tag, double value,
                              Utility::StringBuilder* outstream) {
  EncodeTag(tag, WIRETYPE_FIXD64, outstream);
  WriteFixed64(RawCastDoubleToUint64(value), outstream);
}

void WireFormat::EncodeBool(uint32 tag, bool value,
                            Utility::StringBuilder* outstream) {
  EncodeTag(tag, WIRETYPE_VARIANT, outstream);
  WriteVariant64(static_cast<uint32>(value), outstream);
}

void WireFormat::EncodeString(uint32 tag, const std::string& str,
                              Utility::StringBuilder* outstream) {
  EncodeTag(tag, WIRETYPE_LENGTH_DIMITED, outstream);
  WriteVariant32(str.length(), outstream);
  for (uint32 i = 0; i < str.length(); i++) {
    outstream->Append(str[i]);
  }
}

// -------------------- Decode functions ------------------------ //
uint32 WireFormat::DecodeUInt32(const char* buf, int* size) {
  uint32 result = 0;
  int index = 0, offset = 0, cnt = 0;
  while (1) {
    char c = buf[index++];
    result |= ((c & kTagByteMask) << offset);
    offset += kTagByteSize;
    if ((c & (~kTagByteMask)) == 0) {
      break;
    }
    cnt++;
    if (cnt > 4) {
      throw DecodeException("too long encoded unsigned 32");
    }
  }
  *size = cnt + 1;
  return result;
}

uint64 WireFormat::DecodeUInt64(const char* buf, int* size) {
  uint64 result = 0;
  int index = 0, offset = 0, cnt = 0;
  while (1) {
    char c = buf[index++];
    result |= ((c & kTagByteMask) << offset);
    offset += kTagByteSize;
    if ((c & (~kTagByteMask)) == 0) {
      break;
    }
    cnt++;
    if (cnt > 9) {
      throw DecodeException("too long encoded unsigned 64");
    }
  }
  *size = cnt + 1;
  return result;
}

int32 WireFormat::DecodeSInt32(const char* buf, int* size) {
  uint32 raw_uint32 = DecodeUInt32(buf, size);
  return UnZigZag32(raw_uint32);
}

int64 WireFormat::DecodeSInt64(const char* buf, int* size) {
  uint64 raw_uint64 = DecodeUInt64(buf, size);
  return UnZigZag64(raw_uint64);
}

double WireFormat::DecodeDouble(const char* buf, int* size) {
  uint64 raw_uint64 = DecodeUInt64(buf, size);
  return RawCastUint64ToDouble(raw_uint64);
}

bool WireFormat::DecodeBool(const char* buf, int* size) {
  uint32 raw_uint32 = DecodeUInt32(buf, size);
  return static_cast<bool>(raw_uint32);
}

std::string WireFormat::DecodeString(const char* buf, int size) {
  return std::string(buf, size);
}




}  // namespace proto
