#ifndef CMD_PARSER_DETAIL_ARGUMENT_PARSER_IMPL_H
#define CMD_PARSER_DETAIL_ARGUMENT_PARSER_IMPL_H

#include "../argument.h"
#include "../parsed_args.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cmd_parser {

// Forward declaration
class ArgumentParser;

namespace detail {

/**
 * Private implementation class for ArgumentParser (PIMPL idiom)
 */
class ArgumentParserImpl {
  public:
    std::string program_name;
    std::string description;
    std::vector<std::shared_ptr<Argument>> arguments;
    std::map<std::string, std::shared_ptr<Argument>> arg_map;

    ArgumentParserImpl(std::string name, std::string desc);

    auto register_argument(const std::shared_ptr<Argument> &arg) -> void;
    [[nodiscard]] auto
    find_argument(const std::string &name) const -> std::shared_ptr<Argument>;
    static auto validate_choices(const Argument &arg,
                                 const std::string &value) -> void;

    [[nodiscard]] auto build_usage_line() const -> std::string;
    [[nodiscard]] auto build_positional_section() const -> std::string;
    [[nodiscard]] auto build_optional_section() const -> std::string;
    static auto
    format_choices(const std::vector<std::string> &choices) -> std::string;

    [[nodiscard]] auto collect_positional_arguments() const
        -> std::vector<std::shared_ptr<Argument>>;
    auto process_optional_or_flag(const std::string &arg,
                                  const std::vector<std::string> &args,
                                  size_t &size,
                                  ParsedArgs &result) const -> void;
    static auto process_positional_argument(
        const std::string &arg,
        const std::vector<std::shared_ptr<Argument>> &positional_args,
        size_t &positional_index, ParsedArgs &result) -> void;
    auto apply_defaults_and_validate(ParsedArgs &result) const -> void;
};

} // namespace detail
} // namespace cmd_parser

#endif // CMD_PARSER_DETAIL_ARGUMENT_PARSER_IMPL_H
