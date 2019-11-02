#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

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
    std::map<std::string, Value *> obj;
    std::vector<Value *> ary;

  public:
    Value() : type(eTYPE_NULL) {}
    ~Value() {}

    void SetObject(std::string key, Value *value) {
        if (type == eTYPE_NULL) {
            type = eTYPE_OBJECT;
            obj.insert(std::make_pair(key, value));
        } else {
            // TODO(set error)
        }
    }
    void SetArray(Value *value) {
        if (type == eTYPE_NULL) {
            type = eTYPE_ARRAY;
            ary.push_back(value);
        } else {
            // TODO(set error)
        }
    }
    void SetString(std::string value) {
        if (type == eTYPE_NULL) {
            type = eTYPE_STRING;
            str = value;
        } else {
            // TODO(set error)
        }
    }

    // TODO(Get)
};

Value *parse_value(std::string *str);
Value *parse_array(std::string *str);
Value *parse_object(std::string *str);
Value *parse_string(std::string *str);

void remove(std::string *str) {
    while ((*str)[0] == ' ' || (*str)[0] == '\n' || (*str)[0] == '\t' || (*str)[0] == '\r') {
        (*str).erase(str->begin());
    }
}

char first_char(std::string *str) {
    remove(str);
    if (str->empty()) {
        // TODO(set json parse error)
        puts("failed to parse json in first_char");
        exit(1);
    }
    char res = (*str)[0];
    (*str).erase(str->begin());
    return res;
}

Value *parse_array(std::string *str) {
    Value *res = new Value();
    while (!str->empty()) {
        char c = first_char(str);
        switch (c) {
        case '{':
            res->SetArray(parse_object(str));
            break;
        case ',':
            break;
        case '"':
            res->SetArray(parse_string(str));
            break;
        case ']':
            return res;
        default:
            puts("maybe mistake");
            break;
        }
    }
    // TODO(set json parse error)
    puts("failed to parse json in parse_array");
    exit(1);
    return nullptr;
}

Value *parse_object(std::string *str) {
    Value *res = new Value();
    std::string key;
    while (!str->empty()) {
        char c = first_char(str);
        switch (c) {
        case '{':
        case '[':
            puts("maybe mistake");
            break;
        case ':':
            res->SetObject(key, parse_value(str));
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
    // TODO(set json parse error)
    puts("failed to parse json in parse_object");
    exit(1);
    return nullptr;
}

Value *parse_string(std::string *str) {
    std::string val;
    char c;
    while ((c = first_char(str)) != '"') {
        val += c;
    }
    Value *res = new Value();
    res->SetString(val);
    return res;
}

Value *parse_value(std::string *str) {
    char c = first_char(str);
    switch (c) {
    case '{':
        return parse_object(str);
    case '"':
        return parse_string(str);
    case '[':
        return parse_array(str);
    default:
        // TODO(set json parse error)
        puts("failed to parse json in parse_value");
        exit(1);
        return nullptr;
    }
}

int main() {
    std::string input;
    FILE *fp = fopen("sample.json", "r");
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        input += (char)ch;
    }
    fclose(fp);

    //puts(input.c_str());
    Value result;
    parse_value(&input);
}
