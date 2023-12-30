#include "brace_checker.h"
#include <algorithm>
#include <stack>
#include <vector>

namespace algorithms {
bool is_opening_brace(const char &character) {
    std::vector<char> opening_braces = {'{', '[', '('};
    return (std::end(opening_braces) != std::find(std::begin(opening_braces),
                                                  std::end(opening_braces),
                                                  character));
}
bool is_closing_brace(const char &character) {
    std::vector<char> closing_braces = {'}', ']', ')'};
    return (std::end(closing_braces) != std::find(std::begin(closing_braces),
                                                  std::end(closing_braces),
                                                  character));
}
bool is_open_close_brace_pair(const char &opening, const char &closing) {
    if (opening == '{' && closing == '}') {
        return true;
    }
    if (opening == '[' && closing == ']') {
        return true;
    }
    if (opening == '(' && closing == ')') {
        return true;
    }
    return false;
}
bool check_brace_correctness(const std::string &str) {
    std::stack<char> braces;
    for (char character : str) {
        if (is_opening_brace(character)) {
            braces.push(character);
        }
        if (is_closing_brace(character)) {
            if (braces.empty()) {
                return false;
            }
            if (!is_open_close_brace_pair(braces.top(), character)) {
                return false;
            }
            braces.pop();
        }
    }
    return braces.empty();
}
} // namespace algorithms
