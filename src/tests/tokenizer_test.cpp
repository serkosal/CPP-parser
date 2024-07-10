#include <gtest/gtest.h>

#include <iostream>

#include "tokenizer.hpp"

Tokenizer tokenizer;

TEST(IdentificatorCreation, simple)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("variable");

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[0].m_value, "variable");
}

TEST(IdentificatorCreation, spaceSeparated)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("first second\nthird\tfourth");

	ASSERT_EQ(tokens.size(), size_t(4));

	EXPECT_EQ(tokens[0].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[0].m_value, "first");

	EXPECT_EQ(tokens[1].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[1].m_value, "second");

	EXPECT_EQ(tokens[2].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[2].m_value, "third");

	EXPECT_EQ(tokens[3].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[3].m_value, "fourth");
}

TEST(IdentificatorCreation, withUnderscores)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("var _var va_r var_ rav");

	ASSERT_EQ(tokens.size(), size_t(5));

	EXPECT_EQ(tokens[0].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[0].m_value, "var");

	EXPECT_EQ(tokens[1].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[1].m_value, "_var");

	EXPECT_EQ(tokens[2].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[2].m_value, "va_r");

	EXPECT_EQ(tokens[3].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[3].m_value, "var_");

	EXPECT_EQ(tokens[4].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[4].m_value, "rav");
}

TEST(IdentificatorCreation, withDigits)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("var var1 va2r 3var");

	ASSERT_EQ(tokens.size(), size_t(4));

	EXPECT_EQ(tokens[0].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[0].m_value, "var");

	EXPECT_EQ(tokens[1].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[1].m_value, "var1");

	EXPECT_EQ(tokens[2].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[2].m_value, "va2r");

	EXPECT_EQ(tokens[3].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[3].m_value, "3var");
}

TEST(IdentificatorCreation, withDigitsUnderscore)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize(
		"var "
		"_1var _va2r _var3 "
		"4_var 5va_r 6var_ "
		"v7_ar va8_r var9_ "
		"var_10"
	);

	ASSERT_EQ(tokens.size(), size_t(11));

	EXPECT_EQ(tokens[0].m_type, Token::Type::identificator);	// var
	EXPECT_EQ(tokens[0].m_value, "var");

	EXPECT_EQ(tokens[1].m_type, Token::Type::identificator);	// _1var
	EXPECT_EQ(tokens[1].m_value, "_1var");

	EXPECT_EQ(tokens[2].m_type, Token::Type::identificator);	// _va2r
	EXPECT_EQ(tokens[2].m_value, "_va2r");

	EXPECT_EQ(tokens[3].m_type, Token::Type::identificator);	// _var3
	EXPECT_EQ(tokens[3].m_value, "_var3");

	EXPECT_EQ(tokens[4].m_type, Token::Type::invalid);			// 4_var
	EXPECT_EQ(tokens[4].m_value, "4_var");

	EXPECT_EQ(tokens[5].m_type, Token::Type::invalid);			// 5va_r
	EXPECT_EQ(tokens[5].m_value, "5va_r");

	EXPECT_EQ(tokens[6].m_type, Token::Type::invalid);			// 6var_
	EXPECT_EQ(tokens[6].m_value, "6var_");

	EXPECT_EQ(tokens[7].m_type, Token::Type::identificator);	// v7_ar
	EXPECT_EQ(tokens[7].m_value, "v7_ar");

	EXPECT_EQ(tokens[8].m_type, Token::Type::identificator);	// va8_r
	EXPECT_EQ(tokens[8].m_value, "va8_r");

	EXPECT_EQ(tokens[9].m_type, Token::Type::identificator);	// var9_
	EXPECT_EQ(tokens[9].m_value, "var9_");

	EXPECT_EQ(tokens[10].m_type, Token::Type::identificator);	// var_10
	EXPECT_EQ(tokens[10].m_value, "var_10");
}


TEST(IntegerCreation, positive)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("1234567890");

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::integer);
	EXPECT_EQ(tokens[0].m_value, "1234567890");
}

TEST(IntegerCreation, negative)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("-30");

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::integer);
	EXPECT_EQ(tokens[0].m_value, "-30");
}

TEST(IntegerCreation, zero)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("0");

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::integer);
	EXPECT_EQ(tokens[0].m_value, "0");
}


TEST(StringCreation, simple)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("\"abcdfhksdhkfvmks\"");

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::string);
	EXPECT_EQ(tokens[0].m_value, "\"abcdfhksdhkfvmks\"");
}

TEST(StringCreation, spaceSeparated)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("\"first\" \"second\"");
	
	ASSERT_EQ(tokens.size(), size_t(2));

	EXPECT_EQ(tokens[0].m_type, Token::Type::string);
	EXPECT_EQ(tokens[0].m_value, "\"first\"");
	EXPECT_EQ(tokens[1].m_type, Token::Type::string);
	EXPECT_EQ(tokens[1].m_value, "\"second\"");
}

TEST(StringCreation, spaceInside)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("\"first second\"");

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::string);
}

TEST(StringCreation, escapeSequences)
{
	tokenizer.reset();
	
	auto& tokens = tokenizer.tokenize( std::string() +
		'"' + '\\' +  '"' + '"' + ' '  +	// 0
		'"' + '\\' + '\\' + '"' + ' '  +	// 1
		'"' + '\\' +  'n' + '"' + ' '  +	// 2
		'"' + '\\' +  't' + '"' + ' '  +	// 3
		'"' + '\\' +  'a' + '"' + ' '  +	// 4
		'"' + '\\' +  'b' + '"' + ' '  +	// 5
		'"' + '\\' +  'f' + '"' + ' '  +	// 6
		'"' + '\\' +  'v' + '"' + ' '  +	// 7
		'"' + '\\' +  'r' + '"' + ' '  +	// 8
		'"' + '\\' +  'x' + '"' + ' '		// 9
	);

	ASSERT_EQ(tokens.size(), size_t(10));

	EXPECT_EQ(tokens[0].m_type, Token::Type::string);
	EXPECT_EQ(tokens[1].m_type, Token::Type::string);
	EXPECT_EQ(tokens[2].m_type, Token::Type::string);
	EXPECT_EQ(tokens[3].m_type, Token::Type::string);
	EXPECT_EQ(tokens[4].m_type, Token::Type::string);
	EXPECT_EQ(tokens[5].m_type, Token::Type::string);
	EXPECT_EQ(tokens[6].m_type, Token::Type::string);
	EXPECT_EQ(tokens[7].m_type, Token::Type::string);
	EXPECT_EQ(tokens[8].m_type, Token::Type::string);
	EXPECT_EQ(tokens[9].m_type, Token::Type::invalid);
}

TEST(StringCreation, escapeSequenceDoesntEnd)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("\" \\");

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::invalid);
}



TEST(OperatorCreation, arithmetic)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("+ - * ** / // % ++ --");

	ASSERT_EQ(tokens.size(), size_t(9));

	EXPECT_EQ(tokens[0].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[1].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[2].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[3].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[4].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[5].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[6].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[7].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[8].m_type, Token::Type::_operator);
}

TEST(OperatorCreation, incorrectArithmetic)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("+- -+ %% %/ /%");

	ASSERT_EQ(tokens.size(), size_t(5));

	EXPECT_EQ(tokens[0].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[1].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[2].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[3].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[4].m_type, Token::Type::invalid);
}

TEST(OperatorCreation, logicAndComparison)
{
	tokenizer.reset();

	auto& tokens = tokenizer.tokenize("== != ! && || < > <= >=");

	ASSERT_EQ(tokens.size(), size_t(9));

	EXPECT_EQ(tokens[0].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[1].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[2].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[3].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[4].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[5].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[6].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[7].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[8].m_type, Token::Type::_operator);
}

TEST(OperatorCreation, incorrectLogicAndComparison)
{
	tokenizer.reset();		
								   //   0  1  2  3  4  5  6   7
	auto& tokens = tokenizer.tokenize("=! !! &| |& <> => =< <=>");

	ASSERT_EQ(tokens.size(), size_t(8));

	EXPECT_EQ(tokens[0].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[1].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[2].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[3].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[4].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[5].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[6].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[7].m_type, Token::Type::invalid);
}

TEST(OperatorCreation, bitwise)
{
	tokenizer.reset();
									 //0 1 2 3  4  5
	auto& tokens = tokenizer.tokenize("& | ^ ~ << >> ");

	ASSERT_EQ(tokens.size(), size_t(6));

	EXPECT_EQ(tokens[0].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[1].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[2].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[3].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[4].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[5].m_type, Token::Type::_operator);
}

TEST(OperatorCreation, assignment)
{
	tokenizer.reset();
									// 0  1  2  3   4  5   6  7   8   9 10 11 12 13  14  15
	auto& tokens = tokenizer.tokenize("= += -= *= **= /= //= %= &&= ||= &= |= ^= ~= <<= >>=");

	ASSERT_EQ(tokens.size(), size_t(16));

	EXPECT_EQ(tokens[0].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[1].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[2].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[3].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[4].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[5].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[6].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[7].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[8].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[9].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[10].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[11].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[12].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[13].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[14].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[15].m_type, Token::Type::_operator);
}


TEST(OperatorInteraction, integers)
{
	tokenizer.reset();
									// 0  1 2 3 4 5 6 7 8910 111213
	auto& tokens = tokenizer.tokenize("4 ++ 2-- ++2 //3 5<3  25>=13");

	ASSERT_EQ(tokens.size(), size_t(14));

	EXPECT_EQ(tokens[0].m_type, Token::Type::integer);
	EXPECT_EQ(tokens[0].m_value, "4");

	EXPECT_EQ(tokens[1].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[1].m_value, "++");

	EXPECT_EQ(tokens[2].m_type, Token::Type::integer);
	EXPECT_EQ(tokens[2].m_value, "2");

	EXPECT_EQ(tokens[3].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[3].m_value, "--");

	EXPECT_EQ(tokens[4].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[4].m_value, "++");

	EXPECT_EQ(tokens[5].m_type, Token::Type::integer);
	EXPECT_EQ(tokens[5].m_value, "2");

	EXPECT_EQ(tokens[6].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[6].m_value, "//");

	EXPECT_EQ(tokens[7].m_type, Token::Type::integer);
	EXPECT_EQ(tokens[7].m_value, "3");

	EXPECT_EQ(tokens[8].m_type, Token::Type::integer);
	EXPECT_EQ(tokens[8].m_value, "5");

	EXPECT_EQ(tokens[9].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[9].m_value, "<");

	EXPECT_EQ(tokens[10].m_type, Token::Type::integer);
	EXPECT_EQ(tokens[10].m_value, "3");

	EXPECT_EQ(tokens[11].m_type, Token::Type::integer);
	EXPECT_EQ(tokens[11].m_value, "25");

	EXPECT_EQ(tokens[12].m_type, Token::Type::_operator);
	EXPECT_EQ(tokens[12].m_value, ">=");

	EXPECT_EQ(tokens[13].m_type, Token::Type::integer);
	EXPECT_EQ(tokens[13].m_value, "13");
}

TEST(OperatorInteraction, incorrect)
{
	tokenizer.reset();
	auto& tokens = tokenizer.tokenize("**/2 //+3");
	ASSERT_EQ(tokens.size(), size_t(4));

	EXPECT_EQ(tokens[0].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[0].m_value, "**/");

	EXPECT_EQ(tokens[1].m_type, Token::Type::integer);
	EXPECT_EQ(tokens[1].m_value, "2");

	EXPECT_EQ(tokens[2].m_type, Token::Type::invalid);
	EXPECT_EQ(tokens[2].m_value, "//+");

	EXPECT_EQ(tokens[3].m_type, Token::Type::integer);
	EXPECT_EQ(tokens[3].m_value, "3");
}