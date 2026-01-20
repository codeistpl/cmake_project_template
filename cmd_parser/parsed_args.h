#ifndef CMD_PARSER_PARSED_ARGS_H
#define CMD_PARSER_PARSED_ARGS_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace cmd_parser {

namespace detail {
class ArgumentParserImpl;
}

enum class ParseStatus : uint8_t { OK = 0, HELP_REQUESTED };

/**
 * Container for parsed command-line arguments
 */
class ParsedArgs {
  public:
    [[nodiscard]] auto get(const std::string &name) const -> std::string;
    [[nodiscard]] auto get_flag(const std::string &name) const -> bool;
    [[nodiscard]] auto has(const std::string &name) const -> bool;
    [[nodiscard]] auto
    get_list(const std::string &name) const -> std::vector<std::string>;
    [[nodiscard]] auto status() const -> ParseStatus { return status_; }

  private:
    friend class ArgumentParser;
    friend class detail::ArgumentParserImpl;

    std::map<std::string, std::string> values_;
    std::map<std::string, bool> flags_;
    std::map<std::string, std::vector<std::string>> lists_;
    ParseStatus status_ = ParseStatus::OK;
};

} // namespace cmd_parser

#endif // CMD_PARSER_PARSED_ARGS_H
