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
        eTYPE_NONE,
        eTYPE_STRING,
        eTYPE_OBJECT,
        eTYPE_ARRAY,
        eTYPE_BOOL,
        eTYPE_NUMBER,
        eTYPE_NULL
    };

  private:
    Type type;
    bool boolean;
    double number;
    std::string str;
    std::map<std::string, Value> obj;
    std::vector<Value> ary;
    bool isNull;

  public:
    Value() : type(eTYPE_NONE), boolean(false), isNull(false), number(0) {}
    ~Value() {}

    // Setter
    Error SetObject(std::string key, Value value);
    Error SetArray(Value value);
    Error SetString(std::string value);
    Error SetBool(bool value);
    Error SetNumber(double value);
    Error SetNull();

    // Getter
    Type GetType() const { return type; }
    bool GetBool() const { return boolean; }
    double GetNumber() const { return number; }
    std::string GetString() const { return str; }
    std::map<std::string, Value> GetObject() const { return obj; }
    std::vector<Value> GetArray() const { return ary; }

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
    Value ParseBool(bool setValue, std::string expect);
    Value ParseNumber(char firstVal);
    Value ParseNull();
    Error Print(Value data, std::string indent);
    bool IsNumber(char c);
    std::string GetPrevIndent(std::string indent);

  public:
    Parser() {}
    ~Parser() {}

    Error ParseFile(std::string fileName);
    Error ParseString(std::string str);
    Error Print();
    Value Get();
};
}; // namespace json