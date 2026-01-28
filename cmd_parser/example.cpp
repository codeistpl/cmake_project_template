#include <iostream>

#include "argument_parser.h"
#include "parse_error.h"

auto main(int argc, char *argv[]) -> int {
    cmd_parser::ArgumentParser parser(
        "example_program", "This is an example program using cmd_parser");

    parser.add_argument("input", "Input file path");
    parser.add_argument("-o", "--output", "Output file path")
        .default_value("output.txt");
    parser.add_flag("-v", "--verbose", "Enable verbose output");

    try {
        auto args =
            parser.parse_args(std::vector<std::string>(argv, argv + argc));

        // If help was requested, exit successfully
        if (args.status() == cmd_parser::ParseStatus::HELP_REQUESTED) {
            return 0;
        }

        std::string input = args.get("input");
        std::string output = args.get("output");
        bool verbose = args.get_flag("verbose");

        if (verbose) {
            std::cout << "Verbose mode enabled\n";
        }
        std::cout << "Input: " << input << "\n";
        std::cout << "Output: " << output << "\n";
    } catch (const cmd_parser::ParseError &e) {
        std::cerr << "Error: " << e.what() << "\n";
        parser.print_help();
        return 1;
    }

    return 0;
}