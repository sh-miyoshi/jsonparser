#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class Value {
  public:
    std::string str;
    std::map<std::string, Value *> obj;
    std::vector<Value *> ary;

    Value() {}
    ~Value() {}
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
    int index = 0;
    while (!str->empty()) {
        char c = first_char(str);
        switch (c) {
        case '{':
            printf("array value %d:\n", index);
            parse_object(str);
            break;
        case ',':
            index++;
            break;
        case '"':
            printf("array value %d:\n", index);
            parse_string(str);
            break;
        case ']':
            puts("parse_array is not implemented yet");
            return nullptr;
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
            parse_object(str);
            break;
        case '[':
            parse_array(str);
            break;
        case ':':
            printf("key: %s,\n", key.c_str());
            res->obj = std::map<std::string, Value *>();
            res->obj.insert(std::make_pair(key, parse_value(str)));
            break;
        case ',':
            key = "";
            break;
        case '"':
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
    Value *res = new Value();
    remove(str);
    while ((*str)[0] != '"') {
        res->str += (*str)[0];
        (*str).erase(str->begin());
    }
    printf("value: %s\n", res->str.c_str());
    (*str).erase(str->begin());
    return res;
}

Value *parse_value(std::string *str) {
    char c = first_char(str);
    printf("val: %c\n", c);
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
