#include <map>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>

/*union json {
    std::string str;
    std::map<std::string, union json> obj;
    std::vector<union json> ary;

    json() {}
};*/

void parse_value(std::string *str);
void parse_array(std::string *str);
void parse_object(std::string *str);
void parse_string(std::string *str);

void remove(std::string *str) {
    while ((*str)[0] == ' ' || (*str)[0] == '\n' || (*str)[0] == '\t' || (*str)[0] == '\r') {
        (*str).erase(str->begin());
    }
}

void parse_array(std::string *str) {
    std::string val;
    int index = 0;
    while (!str->empty()) {
        remove(str);
        switch ((*str)[0]) {
        case '{':
            (*str).erase(str->begin());
            parse_object(str);
            break;
        case ',':
            printf("value%d: %s\n", index, val.c_str());
            val = "";
            index++;
            break;
        case '"':
            break;
        case ']':
            printf("value%d: %s\n", index, val.c_str());
            return;
        default:
            val += (*str)[0];
            break;
        }
        (*str).erase(str->begin());
    }
}

void parse_object(std::string *str) {
    std::string key;
    while (!str->empty()) {
        remove(str);
        switch ((*str)[0]) {
        case '{':
            (*str).erase(str->begin());
            parse_object(str);
            break;
        case '[':
            (*str).erase(str->begin());
            parse_array(str);
            break;
        case ':':
            printf("key: %s, ", key.c_str());
            (*str).erase(str->begin());
            parse_value(str);
            break;
        case ',':
            key = "";
            break;
        case '"':
            break;
        case '}':
            return;
        default:
            key += (*str)[0];
            break;
        }
        (*str).erase(str->begin());
    }
}

void parse_string(std::string *str) {
    std::string res;
    remove(str);
    while ((*str)[0] != '"') {
        res += (*str)[0];
        (*str).erase(str->begin());
    }
    printf("value: %s\n", res.c_str());
}

void parse_value(std::string *str) {
    remove(str);
    switch ((*str)[0]) {
    case '{':
        (*str).erase(str->begin());
        parse_object(str);
        break;
    case '"':
        (*str).erase(str->begin());
        parse_string(str);
        break;
    case '[':
        (*str).erase(str->begin());
        parse_array(str);
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
    parse_value(&input);
}
