#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "jsonparser.h"

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
    std::shared_ptr<json::Value> result = parser.Get();

    parser.Print();
    puts("");
    //std::cout<<result->GetObject()["projects"]->GetArray()[1]->GetObject()["projectID"]->GetString()<<std::endl;
}
