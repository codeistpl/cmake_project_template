### This is a template project c++ project using cmake gtest and gmock, cmake, and conan

### build instructions
```
$ conan install . --output-folder=build --build=missing
$ cd .build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```

### Coding Conventions

#### Code Style and Formatting

**Naming Conventions**
- **Classes and Types**: Use `PascalCase` (e.g., `BraceChecker`, `MyClass`)
- **Functions and Methods**: Use `snake_case` (e.g., `is_valid()`, `check_braces()`)
- **Variables**: Use `snake_case` (e.g., `input_string`, `max_count`)
- **Constants**: Use `UPPER_SNAKE_CASE` (e.g., `MAX_SIZE`, `DEFAULT_TIMEOUT`)
- **Priate Members**: Use `snake_case_with_floor_` (e.g., `internal_buffer_`, `count_`)
- **File Names**: Use `snake_case` (e.g., `brace_checker.cpp`, `brace_checker.h`)

- **Avoid prefixes and postfixes**
- **Use Short names in local scope**
- **Use Long descriptive names in global scope**
- **Avoid meta names like \"manager\"**
- **Use only commonly agreed shortcuts**

- **Use trailing type function notation**

**Commonly Used Abbreviations**

The following abbreviations are approved for use in this project:

| Abbreviation | Meaning           | Example          |
|--------------|-------------------|------------------|
| `abs`        | abstract          | `abs_value`      |
| `arg`        | argument          | `func_arg`       |
| `arr`        | array             | `char_arr`       |
| `buf`        | buffer            | `read_buf`       |
| `cfg`        | configuration     | `app_cfg`        |
| `char`       | character         | `char_code`      |
| `cmd`        | command           | `user_cmd`       |
| `config`     | configuration     | `load_config`    |
| `const`      | constant          | *(keyword)*      |
| `ctx`        | context           | `request_ctx`    |
| `cur`        | current           | `cur_index`      |
| `del`        | delete            | `del_node`       |
| `dir`        | directory         | `file_dir`       |
| `dst`        | destination       | `copy_dst`       |
| `err`        | error             | `parse_err`      |
| `func`       | function          | `callback_func`  |
| `gbl`        | global            | `gbl_state`      |
| `id`         | identifier        | `user_id`        |
| `idx`        | index             | `array_idx`      |
| `init`       | initialize        | `init_module`    |
| `len`        | length            | `str_len`        |
| `lib`        | library           | `math_lib`       |
| `max`        | maximum           | `max_size`       |
| `min`        | minimum           | `min_value`      |
| `msg`        | message           | `status_msg`     |
| `num`        | number            | `item_num`       |
| `obj`        | object            | `data_obj`       |
| `param`      | parameter         | `func_param`     |
| `ptr`        | pointer           | `node_ptr`       |
| `ref`        | reference         | `obj_ref`        |
| `req`        | request           | `http_req`       |
| `res`        | result/response   | `query_res`      |
| `src`        | source            | `copy_src`       |
| `str`        | string            | `input_str`      |
| `tmp`        | temporary         | `tmp_buffer`     |
| `val`        | value             | `stored_val`     |
| `var`        | variable          | `local_var`      |

**Comments and Documentation**
- Use `//` for single-line comments
- Use `/* */` for multi-line comments
- Start comments with a space after `//` or `/*`
- Document public functions with their purpose, parameters, and return value
- Avoid obvious comments; focus on the "why" rather than the "what"

```cpp
/* Check if braces are balanced in the input string
   Returns true if all braces are properly paired and nested */
bool is_valid(const std::string& input);
```

#### Code Organization

**Headers and Includes**
- Include guards: `#pragma once` at the top of header files
- Group includes: standard library, third-party libraries, local headers
- Separate groups with blank lines

```cpp
#pragma once

#include <string>
#include <vector>

#include "brace_checker.h"
```

**Class Structure**
- Order: public, protected, private
- Group methods by functionality
- Member variables should be private or protected

```cpp
class MyClass {
public:
    MyClass();
    void public_method();

private:
    void private_helper();
    int member_variable;
};
```

#### Best Practices

- Keep functions focused and single-purpose
- Use const references for parameters when appropriate
- Avoid global variables
- Handle errors explicitly (don't silently fail)
- Write self-documenting code; names should be clear and meaningful
- Use modern C++ features (prefer `std::string` over `char*`, etc.)

#### Tools

This project uses automated tools to enforce conventions:
- `lint.sh`: Runs static analysis and style checks
- `format.sh`: Auto-formats code to meet style guidelines
- `test.sh`: Runs unit tests to verify correctness
```

### Coding Conventions
