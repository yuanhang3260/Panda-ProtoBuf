##C++ Protocol Buffer Framework.

It's a very simple implementation of protocol buffer framework. The syntax and API design are identical to google's protobuf. Current version supports:

1. Basic protocol buffer compiler for C++.
2. C++ message reflection and serialization.
3. RPC service framwork.

#####Compile a .proto file to generate C++ code:
    make
    ./compile cpp $proto_file
It generates a .h and .cpp file in the same directory of the proto file.