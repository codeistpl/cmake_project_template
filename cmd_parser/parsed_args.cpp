#include "parsed_args.h"
#include "parse_error.h"

namespace cmd_parser {

auto ParsedArgs::get(const std::string &name) const -> std::string {
    auto found = values_.find(name);
    if (found == values_.end()) {
        throw ParseError("Argument '" + name + "' not found");
    }
    return found->second;
}

auto ParsedArgs::get_flag(const std::string &name) const -> bool {
    auto found = flags_.find(name);
    if (found == flags_.end()) {
        return false;
    }
    return found->second;
}

auto ParsedArgs::has(const std::string &name) const -> bool {
    return values_.find(name) != values_.end() ||
           flags_.find(name) != flags_.end();
}

auto ParsedArgs::get_list(const std::string &name) const
    -> std::vector<std::string> {
    auto found = lists_.find(name);
    if (found == lists_.end()) {
        return {};
    }
    return found->second;
}

} // namespace cmd_parser
