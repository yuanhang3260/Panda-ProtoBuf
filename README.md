## C++ Protocol Buffer Framework.

It's a very simple implementation of protocol buffer framework. The syntax and API design are identical to google's [protobuf](https://github.com/google/protobuf). Current version supports:

- Basic protocol buffer compiler for C++.
- C++ message class reflection and serialization.
- RPC service framwork.

#### Usage
Compile `.proto` file to generate C++ code:
```bash
make
./compile cpp $proto_file
```

[src/Public](https://github.com/yuanhang3260/HyLib) points to my own C++ utility libraries.
