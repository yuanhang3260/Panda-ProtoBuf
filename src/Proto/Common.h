#ifndef PROTO_COMMON_
#define PROTO_COMMON_

namespace proto {

enum FieldLabel {
  UNKNOWN_MODIFIER,
  OPTIONAL,
  REQUIRED,
  REPEATED,
};

enum FieldType {
  UNDETERMINED,
  INT32,
  INT64,
  UINT32,
  UINT64,
  DOUBLE,
  BOOL,
  STRING,
  ENUMTYPE,
  MESSAGETYPE,
  SERVICETYPE,
};

using FIELD_TYPE = FieldType;
using FIELD_MODIFIER = FieldLabel;

}


#endif  /* PROTO_COMMON_ */
