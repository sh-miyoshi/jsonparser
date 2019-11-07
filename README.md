# [WIP] JSON parser

## Overview

This is a library to parse JSON for C++.  
Currently, Work In Progress...

## Usage

```bash
make
./a.out
```

## TODO List

- correspond to Shift-JIS
- add test
- add other JSON type(number, null, boolean)
- correspond escape sequence
- add useful method
  - Example

  ```c++
  json = Parse(data)
  str = json["key"][0].get()
  ```
