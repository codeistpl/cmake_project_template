#include "argument_parser_impl.h"
#include "../parse_error.h"
#include <algorithm>
#include <iomanip>
#include <sstream>

namespace cmd_parser {
namespace detail {

// Constants
constexpr int HELP_COLUMN_WIDTH = 20;

ArgumentParserImpl::ArgumentParserImpl(std::string name, std::string desc)
    : program_name(std::move(name)), description(std::move(desc)) {}

auto ArgumentParserImpl::register_argument(const std::shared_ptr<Argument> &arg)
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

auto ArgumentParserImpl::find_argument(const std::string &name) const
    -> std::shared_ptr<Argument> {
    if (auto it = arg_map.find(name); it != arg_map.end()) {
        return it->second;
    }
    return nullptr;
}

auto ArgumentParserImpl::validate_choices(const Argument &arg,
                                          const std::string &value) -> void {
    const auto &choices = arg.get_choices();
    if (choices.empty()) {
        return;
    }

    if (std::find(choices.cbegin(), choices.cend(), value) != choices.cend()) {
        return;
    }

    std::ostringstream oss;
    oss << "Invalid value '" << value << "' for argument '" << arg.name()
        << "'. Valid choices are: ";

    bool first = true;
    for (const auto &choice : choices) {
        if (!first) {
            oss << ", ";
        }
        oss << "'" << choice << "'";
        first = false;
    }
    throw ParseError(oss.str());
}

auto ArgumentParserImpl::collect_positional_arguments() const
    -> std::vector<std::shared_ptr<Argument>> {
    std::vector<std::shared_ptr<Argument>> positional_args;
    positional_args.reserve(arguments.size());

    for (const auto &arg : arguments) {
        if (arg->type() == Argument::Type::POSITIONAL) {
            positional_args.emplace_back(arg);
        }
    }
    return positional_args;
}

auto ArgumentParserImpl::process_optional_or_flag(
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

auto ArgumentParserImpl::process_positional_argument(
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

auto ArgumentParserImpl::apply_defaults_and_validate(ParsedArgs &result) const
    -> void {
    for (const auto &arg : arguments) {
        if (arg->type() == Argument::Type::FLAG) {
            if (!result.flags_.contains(arg->name())) {
                result.flags_[arg->name()] = false;
            }
        } else {
            if (result.values_.contains(arg->name())) {
                continue;
            }

            if (arg->is_required()) {
                std::string display_name;
                if (!arg->long_name().empty()) {
                    display_name = arg->long_name();
                } else if (!arg->short_name().empty()) {
                    display_name = arg->short_name();
                } else {
                    display_name = arg->name();
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

auto ArgumentParserImpl::build_usage_line() const -> std::string {
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

auto ArgumentParserImpl::build_positional_section() const -> std::string {
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

auto ArgumentParserImpl::build_optional_section() const -> std::string {
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

auto ArgumentParserImpl::format_choices(const std::vector<std::string> &choices)
    -> std::string {
    if (choices.empty()) {
        return "";
    }

    std::ostringstream oss;
    oss << " (choices: ";

    auto it = choices.cbegin();
    oss << *it;
    for (++it; it != choices.cend(); ++it) {
        oss << ", " << *it;
    }

    oss << ")";
    return oss.str();
}

} // namespace detail
} // namespace cmd_parser
