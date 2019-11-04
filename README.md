# [WIP] JSON parser

## Overview

This is a library to parse JSON for C++.

## Usage

```bash
make
./a.out
```

## TODO List

- create Parse class
- split files from main.cpp
- correspond to Shift-JIS
- add test
- add useful method

  ```c++
  json = Parse(data)
  str = json["key"][0].get()
  ```
