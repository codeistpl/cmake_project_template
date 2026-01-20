#include "argument_parser.h"
#include "parse_error.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>

namespace cmd_parser {

// Constants
constexpr int HELP_COLUMN_WIDTH = 20;

ArgumentParser::ArgumentParser(std::string program_name_param,
                               std::string description_param)
    : program_name(std::move(program_name_param)),
      description(std::move(description_param)) {
    // Add default help argument
    add_flag("-h", "--help", "Show this help message and exit");
}

auto ArgumentParser::add_argument(const std::string &name,
                                  const std::string &help_text) -> Argument & {
    auto arg = std::make_shared<Argument>(name, Argument::Type::POSITIONAL);
    arg->help(help_text);
    register_argument(arg);
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

    register_argument(arg);
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

    register_argument(arg);
    return *arg;
}

auto ArgumentParser::register_argument(const std::shared_ptr<Argument> &arg)
    -> void {
    arguments.push_back(arg);

    // Register by name
    arg_map[arg->name_] = arg;

    // Register by short name
    if (!arg->short_name_.empty()) {
        arg_map[arg->short_name_] = arg;
    }

    // Register by long name
    if (!arg->long_name_.empty()) {
        arg_map[arg->long_name_] = arg;
    }
}

auto ArgumentParser::find_argument(const std::string &name) const
    -> std::shared_ptr<Argument> {
    auto found = arg_map.find(name);
    if (found != arg_map.end()) {
        return found->second;
    }
    return nullptr;
}

auto ArgumentParser::validate_choices(const Argument &arg,
                                      const std::string &value) -> void {
    if (arg.get_choices().empty()) {
        return;
    }

    const auto &choices = arg.get_choices();
    if (std::find(choices.begin(), choices.end(), value) == choices.end()) {
        std::ostringstream oss;
        oss << "Invalid value '" << value << "' for argument '" << arg.name()
            << "'. Valid choices are: ";
        for (size_t i = 0; i < choices.size(); ++i) {
            if (i > 0) {
                oss << ", ";
            }
            oss << "'" << choices[i] << "'";
        }
        throw ParseError(oss.str());
    }
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

    auto positional_args = collect_positional_arguments();
    size_t positional_index = 0;

    // Parse arguments
    for (size_t i = 0; i < args.size(); ++i) {
        const std::string &arg = args[i];

        if (arg.empty()) {
            continue;
        }

        if (arg[0] == '-') {
            process_optional_or_flag(arg, args, i, result);
        } else {
            process_positional_argument(arg, positional_args, positional_index,
                                        result);
        }
    }

    apply_defaults_and_validate(result);
    return result;
}

auto ArgumentParser::collect_positional_arguments() const
    -> std::vector<std::shared_ptr<Argument>> {
    std::vector<std::shared_ptr<Argument>> positional_args;
    for (const auto &arg : arguments) {
        if (arg->type() == Argument::Type::POSITIONAL) {
            positional_args.push_back(arg);
        }
    }
    return positional_args;
}

auto ArgumentParser::process_optional_or_flag(
    const std::string &arg, const std::vector<std::string> &args, size_t &size,
    ParsedArgs &result) const -> void {
    auto arg_def = find_argument(arg);
    if (!arg_def) {
        throw ParseError("Unknown argument: " + arg);
    }

    if (arg_def->type() == Argument::Type::FLAG) {
        result.flags_[arg_def->name()] = true;
    } else if (arg_def->type() == Argument::Type::OPTIONAL) {
        if (size + 1 >= args.size()) {
            throw ParseError("Argument '" + arg + "' requires a value");
        }
        const std::string &value = args[++size];
        validate_choices(*arg_def, value);
        result.values_[arg_def->name()] = value;
    }
}

auto ArgumentParser::process_positional_argument(
    const std::string &arg,
    const std::vector<std::shared_ptr<Argument>> &positional_args,
    size_t &positional_index, ParsedArgs &result) -> void {
    if (positional_index >= positional_args.size()) {
        throw ParseError("Too many positional arguments");
    }
    const auto &arg_def = positional_args[positional_index++];
    validate_choices(*arg_def, arg);
    result.values_[arg_def->name()] = arg;
}

auto ArgumentParser::apply_defaults_and_validate(ParsedArgs &result) const
    -> void {
    for (const auto &arg : arguments) {
        if (arg->type() == Argument::Type::FLAG) {
            if (result.flags_.find(arg->name()) == result.flags_.end()) {
                result.flags_[arg->name()] = false;
            }
        } else {
            bool has_value =
                result.values_.find(arg->name()) != result.values_.end();

            if (!has_value) {
                if (arg->is_required()) {
                    std::string display_name = arg->name();
                    if (!arg->long_name().empty()) {
                        display_name = arg->long_name();
                    } else if (!arg->short_name().empty()) {
                        display_name = arg->short_name();
                    }
                    throw ParseError("Required argument '" + display_name +
                                     "' is missing");
                }
                if (arg->has_default()) {
                    result.values_[arg->name()] = arg->default_val();
                }
            }
        }
    }
}

auto ArgumentParser::print_help() const -> void { std::cout << get_help(); }

auto ArgumentParser::get_help() const -> std::string {
    std::ostringstream oss;

    oss << build_usage_line();

    if (!description.empty()) {
        oss << "\n" << description << "\n";
    }

    oss << build_positional_section();
    oss << build_optional_section();

    return oss.str();
}

auto ArgumentParser::build_usage_line() const -> std::string {
    std::ostringstream oss;
    oss << "Usage: " << program_name;

    // Show optional arguments in usage
    for (const auto &arg : arguments) {
        if (arg->type() == Argument::Type::OPTIONAL) {
            oss << " [";
            if (!arg->short_name().empty()) {
                oss << arg->short_name();
            } else if (!arg->long_name().empty()) {
                oss << arg->long_name();
            }
            oss << " VALUE]";
        } else if (arg->type() == Argument::Type::FLAG) {
            if (arg->name() != "help") { // Skip help flag in usage
                oss << " [";
                if (!arg->short_name().empty()) {
                    oss << arg->short_name();
                } else if (!arg->long_name().empty()) {
                    oss << arg->long_name();
                }
                oss << "]";
            }
        }
    }

    // Show positional arguments in usage
    for (const auto &arg : arguments) {
        if (arg->type() == Argument::Type::POSITIONAL) {
            oss << " " << arg->name();
        }
    }

    oss << "\n";
    return oss.str();
}

auto ArgumentParser::build_positional_section() const -> std::string {
    std::ostringstream oss;
    bool has_positional = false;

    for (const auto &arg : arguments) {
        if (arg->type() == Argument::Type::POSITIONAL) {
            if (!has_positional) {
                oss << "\nPositional arguments:\n";
                has_positional = true;
            }
            oss << "  " << std::left << std::setw(HELP_COLUMN_WIDTH)
                << arg->name();
            oss << arg->help_text();
            if (!arg->get_choices().empty()) {
                oss << format_choices(arg->get_choices());
            }
            oss << "\n";
        }
    }

    return oss.str();
}

auto ArgumentParser::build_optional_section() const -> std::string {
    std::ostringstream oss;
    oss << "\nOptional arguments:\n";

    for (const auto &arg : arguments) {
        if (arg->type() == Argument::Type::OPTIONAL ||
            arg->type() == Argument::Type::FLAG) {
            std::string names;
            if (!arg->short_name().empty()) {
                names = arg->short_name();
            }
            if (!arg->long_name().empty()) {
                if (!names.empty()) {
                    names += ", ";
                }
                names += arg->long_name();
            }

            oss << "  " << std::left << std::setw(HELP_COLUMN_WIDTH) << names;
            oss << arg->help_text();

            if (arg->has_default()) {
                oss << " (default: " << arg->default_val() << ")";
            }
            if (!arg->get_choices().empty()) {
                oss << format_choices(arg->get_choices());
            }
            oss << "\n";
        }
    }

    return oss.str();
}

auto ArgumentParser::format_choices(const std::vector<std::string> &choices)
    -> std::string {
    std::ostringstream oss;
    oss << " (choices: ";
    for (size_t i = 0; i < choices.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << choices[i];
    }
    oss << ")";
    return oss.str();
}

} // namespace cmd_parser
