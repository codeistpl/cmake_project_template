#ifndef CMD_PARSER_PARSE_ERROR_H
#define CMD_PARSER_PARSE_ERROR_H

#include <stdexcept>
#include <string>

namespace cmd_parser {

/**
 * Exception thrown when parsing fails
 */
class ParseError : public std::runtime_error {
  public:
    explicit ParseError(const std::string &message)
        : std::runtime_error(message) {}
};

} // namespace cmd_parser

#endif // CMD_PARSER_PARSE_ERROR_H
