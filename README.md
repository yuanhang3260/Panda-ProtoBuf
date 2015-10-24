##C++ Protocol Buffer Framework.

It's a very simple implementation of protocol buffer framework. The syntax and API design are identical to google's [protobuf](https://github.com/google/protobuf). Current version supports:

1. Basic protocol buffer compiler for C++.
2. C++ message reflection and serialization.
3. RPC service framwork.

#####Compile a .proto file to generate C++ code:
    make
    ./compile cpp $proto_file
