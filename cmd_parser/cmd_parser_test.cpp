#include "cmd_parser.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace cmd_parser;

class CmdParserTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Setup code if needed
    }
};

// Test basic positional argument parsing
TEST_F(CmdParserTest, ParsePositionalArgument) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("input", "Input file");

    std::vector<std::string> args = {"file.txt"};
    auto result = parser.parse_args(args);

    EXPECT_EQ(result.get("input"), "file.txt");
}

// Test multiple positional arguments
TEST_F(CmdParserTest, ParseMultiplePositionalArguments) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("input", "Input file");
    parser.add_argument("output", "Output file");

    std::vector<std::string> args = {"input.txt", "output.txt"};
    auto result = parser.parse_args(args);

    EXPECT_EQ(result.get("input"), "input.txt");
    EXPECT_EQ(result.get("output"), "output.txt");
}

// Test optional argument with long form
TEST_F(CmdParserTest, ParseOptionalArgumentLongForm) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("--output", "--output", "Output file");

    std::vector<std::string> args = {"--output", "out.txt"};
    auto result = parser.parse_args(args);

    EXPECT_EQ(result.get("output"), "out.txt");
}

// Test optional argument with short form
TEST_F(CmdParserTest, ParseOptionalArgumentShortForm) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("-o", "--output", "Output file");

    std::vector<std::string> args = {"-o", "out.txt"};
    auto result = parser.parse_args(args);

    EXPECT_EQ(result.get("output"), "out.txt");
}

// Test flag argument
TEST_F(CmdParserTest, ParseFlagArgument) {
    ArgumentParser parser("test", "Test program");
    parser.add_flag("-v", "--verbose", "Verbose output");

    std::vector<std::string> args = {"--verbose"};
    auto result = parser.parse_args(args);

    EXPECT_TRUE(result.get_flag("verbose"));
}

// Test flag not set
TEST_F(CmdParserTest, FlagNotSet) {
    ArgumentParser parser("test", "Test program");
    parser.add_flag("-v", "--verbose", "Verbose output");

    std::vector<std::string> args = {};
    auto result = parser.parse_args(args);

    EXPECT_FALSE(result.get_flag("verbose"));
}

// Test default value
TEST_F(CmdParserTest, DefaultValue) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("-o", "--output", "Output file")
        .default_value("default.txt");

    std::vector<std::string> args = {};
    auto result = parser.parse_args(args);

    EXPECT_EQ(result.get("output"), "default.txt");
}

// Test default value override
TEST_F(CmdParserTest, DefaultValueOverride) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("-o", "--output", "Output file")
        .default_value("default.txt");

    std::vector<std::string> args = {"-o", "custom.txt"};
    auto result = parser.parse_args(args);

    EXPECT_EQ(result.get("output"), "custom.txt");
}

// Test required argument missing
TEST_F(CmdParserTest, RequiredArgumentMissing) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("-o", "--output", "Output file").required(true);

    std::vector<std::string> args = {};

    EXPECT_THROW(parser.parse_args(args), ParseError);
}

// Test required argument provided
TEST_F(CmdParserTest, RequiredArgumentProvided) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("-o", "--output", "Output file").required(true);

    std::vector<std::string> args = {"-o", "out.txt"};
    auto result = parser.parse_args(args);

    EXPECT_EQ(result.get("output"), "out.txt");
}

// Test choices validation - valid
TEST_F(CmdParserTest, ChoicesValid) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("-m", "--mode", "Mode")
        .choices({"fast", "slow", "medium"});

    std::vector<std::string> args = {"-m", "fast"};
    auto result = parser.parse_args(args);

    EXPECT_EQ(result.get("mode"), "fast");
}

// Test choices validation - invalid
TEST_F(CmdParserTest, ChoicesInvalid) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("-m", "--mode", "Mode")
        .choices({"fast", "slow", "medium"});

    std::vector<std::string> args = {"-m", "invalid"};

    EXPECT_THROW(parser.parse_args(args), ParseError);
}

// Test unknown argument
TEST_F(CmdParserTest, UnknownArgument) {
    ArgumentParser parser("test", "Test program");

    std::vector<std::string> args = {"--unknown"};

    EXPECT_THROW(parser.parse_args(args), ParseError);
}

// Test too many positional arguments
TEST_F(CmdParserTest, TooManyPositionalArguments) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("input", "Input file");

    std::vector<std::string> args = {"file1.txt", "file2.txt"};

    EXPECT_THROW(parser.parse_args(args), ParseError);
}

// Test missing value for optional argument
TEST_F(CmdParserTest, MissingValueForOptionalArgument) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("-o", "--output", "Output file");

    std::vector<std::string> args = {"-o"};

    EXPECT_THROW(parser.parse_args(args), ParseError);
}

// Test complex scenario
TEST_F(CmdParserTest, ComplexScenario) {
    ArgumentParser parser("myapp", "My application");
    parser.add_argument("input", "Input file");
    parser.add_argument("-o", "--output", "Output file")
        .default_value("out.txt");
    parser.add_flag("-v", "--verbose", "Verbose mode");
    parser.add_argument("-f", "--format", "Output format")
        .choices({"json", "xml", "csv"})
        .default_value("json");

    std::vector<std::string> args = {"data.txt", "-v", "-f", "xml"};
    auto result = parser.parse_args(args);

    EXPECT_EQ(result.get("input"), "data.txt");
    EXPECT_EQ(result.get("output"), "out.txt");
    EXPECT_TRUE(result.get_flag("verbose"));
    EXPECT_EQ(result.get("format"), "xml");
}

// Test has() method
TEST_F(CmdParserTest, HasMethod) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("-o", "--output", "Output file");
    parser.add_flag("-v", "--verbose", "Verbose mode");

    std::vector<std::string> args = {"-o", "out.txt"};
    auto result = parser.parse_args(args);

    EXPECT_TRUE(result.has("output"));
    EXPECT_TRUE(result.has("verbose")); // Flag is always present
    EXPECT_FALSE(result.has("nonexistent"));
}

// Test get_help method
TEST_F(CmdParserTest, GetHelpMethod) {
    ArgumentParser parser("myapp", "Test application");
    parser.add_argument("input", "Input file");
    parser.add_argument("-o", "--output", "Output file")
        .default_value("out.txt");
    parser.add_flag("-v", "--verbose", "Verbose mode");

    std::string help = parser.get_help();

    EXPECT_NE(help.find("myapp"), std::string::npos);
    EXPECT_NE(help.find("Test application"), std::string::npos);
    EXPECT_NE(help.find("input"), std::string::npos);
    EXPECT_NE(help.find("--output"), std::string::npos);
    EXPECT_NE(help.find("--verbose"), std::string::npos);
}

// Test positional argument with choices
TEST_F(CmdParserTest, PositionalArgumentWithChoices) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("command", "Command to execute")
        .choices({"start", "stop", "restart"});

    std::vector<std::string> args = {"start"};
    auto result = parser.parse_args(args);

    EXPECT_EQ(result.get("command"), "start");
}

// Test positional argument with invalid choice
TEST_F(CmdParserTest, PositionalArgumentWithInvalidChoice) {
    ArgumentParser parser("test", "Test program");
    parser.add_argument("command", "Command to execute")
        .choices({"start", "stop", "restart"});

    std::vector<std::string> args = {"invalid"};

    EXPECT_THROW(parser.parse_args(args), ParseError);
}
