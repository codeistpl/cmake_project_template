#include "argument_parser.h"
#include "detail/argument_parser_impl.h"
#include "parse_error.h"
#include <iostream>
#include <sstream>
#include <utility>

namespace cmd_parser {

// ============================================================================
// ArgumentParser Public Interface
// ============================================================================

ArgumentParser::ArgumentParser(std::string program_name,
                               std::string description)
    : pimpl_(std::make_unique<detail::ArgumentParserImpl>(
          std::move(program_name), std::move(description))) {
    // Add default help argument
    add_flag("-h", "--help", "Show this help message and exit");
}

ArgumentParser::~ArgumentParser() = default;

ArgumentParser::ArgumentParser(ArgumentParser &&) noexcept = default;

auto ArgumentParser::operator=(ArgumentParser &&) noexcept -> ArgumentParser & =
                                                                  default;

auto ArgumentParser::add_argument(const std::string &name,
                                  const std::string &help_text) -> Argument & {
    auto arg = std::make_shared<Argument>(name, Argument::Type::POSITIONAL);
    arg->help(help_text);
    pimpl_->register_argument(arg);
    return *arg;
}

auto ArgumentParser::add_argument(const std::string &short_name,
                                  const std::string &long_name,
                                  const std::string &help_text) -> Argument & {
    auto arg = std::make_shared<Argument>(short_name, Argument::Type::OPTIONAL);
    arg->short_name_ = short_name;
    arg->long_name_ = long_name;
    arg->help(help_text);

    // Use long name without dashes as the key
    std::string key = long_name;
    if (key.size() >= 2 && key.substr(0, 2) == "--") {
        key = key.substr(2);
    }
    arg->name_ = key;

    pimpl_->register_argument(arg);
    return *arg;
}

auto ArgumentParser::add_flag(const std::string &short_name,
                              const std::string &long_name,
                              const std::string &help_text) -> Argument & {
    auto arg = std::make_shared<Argument>(short_name, Argument::Type::FLAG);
    arg->short_name_ = short_name;
    arg->long_name_ = long_name;
    arg->help(help_text);

    // Use long name without dashes as the key
    std::string key = long_name;
    if (key.size() >= 2 && key.substr(0, 2) == "--") {
        key = key.substr(2);
    }
    arg->name_ = key;

    pimpl_->register_argument(arg);
    return *arg;
}

auto ArgumentParser::parse_args(const std::vector<std::string> &args)
    -> ParsedArgs {
    ParsedArgs result;

    // Check for help flag
    for (const auto &arg : args) {
        if (arg == "-h" || arg == "--help") {
            print_help();
            result.status_ = ParseStatus::HELP_REQUESTED;
            return result;
        }
    }

    auto positional_args = pimpl_->collect_positional_arguments();
    size_t positional_index = 0;

    // Parse arguments
    for (size_t i = 0; i < args.size(); ++i) {
        const std::string &arg = args[i];

        if (arg.empty()) {
            continue;
        }

        if (arg[0] == '-') {
            pimpl_->process_optional_or_flag(arg, args, i, result);
        } else {
            detail::ArgumentParserImpl::process_positional_argument(
                arg, positional_args, positional_index, result);
        }
    }

    pimpl_->apply_defaults_and_validate(result);
    return result;
}

auto ArgumentParser::print_help() const -> void { std::cout << get_help(); }

auto ArgumentParser::get_help() const -> std::string {
    std::ostringstream oss;

    oss << pimpl_->build_usage_line();

    if (!pimpl_->description.empty()) {
        oss << "\n" << pimpl_->description << "\n";
    }

    oss << pimpl_->build_positional_section();
    oss << pimpl_->build_optional_section();

    return oss.str();
}

} // namespace cmd_parser
