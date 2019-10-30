#include <map>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>

union json {
    std::string str;
    std::map<std::string, union json> obj;
    std::vector<union json> ary;

    json(){}
    ~json(){}
};

void parse_value(union json *result, std::string *str);
void parse_array(union json *result, std::string *str);
void parse_object(union json *result, std::string *str);
void parse_string(union json *result, std::string *str);

void remove(std::string *str) {
    while ((*str)[0] == ' ' || (*str)[0] == '\n' || (*str)[0] == '\t' || (*str)[0] == '\r') {
        (*str).erase(str->begin());
    }
}

char first_char(std::string *str) {
    remove(str);
    if(str->empty()){
        // TODO(set json parse error)
        puts("failed to parse json in first_char");
        exit(1);
    }
    char res = (*str)[0];
    (*str).erase(str->begin());
    return res;
}

void parse_array(union json *result, std::string *str) {
    int index = 0;
    while (!str->empty()) {
        char c = first_char(str);
        switch (c) {
        case '{':
            printf("array value %d:\n", index);
            parse_object(result, str);
            break;
        case ',':
            index++;
            break;
        case '"':
            printf("array value %d:\n", index);
            parse_string(result, str);
            break;
        case ']':
            return;
        default:
            puts("maybe mistake");
            break;
        }
    }
}

void parse_object(union json *result, std::string *str) {
    std::string key;
    while (!str->empty()) {
        char c = first_char(str);
        switch (c) {
        case '{':
            parse_object(result, str);
            break;
        case '[':
            parse_array(result, str);
            break;
        case ':':
            printf("key: %s,\n", key.c_str());
            parse_value(result, str);
            break;
        case ',':
            key = "";
            break;
        case '"':
            break;
        case '}':
            return;
        default:
            key += c;
            break;
        }
    }
}

void parse_string(union json *result, std::string *str) {
    std::string res;
    remove(str);
    while ((*str)[0] != '"') {
        res += (*str)[0];
        (*str).erase(str->begin());
    }
    printf("value: %s\n", res.c_str());
    (*str).erase(str->begin());
}

void parse_value(union json *result, std::string *str) {
    char c = first_char(str);
    switch (c) {
    case '{':
        parse_object(result, str);
        break;
    case '"':
        parse_string(result, str);
        break;
    case '[':
        parse_array(result, str);
        break;
    default:
        // TODO(set json parse error)
        puts("failed to parse json");
        exit(1);
        break;
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
    union json result; 
    parse_value(&result, &input);
}
