#ifndef WIRE_FORMAT_
#define WIRE_FORMAT_

#include "Base/BaseTypes.h"
#include "Strings/StringBuilder.h"

namespace proto {

class DecodeException: public std::exception {
 private:
  std::string message_;
 public:
  explicit DecodeException(const std::string& message);
  virtual const char* what() const throw() {
      return message_.c_str();
  }
};

class WireFormat {
 public:
  enum WireType {
    WIRETYPE_VARIANT            = 0,
    WIRETYPE_FIXD64             = 1,
    WIRETYPE_LENGTH_DELIMITED   = 2,
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

  static std::string WireTypeAsString(const WireType wire_type);

  static uint32 ZigZag32(int32 value);
  static uint64 ZigZag64(int64 value);

  static int32 UnZigZag32(uint32 value);
  static int64 UnZigZag64(uint64 value);

  static void WriteFixed64(uint64 value, Strings::StringBuilder* outstream);

  static uint64 RawCastDoubleToUint64(double value);
  static double RawCastUint64ToDouble(uint64 value);

  static void WriteVariant32(uint32 value, Strings::StringBuilder* outstream);
  static void WriteVariant64(uint64 value, Strings::StringBuilder* outstream);

  // -------------------- write functions ------------------------ //
  static void WriteUInt32(uint32 value, Strings::StringBuilder* outstream);
  static void WriteUInt64(uint64 value, Strings::StringBuilder* outstream);
  static void WriteSInt32(int32 value, Strings::StringBuilder* outstream);
  static void WriteSInt64(int64 value, Strings::StringBuilder* outstream);
  static void WriteDouble(double value, Strings::StringBuilder* outstream);
  static void WriteBool(bool value, Strings::StringBuilder* outstream);
  static void WriteChar(char value, Strings::StringBuilder* outstream);
  static void WriteString(const std::string& str,
                          Strings::StringBuilder* outstream);

  // ------------------- Encode functions ------------------------- //
  static void EncodeTag(const uint32 tag, WireType wire_type,
                        Strings::StringBuilder* outstream);
  static void EncodeUInt32(const uint32 tag, const uint32 value,
                           Strings::StringBuilder* outstream);
  static void EncodeUInt64(const uint32 tag, const uint64 value,
                           Strings::StringBuilder* outstream);


  static void EncodeSInt32(const uint32 tag, const int32 value,
                           Strings::StringBuilder* outstream);
  static void EncodeSInt64(const uint32 tag, const int64 value,
                           Strings::StringBuilder* outstream);

  static void EncodeDouble(const uint32 tag, const double value,
                           Strings::StringBuilder* outstream);

  static void EncodeBool(const uint32 tag, const bool value,
                         Strings::StringBuilder* outstream);

  static void EncodeChar(const uint32 tag, char value,
                         Strings::StringBuilder* outstream);  

  static void EncodeString(const uint32 tag, const std::string& str,
                           Strings::StringBuilder* outstream);

  // -------------------- Decode functions ------------------------ //
  static void DecodeTag(
    const char* buf, uint32* tag, WireType* wire_type, uint32* size);

  static uint32 DecodeUInt32(const char* buf, uint32* size);
  static uint64 DecodeUInt64(const char* buf, uint32* size);

  static int32 DecodeSInt32(const char* buf, uint32* size);
  static int64 DecodeSInt64(const char* buf, uint32* size);

  static double DecodeDouble(const char* buf, uint32* size);

  static bool DecodeBool(const char* buf, uint32* size);

  static char DecodeChar(const char* buf, uint32* size);

  static std::string DecodeString(const char* buf, uint32* size);

 private:
};

inline void WireFormat::WriteUInt32(uint32 value,
                                    Strings::StringBuilder* outstream) {
  WriteVariant32(value, outstream);
}

inline void WireFormat::WriteUInt64(uint64 value,
                                    Strings::StringBuilder* outstream) {
  WriteVariant64(value, outstream);
}

inline void WireFormat::WriteSInt32(int32 value,
                                    Strings::StringBuilder* outstream) {
  WriteVariant32(ZigZag32(value), outstream);
}

inline void WireFormat::WriteSInt64(int64 value,
                                    Strings::StringBuilder* outstream) {
  WriteVariant64(ZigZag64(value), outstream);
}

inline void WireFormat::WriteDouble(double value,
                                    Strings::StringBuilder* outstream) {
  WriteFixed64(RawCastDoubleToUint64(value), outstream);
}

inline void WireFormat::WriteBool(bool value,
                                  Strings::StringBuilder* outstream) {
  WriteVariant32(static_cast<uint32>(value), outstream);
}

inline void WireFormat::WriteChar(char value,
                                  Strings::StringBuilder* outstream) {
  outstream->Append(value);
}

inline void WireFormat::WriteString(const std::string& str,
                                    Strings::StringBuilder* outstream) {
  WriteVariant32(str.length(), outstream);
  for (uint32 i = 0; i < str.length(); i++) {
    outstream->Append(str[i]);
  }
}

}  // namespace proto

#endif  /* WIRE_FORMAT_ */