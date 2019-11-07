#pragma once

#include <map>
#include <memory>
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
    std::map<std::string, std::shared_ptr<Value>> obj;
    std::vector<std::shared_ptr<Value>> ary;

  public:
    Value() : type(eTYPE_NULL) {}
    ~Value() {}

    // Setter
    Error SetObject(std::string key, std::shared_ptr<Value> value);
    Error SetArray(std::shared_ptr<Value> value);
    Error SetString(std::string value);

    // Getter
    Type GetType() const { return type; }
    std::string GetString() const { return str; }
    std::map<std::string, std::shared_ptr<Value>> GetObject() const { return obj; }
    std::vector<std::shared_ptr<Value>> GetArray() const { return ary; }
    // TODO(Get(), ["key"], [index])
};

class Parser {
    std::string data;
    std::shared_ptr<Value> result;
    Error err;

    void Remove();
    char FirstChar();
    std::shared_ptr<Value> ParseValue();
    std::shared_ptr<Value> ParseArray();
    std::shared_ptr<Value> ParseObject();
    std::shared_ptr<Value> ParseString();
    Error Print(Value data);

  public:
    Parser() {}
    ~Parser() {}

    Error ParseFile(std::string fileName);
    Error ParseString(std::string str);
    Error Print();
    std::shared_ptr<Value> Get();
};
}; // namespace json