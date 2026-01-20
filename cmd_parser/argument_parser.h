#ifndef CMD_PARSER_ARGUMENT_PARSER_H
#define CMD_PARSER_ARGUMENT_PARSER_H

#include "argument.h"
#include "parsed_args.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cmd_parser {

/**
 * Python-style command-line argument parser
 *
 * Example usage:
 *   ArgumentParser parser("my_program", "Description of my program");
 *   parser.add_argument("input", "Input file path");
 *   parser.add_argument("-o", "--output", "Output file
 * path").default_value("output.txt"); parser.add_argument("-v", "--verbose",
 * "Verbose output").flag();
 *
 *   auto args = parser.parse_args(argc, argv);
 *   std::string input = args.get("input");
 *   bool verbose = args.get_flag("verbose");
 */
class ArgumentParser {
  public:
    ArgumentParser(std::string program_name, std::string description = "");

    // Add a positional argument
    auto add_argument(const std::string &name,
                      const std::string &help_text = "") -> Argument &;

    // Add an optional argument with short and/or long form
    auto add_argument(const std::string &short_name,
                      const std::string &long_name,
                      const std::string &help_text) -> Argument &;

    // Add a flag (boolean option)
    auto add_flag(const std::string &short_name, const std::string &long_name,
                  const std::string &help_text = "") -> Argument &;

    // Parse command-line arguments
    auto parse_args(const std::vector<std::string> &args) -> ParsedArgs;

    // Print help message
    auto print_help() const -> void;
    [[nodiscard]] auto get_help() const -> std::string;

  private:
    std::string program_name;
    std::string description;
    std::vector<std::shared_ptr<Argument>> arguments;
    std::map<std::string, std::shared_ptr<Argument>> arg_map;

    auto register_argument(const std::shared_ptr<Argument> &arg) -> void;
    [[nodiscard]] auto
    find_argument(const std::string &name) const -> std::shared_ptr<Argument>;

    static auto validate_choices(const Argument &arg,
                                 const std::string &value) -> void;

    // Helper methods for get_help()
    [[nodiscard]] auto build_usage_line() const -> std::string;
    [[nodiscard]] auto build_positional_section() const -> std::string;
    [[nodiscard]] auto build_optional_section() const -> std::string;
    static auto
    format_choices(const std::vector<std::string> &choices) -> std::string;

    // Helper methods for parse_args()
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

} // namespace cmd_parser

#endif // CMD_PARSER_ARGUMENT_PARSER_H
