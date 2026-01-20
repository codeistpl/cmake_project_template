#ifndef CMD_PARSER_ARGUMENT_PARSER_H
#define CMD_PARSER_ARGUMENT_PARSER_H

#include "argument.h"
#include "parsed_args.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cmd_parser {

namespace detail {
class ArgumentParserImpl;
}

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
    ~ArgumentParser();

    // Disable copy, enable move
    ArgumentParser(const ArgumentParser &) = delete;
    auto operator=(const ArgumentParser &) -> ArgumentParser & = delete;
    ArgumentParser(ArgumentParser &&) noexcept;
    auto operator=(ArgumentParser &&) noexcept -> ArgumentParser &;

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
    std::unique_ptr<detail::ArgumentParserImpl> pimpl_;
};

} // namespace cmd_parser

#endif // CMD_PARSER_ARGUMENT_PARSER_H
