# JSON parser

![Actions Status](https://github.com/sh-miyoshi/jsonparser/workflows/C/C++%20CI/badge.svg)

## Overview

This is a library to parse JSON for C++.  

## Usage

1. copy jsonparser.cpp and jsonparser.h to your project
2. use in your project like this

```c++
#include "jsonparser.h"
#include <iostream>

int main() {
    // Parse JSON file
    json::Parser parser;
    json::Error err = parser.ParseFile("sample.json");
    if (!err.success) {
        std::cout << "Failed to parse json: " << err.message << std::endl;
        return 1;
    }

    // Get data
    json::Value result = parser.Get();
    std::string value = result["projects"][1]["projectID"].GetString();
}

```

## Examples

TODO(more examples are in ./examples)

## TODO List

- correspond to Shift-JIS
- add more test case
- correspond escape sequence
- update error messages
