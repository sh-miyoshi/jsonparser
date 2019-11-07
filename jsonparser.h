#pragma once

#include <map>
#include <string>
#include <vector>

namespace json {
class Error {
  public:
    bool success;
    std::string message;

    Error() : success(true) {}
    ~Error() {}
};

class Value {
  public:
    enum Type {
        eTYPE_NULL,
        eTYPE_STRING,
        eTYPE_OBJECT,
        eTYPE_ARRAY
    };

  private:
    Type type;
    std::string str;
    std::map<std::string, Value> obj;
    std::vector<Value> ary;

  public:
    Value() : type(eTYPE_NULL) {}
    ~Value() {}

    // Setter
    Error SetObject(std::string key, Value value);
    Error SetArray(Value value);
    Error SetString(std::string value);

    // Getter
    Type GetType() const { return type; }
    std::string GetString() const { return str; }
    std::map<std::string, Value> GetObject() const { return obj; }
    std::vector<Value> GetArray() const { return ary; }

    std::string Get() const { return str; }
    Value &operator[](int index) { return ary[index]; }
    Value &operator[](std::string key) { return obj[key]; }
};

class Parser {
    std::string data;
    Value result;
    Error err;

    void Remove();
    char FirstChar();
    Value ParseValue();
    Value ParseArray();
    Value ParseObject();
    Value ParseString();
    Error Print(Value data);

  public:
    Parser() {}
    ~Parser() {}

    Error ParseFile(std::string fileName);
    Error ParseString(std::string str);
    Error Print();
    Value Get();
};
}; // namespace json