#ifndef CMD_PARSER_ARGUMENT_H
#define CMD_PARSER_ARGUMENT_H

#include <cstdint>
#include <string>
#include <vector>

namespace cmd_parser {

namespace detail {
class ArgumentParserImpl;
}

/**
 * Represents an argument definition
 */
class Argument {
  public:
    enum class Type : uint8_t { POSITIONAL = 0, OPTIONAL, FLAG };

    Argument(const std::string &name, Type type);

    auto help(const std::string &help_text) -> Argument &;
    auto default_value(const std::string &value) -> Argument &;
    auto required(bool is_required = true) -> Argument &;
    auto choices(const std::vector<std::string> &valid_choices) -> Argument &;

    [[nodiscard]] auto name() const -> std::string { return name_; }
    [[nodiscard]] auto short_name() const -> std::string { return short_name_; }
    [[nodiscard]] auto long_name() const -> std::string { return long_name_; }
    [[nodiscard]] auto type() const -> Type { return type_; }
    [[nodiscard]] auto help_text() const -> std::string { return help_text_; }
    [[nodiscard]] auto default_val() const -> std::string {
        return default_value_;
    }
    [[nodiscard]] auto is_required() const -> bool { return required_; }
    [[nodiscard]] auto has_default() const -> bool { return has_default_; }
    [[nodiscard]] auto get_choices() const -> const std::vector<std::string> & {
        return choices_;
    }

  private:
    friend class ArgumentParser;
    friend class detail::ArgumentParserImpl;

    std::string name_;
    std::string short_name_;
    std::string long_name_;
    Type type_;
    std::string help_text_;
    std::string default_value_;
    bool required_{};
    bool has_default_{};
    std::vector<std::string> choices_;
};

} // namespace cmd_parser

#endif // CMD_PARSER_ARGUMENT_H
