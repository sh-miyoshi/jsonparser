#include "jsonparser.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

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
    json::Parser parser;
    json::Error err = parser.ParseString(input);
    // TODO(error handling)
    json::Value result = parser.Get();

    parser.Print();
    //std::cout << result["projects"][1]["projectID"].Get() << std::endl;
    puts("");
}
