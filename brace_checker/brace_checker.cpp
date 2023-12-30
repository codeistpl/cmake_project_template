/**
 * @file brace_checker.cpp
 * @brief Implementation of a brace checker algorithm.
 */

#include "brace_checker.h"
#include <algorithm>
#include <stack>
#include <vector>

namespace algorithms {

/**
 * @brief Check if a character is an opening brace.
 * @param character The character to check.
 * @return True if the character is an opening brace, false otherwise.
 */
auto is_opening_brace(const char &character) -> bool {
    std::vector<char> opening_braces = {'{', '[', '('};
    return (std::end(opening_braces) != std::find(std::begin(opening_braces),
                                                  std::end(opening_braces),
                                                  character));
}

/**
 * @brief Check if a character is a closing brace.
 * @param character The character to check.
 * @return True if the character is a closing brace, false otherwise.
 */
auto is_closing_brace(const char &character) -> bool {
    std::vector<char> closing_braces = {'}', ']', ')'};
    return (std::end(closing_braces) != std::find(std::begin(closing_braces),
                                                  std::end(closing_braces),
                                                  character));
}

/**
 * @brief Check if a pair of characters form an open-close brace pair.
 * @param opening The opening brace character.
 * @param closing The closing brace character.
 * @return True if the characters form an open-close brace pair, false
 * otherwise.
 */
auto is_open_close_brace_pair(const char &opening, const char &closing)
    -> bool {
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

/**
 * @brief Check the correctness of braces in a string.
 * @param str The string to check.
 * @return True if the braces are correct, false otherwise.
 */
auto check_brace_correctness(const std::string &str) -> bool {
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
