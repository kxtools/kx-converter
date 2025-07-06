#include "gtest/gtest.h"
#include "kx_converter/parser.h"

using namespace kx_converter;

// This test uses the original "standard" line format.
TEST(ParserTest, HandlesStandardLine) {
    LineParser parser;
    std::string line = "1	'Start' 123.45 -67.89 10.0";
    auto result = parser.parse(line);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "Start");
    EXPECT_DOUBLE_EQ(result->x, 123.45);
    EXPECT_DOUBLE_EQ(result->y, 10.0);    // Swapped Z
    EXPECT_DOUBLE_EQ(result->z, -67.89);  // Swapped Y
}

// Test for names with apostrophes from Trolls Revenge JP.txt
TEST(ParserTest, HandlesTrollsRevengeName) {
    LineParser parser;
    std::string line = "1	'Troll's_Revenge_Chest_1' -203.554 46.3196 42.0846";
    auto result = parser.parse(line);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "Troll's_Revenge_Chest_1");
    EXPECT_DOUBLE_EQ(result->x, -203.554);
}

// Test for lines with NO coordinates. This must be rejected.
TEST(ParserTest, RejectsLineWithNoCoordinates) {
    LineParser parser;
    std::string line = "10     [CChest06_HOW_TO_PLAY-23454322311286]";
    auto result = parser.parse(line);
    ASSERT_FALSE(result.has_value());
}

// Test for lines with empty quoted names
TEST(ParserTest, HandlesEmptyQuotedName) {
    LineParser parser;
    std::string line = "78     '' 94.6887 16.8526 -807.248";
    auto result = parser.parse(line);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "");
}

// Test for the line that was causing a crash, with scientific notation
TEST(ParserTest, HandlesScientificNotationAndComplexName) {
    LineParser parser;
    std::string line = "415       'for-this-achievment[Honorary-Crew-Member]You-have-to-protect-the-SS-Pinkeens-from-1/1-looting-group' 1.4013e-45 0 4.09179e-43";
    auto result = parser.parse(line);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "for-this-achievment[Honorary-Crew-Member]You-have-to-protect-the-SS-Pinkeens-from-1/1-looting-group");
    EXPECT_DOUBLE_EQ(result->x, 1.4013e-45);
    EXPECT_DOUBLE_EQ(result->y, 4.09179e-43); // Swapped Z
    EXPECT_DOUBLE_EQ(result->z, 0);           // Swapped Y
}

// Test for bizarre nested/mismatched quotes
TEST(ParserTest, HandlesMismatchedAndNestedQuotes) {
    LineParser parser;
    std::string line = "2	\"'Port-here[-for-the-Achievment=-Upper-City-Security-Chief]-You-need-to-revive-5-Guards\"\"' 83.057 166.162 -11.8453";
    auto result = parser.parse(line);
    ASSERT_TRUE(result.has_value());
    // The parser is greedy, which is acceptable. The important part is getting the coords.
    EXPECT_EQ(result->name, "\"'Port-here[-for-the-Achievment=-Upper-City-Security-Chief]-You-need-to-revive-5-Guards\"\"'");
}

// --- NEWLY ADDED TESTS BASED ON LATEST ERRORS ---

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

TEST(ParserTest, RejectsLineWithMissingCheckpointNumber) {
    LineParser parser;
    std::string line = "-108.266 305.411 -382.819";
    auto result = parser.parse(line);
    // This is invalid because it's missing the checkpoint number and name, only has 3 parts.
    ASSERT_FALSE(result.has_value());
}

TEST(ParserTest, HandlesNameWithUnderscoresAndNoQuotes) {
    LineParser parser;
    std::string line = "2     Enter_Sun's_Refuge -1072.83 79.44 97.64";
    auto result = parser.parse(line);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "Enter_Sun's_Refuge");
}