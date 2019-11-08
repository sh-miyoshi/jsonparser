#include "jsonparser.h"
#include <iostream>
using namespace json;

Error Value::SetObject(std::string key, Value value) {
    if (type == eTYPE_NULL || type == eTYPE_OBJECT) {
        type = eTYPE_OBJECT;
        obj.insert(std::make_pair(key, value));
    } else {
        Error err;
        err.success = false;
        err.message = "Failed to set object due to unexpected type";
        return err;
    }
    return Error(); // return success
}

Error Value::SetArray(Value value) {
    if (type == eTYPE_NULL || type == eTYPE_ARRAY) {
        type = eTYPE_ARRAY;
        ary.push_back(value);
    } else {
        Error err;
        err.success = false;
        err.message = "Failed to set array due to unexpected type";
        return err;
    }
    return Error(); // return success
}

Error Value::SetString(std::string value) {
    if (type == eTYPE_NULL) {
        type = eTYPE_STRING;
        str = value;
    } else {
        Error err;
        err.success = false;
        err.message = "Failed to set string due to unexpected type";
        return err;
    }
    return Error(); // return success
}

Error Value::SetBool(bool value){
    if (type == eTYPE_NULL) {
        type = eTYPE_BOOL;
        boolean = value;
    } else {
        Error err;
        err.success = false;
        err.message = "Failed to set bool due to unexpected type";
        return err;
    }
    return Error(); // return success
}

void Parser::Remove() {
    while (!data.empty() && (data[0] == ' ' || data[0] == '\n' || data[0] == '\t' || data[0] == '\r')) {
        data.erase(data.begin());
    }
}

char Parser::FirstChar() {
    Remove();
    if (data.empty()) {
        this->err.success = false;
        this->err.message = "failed to parse json in first_char";
        return ' '; // return dummy data
    }
    char res = data[0];
    data.erase(data.begin()); // remove read value
    return res;
}

Value Parser::ParseValue() {
    char c = FirstChar();
    switch (c) {
    case '{':
        return ParseObject();
    case '"':
        return ParseString();
    case '[':
        return ParseArray();
    case 't':
        return ParseBool(true, "rue");// rue = "true" - 't'
    case 'f':
        return ParseBool(false, "alse");// alse = "false" - 'f'
    default:
        this->err.success = false;
        this->err.message = "Failed to parse value: find unexpected charactor";
        return Value();
    }
}

Value Parser::ParseArray() {
    Value res;
    while (!data.empty()) {
        char c = FirstChar();
        switch (c) {
        case '{':
            res.SetArray(ParseObject());
            break;
        case ',':
            break;
        case '"':
            res.SetArray(ParseString());
            break;
        case ']':
            return res;
        default:
            this->err.success = false;
            this->err.message = "Failed to parse array: find unexpected charactor";
            return res;
        }
    }

    this->err.success = false;
    this->err.message = "Failed to parse array: got EOF before closing array";
    return res;
}

Value Parser::ParseObject() {
    Value res;
    std::string key;
    while (!data.empty()) {
        char c = FirstChar();
        switch (c) {
        case '{':
        case '[':
            this->err.success = false;
            this->err.message = "Failed to parse object: find unexpected charactor";
            return res;
        case ':':
            res.SetObject(key, ParseValue());
            break;
        case ',':
            key = "";
            break;
        case '"': // begin or end of key
            break;
        case '}':
            return res;
        default:
            key += c;
            break;
        }
    }

    this->err.success = false;
    this->err.message = "Failed to parse object: got EOF before closing object";
    return res;
}

Value Parser::ParseString() {
    std::string val;
    char c;
    while ((c = FirstChar()) != '"') {
        val += c;
    }
    Value res;
    res.SetString(val);
    return res;
}

Value Parser::ParseBool(bool setValue, std::string expect) {
    Value res;
    std::string t;
    for(unsigned int i=0;i<expect.length();i++){
        t += FirstChar();
    }
    if( t == expect){
        res.SetBool(setValue);
    }else{
        this->err.success = false;
        this->err.message = "Failed to parse bool: cannot get true or false";
    }
    return res;
}

Error Parser::Print(Value data) {
    switch (data.GetType()) {
    case Value::eTYPE_OBJECT: {
        std::cout << "{";
        auto obj = data.GetObject();
        unsigned int i = 0, size = obj.size();
        for (auto it = obj.begin(); it != obj.end(); it++) {
            std::cout << "\"" << it->first << "\":";
            Print(it->second);
            if (i < size - 1) {
                std::cout << ",";
            }
            i++;
        }
        std::cout << "}";
        break;
    }
    case Value::eTYPE_ARRAY: {
        auto ary = data.GetArray();
        unsigned int size = ary.size();
        std::cout << "[";
        if (size > 1) {
            for (unsigned int i = 0; i < size - 1; i++) {
                Print(ary[i]);
                std::cout << ",";
            }
        }
        if (size > 0) {
            Print(ary[size - 1]);
        }

        std::cout << "]";
        break;
    }
    case Value::eTYPE_STRING:
        std::cout << "\"" << data.GetString() << "\"";
        break;
    case Value::eTYPE_BOOL:{
        std::string output = data.GetBool() ? "true" : "false";
        std::cout << output;
        break;
    }
    default:
        err.success = false;
        err.message = "Failed to print JSON: got unknown value type";
        return err;
    }
    return err;
}

Error Parser::ParseFile(std::string fileName) {
    std::string input;
    FILE *fp = fopen(fileName.c_str(), "r");
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        input += (char)ch;
    }
    fclose(fp);
    return ParseString(input);
}

Error Parser::ParseString(std::string str) {
    this->data = str;
    this->result = ParseValue();
    return err;
}

Error Parser::Print() {
    return Print(result);
}

Value Parser::Get() {
    return result;
}