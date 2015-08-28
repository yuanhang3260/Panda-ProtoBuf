#ifndef WIRE_FORMAT
#define WIRE_FORMAT

#include "../Utility/StringBuilder.h"

namespace proto {

typedef int int32;
typedef long long int64;
typedef unsigned int uint32;
typedef unsigned long long uint64;

class DecodeException: public std::exception {
 private:
  std::string message_;
 public:
  explicit DecodeException(const std::string& message);
  virtual const char* what() const throw() {
      return message_.c_str();
  }
};

DecodeException::DecodeException(const std::string& message) :
  message_(message) {}

class WireFormat {
 public:
  enum WireType {
    WIRETYPE_VARIANT            = 0,
    WIRETYPE_FIXD64             = 1,
    WIRETYPE_LENGTH_DIMITED     = 2,
    WIRETYPE_START_GROUP        = 3,
    WIRETYPE_END_GROUP          = 4,
    WIRETYPE_FIXD32             = 5,
  };

  static const int kWireTypeBits = 3;
  static const int kWireTypeMask = 0x7;

  static const int kTagByteSize = 7;
  static const int kTagByteMask = 0x7f;
  static const int kTagLowByteSize = 4;
  static const int kTagLowByteMask = 0xf;

  static const char kVariantNotEndBit = 0x80;

  static uint32 ZigZag32(int32 value);
  static uint64 ZigZag64(int64 value);

  static int32 UnZigZag32(uint32 value);
  static int64 UnZigZag64(uint64 value);

  static void WriteFixed64(uint64 value, Utility::StringBuilder* outstream);

  static uint64 RawCastDoubleToUint64(double value);
  static uint64 RawCastUint64ToDouble(uint64 value);

  // ------------------- Encode functions ------------------------- //
  static void EncodeTag(uint32 tag, WireType wire_type,
                        Utility::StringBuilder* outstream);
  static void EncodeUInt32(uint32 tag, uint32 value,
                           Utility::StringBuilder* outstream);
  static void EncodeUInt64(uint32 tag, uint64 value,
                           Utility::StringBuilder* outstream);


  static void EncodeSInt32(uint32 tag, int32 value,
                           Utility::StringBuilder* outstream);
  static void EncodeSInt64(uint32 tag, int64 value,
                           Utility::StringBuilder* outstream);

  static void EncodeDouble(uint32 tag, double value,
                           Utility::StringBuilder* outstream);

  static void EncodeBool(uint32 tag, bool value,
                         Utility::StringBuilder* outstream);

  static void EncodeString(uint32 tag, const std::string& str,
                           Utility::StringBuilder* outstream);

  // -------------------- Decode functions ------------------------ //
  static void DecodeTag(
    const char* buf, uint32* tag, int* wire_type, int* size);

  static uint32 DecodeUInt32(const char* buf, int* size);
  static uint64 DecodeUInt64(const char* buf, int* size);

  static int32 DecodeSInt32(const char* buf, int* size);
  static int64 DecodeSInt64(const char* buf, int* size);

  static double DecodeDouble(const char* buf, int* size);

  static bool DecodeBool(const char* buf, int* size);

  std::string DecodeString(const char* buf, int size);

 private:
  static void WriteVariant32(uint32 value, Utility::StringBuilder* outstream);
  static void WriteVariant64(uint64 value, Utility::StringBuilder* outstream);
};

}  // namespace proto

#endif  /* WIRE_FORMAT */