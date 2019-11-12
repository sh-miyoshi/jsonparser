#include "../jsonparser.h"
#include <iostream>

int main() {
    // Parse JSON data
    json::Parser parser;
    json::Error err = parser.ParseFile("sample.json");
    if (!err.success) {
        std::cout << "Failed to parse json: " << err.message << std::endl;
        return 1;
    }
    parser.Print();

    //json::Value result = parser.GetString();
    //std::cout << result["projects"][1]["projectID"].Get() << std::endl;
}
