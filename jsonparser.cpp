#include "jsonparser.h"
#include <iostream>
using namespace json;

namespace {
const std::string INDENT = "  ";
};

/*
TODO(SJIS method)
int GetSJISByteSize(char str){
	unsigned char bstr = ( unsigned char ) str;
	if( ( 0x81 <= bstr&&bstr <= 0x9f ) || ( 0xE0 <= bstr&&bstr <= 0xff ) ){
		return 2;// full width
	} else
		return 1;// half width
}
*/

Error Value::SetObject(std::string key, Value value) {
    if (type == eTYPE_NONE || type == eTYPE_OBJECT) {
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
    if (type == eTYPE_NONE || type == eTYPE_ARRAY) {
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
    if (type == eTYPE_NONE) {
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

Error Value::SetBool(bool value) {
    if (type == eTYPE_NONE) {
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

Error Value::SetNumber(double value) {
    if (type == eTYPE_NONE) {
        type = eTYPE_NUMBER;
        number = value;
    } else {
        Error err;
        err.success = false;
        err.message = "Failed to set number due to unexpected type";
        return err;
    }
    return Error(); // return success
}

Error Value::SetNull() {
    if (type == eTYPE_NONE) {
        type = eTYPE_NULL;
        isNull = true;
    } else {
        Error err;
        err.success = false;
        err.message = "Failed to set null due to unexpected type";
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
        return ParseBool(true, "rue"); // rue = "true" - 't'
    case 'f':
        return ParseBool(false, "alse"); // alse = "false" - 'f'
    case 'n':
        return ParseNull();
    case '+':
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return ParseNumber(c);
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
    for (unsigned int i = 0; i < expect.length(); i++) {
        t += FirstChar();
    }
    if (t == expect) {
        res.SetBool(setValue);
    } else {
        this->err.success = false;
        this->err.message = "Failed to parse bool: cannot get true or false";
    }

    return res;
}

Value Parser::ParseNumber(char firstVal) {
    Value res;
    std::string value;
    value += firstVal;
    while (!data.empty() && IsNumber(this->data[0])) {
        value += this->data[0];
        this->data.erase(this->data.begin()); // remove read value
    }
    res.SetNumber(std::stod(value));
    return res;
}

Value Parser::ParseNull() {
    Value res;
    std::string t;
    const std::string expect = "ull"; // ull = "null" - 'n'
    for (unsigned int i = 0; i < expect.length(); i++) {
        t += FirstChar();
    }
    if (t == expect) {
        res.SetNull();
    } else {
        this->err.success = false;
        this->err.message = "Failed to parse null: got unexpected charactor";
    }

    return res;
}

Error Parser::Print(Value data, std::string indent) {
    switch (data.GetType()) {
    case Value::eTYPE_OBJECT: {
        indent += INDENT;
        std::cout << "{\n"
                  << indent;
        auto obj = data.GetObject();
        unsigned int i = 0, size = obj.size();
        for (auto it = obj.begin(); it != obj.end(); it++) {
            std::cout << "\"" << it->first << "\": ";
            Print(it->second, indent);
            if (i < size - 1) {
                std::cout << ",\n"
                          << indent;
            } else {
                std::cout << "\n"
                          << GetPrevIndent(indent);
            }
            i++;
        }
        std::cout << "}";
        break;
    }
    case Value::eTYPE_ARRAY: {
        auto ary = data.GetArray();
        unsigned int size = ary.size();
        indent += INDENT;
        std::cout << "[\n"
                  << indent;
        if (size > 1) {
            for (unsigned int i = 0; i < size - 1; i++) {
                Print(ary[i], indent);
                std::cout << ",\n"
                          << indent;
            }
        }
        if (size > 0) {
            Print(ary[size - 1], indent);
            std::cout << "\n"
                      << GetPrevIndent(indent);
        }

        std::cout << "]" << indent;
        break;
    }
    case Value::eTYPE_STRING:
        std::cout << "\"" << data.GetString() << "\"";
        break;
    case Value::eTYPE_BOOL: {
        std::string output = data.GetBool() ? "true" : "false";
        std::cout << output;
        break;
    }
    case Value::eTYPE_NUMBER:
        std::cout << data.GetNumber();
        break;
    case Value::eTYPE_NULL:
        std::cout << "null";
        break;
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
    return Print(result, "");
}

Value Parser::Get() {
    return result;
}

bool Parser::IsNumber(char c) {
    // return true when c is in numberData
    const std::string numberData = "0123456789+-.eE";
    for (auto n : numberData) {
        if (n == c) {
            return true;
        }
    }
    return false;
}

std::string Parser::GetPrevIndent(std::string indent) {
    for (unsigned int i = 0; i < INDENT.size(); i++) {
        if (indent[0] == INDENT[i]) {
            indent.erase(indent.begin());
        } else {
            std::cerr << "Program Error: indent include unexpected charactor" << std::endl;
            exit(1);
        }
    }
    return indent;
}