
# a simple leveldb wrapper

## usage

1. init library

```
storage store;
store.Init("./db");
```

2. implement function

```
bool DeserializeFromString(const std::string & key, A * a);
bool SerializeToString(const A & a, std::string * out);
int Compare(const A & key1, const A & key2);
```

3. register key type

```
struct A;
store.Register(1, A());
```

## example

```
jewel::Storage store;
store.Init("./db");

store.Register(1, std::string());

std::string value1 = "this is a test value";
store.Put(1, "key1", value1);

std::string value2;
store.Get(1, "key1", &value2);
```

