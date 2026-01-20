#include "parsed_args.h"
#include "parse_error.h"

namespace cmd_parser {

auto ParsedArgs::get(const std::string &name) const -> std::string {
    if (auto it = values_.find(name); it != values_.end()) {
        return it->second;
    }
    throw ParseError("Argument '" + name + "' not found");
}

auto ParsedArgs::get_flag(const std::string &name) const -> bool {
    if (auto it = flags_.find(name); it != flags_.end()) {
        return it->second;
    }
    return false;
}

auto ParsedArgs::has(const std::string &name) const -> bool {
    return values_.contains(name) || flags_.contains(name);
}

auto ParsedArgs::get_list(const std::string &name) const
    -> std::vector<std::string> {
    if (auto it = lists_.find(name); it != lists_.end()) {
        return it->second;
    }
    return {};
}

} // namespace cmd_parser
