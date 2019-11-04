#include <iostream>
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

    // Setter
    void SetObject(std::string key, Value *value) {
        if (type == eTYPE_NULL || type == eTYPE_OBJECT) {
            type = eTYPE_OBJECT;
            obj.insert(std::make_pair(key, value));
        } else {
            // TODO(set error)
        }
    }
    void SetArray(Value *value) {
        if (type == eTYPE_NULL || type == eTYPE_ARRAY) {
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

    // Getter
    Type GetType() const {
        return type;
    }

    std::string GetString() {
        // TODO(check type)
        return str;
    }
    std::map<std::string, Value *> GetObject() {
        // TODO(check type)
        return obj;
    }
    std::vector<Value *> GetArray() {
        // TODO(check type)
        return ary;
    }
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

void print(Value data) {
    switch (data.GetType()) {
    case Value::eTYPE_OBJECT: {
        std::cout << "{";
        auto obj = data.GetObject();
        unsigned int i = 0, size = obj.size();
        for (auto it = obj.begin(); it != obj.end(); it++) {
            std::cout << "\"" << it->first << "\":";
            print(*it->second);
            if (i < size - 1) {
                printf(",");
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
                print(*ary[i]);
                printf(",");
            }
        }
        if (size > 0) {
            print(*ary[size - 1]);
        }

        std::cout << "]";
        break;
    }
    case Value::eTYPE_STRING:
        std::cout << "\"" << data.GetString() << "\"";
        break;
    default:
        // TODO(set error or not implemented)
        printf("failed to unknown value type: %d\n", data.GetType());
        exit(1);
        break;
    }
}

int main() {
    // Read JSON File
    std::string input;
    FILE *fp = fopen("sample.json", "r");
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        input += (char)ch;
    }
    fclose(fp);
    //puts(input.c_str());

    // Parse JSON data
    Value *result = parse_value(&input);

    print(*result);
    puts("");
    //std::cout<<result->GetObject()["projects"]->GetArray()[1]->GetObject()["projectID"]->GetString()<<std::endl;
}
