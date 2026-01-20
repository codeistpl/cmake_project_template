#include "argument.h"

namespace cmd_parser {

Argument::Argument(const std::string &name, Argument::Type type)
    : name_(name), type_(type) {

    if (type == Argument::Type::OPTIONAL || type == Argument::Type::FLAG) {
        // Parse short and long names from the format "-s" or "--long"
        if (name.size() >= 2 && name[0] == '-') {
            if (name[1] == '-') {
                long_name_ = name;
            } else {
                short_name_ = name;
            }
        }
    }
}

auto Argument::help(const std::string &help_text) -> Argument & {
    help_text_ = help_text;
    return *this;
}

auto Argument::default_value(const std::string &value) -> Argument & {
    default_value_ = value;
    has_default_ = true;
    return *this;
}

auto Argument::required(bool is_required) -> Argument & {
    required_ = is_required;
    return *this;
}

auto Argument::choices(const std::vector<std::string> &valid_choices)
    -> Argument & {
    choices_ = valid_choices;
    return *this;
}

} // namespace cmd_parser
