
# DataStream

simple binary encode and decode function

# Usage

## OutDataStream

```
OutDataStream out;

uint8_t i8 = 10;
uint16_t i16 = 0x1234;
std::string s = "abc";

out << i8 << s << i16;

std::string res = out.data();
```

## InDataStream

```
InDataStream in(res);

uint8_t oi8;
uint16_t oi16;
uint8_t c;

std::string ostr;
in >> oi8 >> manip::in_string(ostr, 3) >> oi16;
```

## ProtoBuf

```
test_node::ProtoNode n1;
n1.set_id(1);
n1.set_value(11);

OutDataStream out;
out << (uint16_t)n1.ByteSize() << manip::protobuf(n1);

std::string res = out.data();

test_node::ProtoNode n2;

InDataStream in(res);
uint16_t sz = 0;
in >> sz;
in >> manip::protobuf(n2, sz);
```
