#include "gtest/gtest.h"
#include "kx_converter/parser.h"

using namespace kx_converter;

// Test Suite for the LineParser
// This suite covers all known valid and invalid formats discovered during development
// to ensure the parser is robust and to prevent future regressions.

// --- Standard Valid Cases ---

// Verifies that the parser can handle the most common format, which uses a tab
// character as the primary separator between the number and the name.
TEST(ParserTest, HandlesStandardLineWithTab) {
    LineParser parser;
    std::string line = "1	'Start' 123.45 -67.89 10.0";
    auto result = parser.parse(line);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "Start");
    EXPECT_DOUBLE_EQ(result->x, 123.45);
    EXPECT_DOUBLE_EQ(result->y, 10.0);    // Swapped Z
    EXPECT_DOUBLE_EQ(result->z, -67.89);  // Swapped Y
}

// Verifies that the parser is not sensitive to the amount or type of whitespace
// between the different parts of the line.
TEST(ParserTest, HandlesLineWithMultipleSpaces) {
    LineParser parser;
    std::string line = "2   'Finish'   -100.0   200.5   300.99";
    auto result = parser.parse(line);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "Finish");
    EXPECT_DOUBLE_EQ(result->x, -100.0);
    EXPECT_DOUBLE_EQ(result->y, 300.99); // Swapped Z
    EXPECT_DOUBLE_EQ(result->z, 200.5);  // Swapped Y
}


// --- Edge Cases for Names ---

// Tests a format where the name is present but empty, represented by ''.
TEST(ParserTest, HandlesEmptyQuotedName) {
    LineParser parser;
    std::string line = "78     '' 94.6887 16.8526 -807.248";
    auto result = parser.parse(line);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "");
}

// Tests that a name containing an apostrophe (which is also the quote delimiter)
// is parsed correctly as long as the entire name is quoted.
TEST(ParserTest, HandlesNameWithApostrophe) {
    LineParser parser;
    std::string line = "1	'Troll's_Revenge_Chest_1' -203.554 46.3196 42.0846";
    auto result = parser.parse(line);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "Troll's_Revenge_Chest_1");
    EXPECT_DOUBLE_EQ(result->x, -203.554);
}

// Tests that complex names containing brackets, hyphens, and ampersands are
// captured correctly.
TEST(ParserTest, HandlesNameWithBracketsAndHyphens) {
    LineParser parser;
    std::string line = "7	'[Hablion's-Dignity]&[Reversal-of-Fortune]' -58.5739 874.236 -73.7113";
    auto result = parser.parse(line);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "[Hablion's-Dignity]&[Reversal-of-Fortune]");
}

// Tests that names with underscores but no surrounding quotes are parsed correctly.
TEST(ParserTest, HandlesNameWithUnderscoresAndNoQuotes) {
    LineParser parser;
    std::string line = "2     Enter_Sun's_Refuge -1072.83 79.44 97.64";
    auto result = parser.parse(line);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "Enter_Sun's_Refuge");
}


// --- Edge Cases for Coordinates ---

// Verifies that coordinates separated by commas instead of spaces are handled correctly.
TEST(ParserTest, HandlesCommaSeparatedCoordinates) {
    LineParser parser;
    std::string line = "1    'The_Hunting_Grounds' -126.699, 99.3883, 1125.62";
    auto result = parser.parse(line);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "The_Hunting_Grounds");
    EXPECT_DOUBLE_EQ(result->x, -126.699);
    EXPECT_DOUBLE_EQ(result->y, 1125.62); // Swapped Z
    EXPECT_DOUBLE_EQ(result->z, 99.3883);  // Swapped Y
}

// Verifies that coordinates written in scientific e-notation are parsed correctly.
// This was a specific cause of a `bad_conversion` crash.
TEST(ParserTest, HandlesScientificNotation) {
    LineParser parser;
    std::string line = "415 'SomeName' 1.4013e-45 0 4.09179e-43";
    auto result = parser.parse(line);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "SomeName");
    EXPECT_DOUBLE_EQ(result->x, 1.4013e-45);
    EXPECT_DOUBLE_EQ(result->y, 4.09179e-43); // Swapped
    EXPECT_DOUBLE_EQ(result->z, 0);           // Swapped
}


// --- Expected Rejection Cases ---

// Lines that do not contain coordinates should be safely rejected.
TEST(ParserTest, RejectsLineWithNoCoordinates) {
    LineParser parser;
    std::string line = "10     [CChest06_HOW_TO_PLAY-23454322311286]";
    auto result = parser.parse(line);
    ASSERT_FALSE(result.has_value());
}

// Lines that are missing the initial checkpoint number should be rejected.
TEST(ParserTest, RejectsLineWithMissingCheckpointNumber) {
    LineParser parser;
    std::string line = "-108.266 305.411 -382.819";
    auto result = parser.parse(line);
    // This is invalid because it's missing the checkpoint number and name, only has 3 parts.
    ASSERT_FALSE(result.has_value());
}

// Lines that do not have a full set of three coordinates should be rejected.
TEST(ParserTest, RejectsLineWithOnlyTwoCoordinates) {
    LineParser parser;
    std::string line = "4	'Oops' 123.45 56.78";
    auto result = parser.parse(line);
    ASSERT_FALSE(result.has_value());
}