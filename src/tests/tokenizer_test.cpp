#include <gtest/gtest.h>

#include <iostream>

#include "tokenizer.hpp"

Tokenizer tokenizer;

TEST(IdentificatorCreation, simple)
{
	tokenizer.reset();
	tokenizer.tokenize("variable");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::identificator);
}

TEST(IdentificatorCreation, spaceSeparated)
{
	tokenizer.reset();
	tokenizer.tokenize("first second\nthird\tfourth");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(4));

	EXPECT_EQ(tokens[0].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[1].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[2].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[3].m_type, Token::Type::identificator);
}

TEST(IdentificatorCreation, withUnderscores)
{
	tokenizer.reset();
	tokenizer.tokenize("var _var va_r var_ rav");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(5));

	EXPECT_EQ(tokens[0].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[1].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[2].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[3].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[4].m_type, Token::Type::identificator);
}

TEST(IdentificatorCreation, withDigits)
{
	tokenizer.reset();
	tokenizer.tokenize("var var1 va2r 3var");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(4));

	EXPECT_EQ(tokens[0].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[1].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[2].m_type, Token::Type::identificator);
	EXPECT_EQ(tokens[3].m_type, Token::Type::invalid);
}

TEST(IdentificatorCreation, withDigitsUnderscore)
{
	tokenizer.reset();
	tokenizer.tokenize(
		"var "
		"_1var _va2r _var3 "
		"4_var 5va_r 6var_ "
		"v7_ar va8_r var9_ "
		"var_10"
	);

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(11));

	EXPECT_EQ(tokens[0].m_type, Token::Type::identificator);	// var
	EXPECT_EQ(tokens[1].m_type, Token::Type::identificator);	// _1var
	EXPECT_EQ(tokens[2].m_type, Token::Type::identificator);	// _va2r
	EXPECT_EQ(tokens[3].m_type, Token::Type::identificator);	// _var3
	EXPECT_EQ(tokens[4].m_type, Token::Type::invalid);			// 4_var
	EXPECT_EQ(tokens[5].m_type, Token::Type::invalid);			// 5va_r
	EXPECT_EQ(tokens[6].m_type, Token::Type::invalid);			// 6var_
	EXPECT_EQ(tokens[7].m_type, Token::Type::identificator);	// v7_ar
	EXPECT_EQ(tokens[8].m_type, Token::Type::identificator);	// va8_r
	EXPECT_EQ(tokens[9].m_type, Token::Type::identificator);	// var9_
	EXPECT_EQ(tokens[10].m_type, Token::Type::identificator);	// var_10
}


TEST(IntegerCreation, positive)
{
	tokenizer.reset();
	tokenizer.tokenize("1234567890");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::integer);
}

TEST(IntegerCreation, negative)
{
	tokenizer.reset();
	tokenizer.tokenize("-30");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::integer);
}

TEST(IntegerCreation, zero)
{
	tokenizer.reset();
	tokenizer.tokenize("0");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::integer);
}


TEST(StringCreation, simple)
{
	tokenizer.reset();
	tokenizer.tokenize("\"abcdfhksdhkfvmks\"");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::string);
}

TEST(StringCreation, spaceSeparated)
{
	tokenizer.reset();
	tokenizer.tokenize("\"first\" \"second\"");

	auto& tokens = tokenizer.tokens();
	
	ASSERT_EQ(tokens.size(), size_t(2));

	EXPECT_EQ(tokens[0].m_type, Token::Type::string);
	EXPECT_EQ(tokens[1].m_type, Token::Type::string);
}

TEST(StringCreation, spaceInside)
{
	tokenizer.reset();
	tokenizer.tokenize("\"first second\"");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::string);
}

TEST(StringCreation, escapeSequences)
{
	tokenizer.reset();
	auto str = std::string() + 
		'"' + '\\' + '"'  + '"' + ' ' +		// 0
		'"' + '\\' + '\\' + '"' + ' ' +		// 1
		'"' + '\\' + 'n'  + '"' + ' ' +		// 2
		'"' + '\\' + 't'  + '"' + ' ' +		// 3
		'"' + '\\' + 'a'  + '"' + ' ' +		// 4
		'"' + '\\' + 'b'  + '"' + ' ' +		// 5
		'"' + '\\' + 'f'  + '"' + ' ' +		// 6
		'"' + '\\' + 'v'  + '"' + ' ' +		// 7
		'"' + '\\' + 'r'  + '"' + ' ' +		// 8
		'"' + '\\' + 'x'  + '"' + ' ';		// 9	
	tokenizer.tokenize(str);

	auto& tokens = tokenizer.tokens();

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
	tokenizer.tokenize(
		"\" \\"
	);

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_EQ(tokens[0].m_type, Token::Type::invalid);
}



TEST(OperatorCreation, arithmetic)
{
	tokenizer.reset();
	tokenizer.tokenize("+ - * ** / // % ++ --");

	auto& tokens = tokenizer.tokens();

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
	tokenizer.tokenize("+- -+ %% %/ /%");

	auto& tokens = tokenizer.tokens();

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
	tokenizer.tokenize("== != ! && || < > <= >=");

	auto& tokens = tokenizer.tokens();

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
	tokenizer.tokenize("=! !! &| |& <> => =< <=>");

	auto& tokens = tokenizer.tokens();

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
	tokenizer.tokenize("& | ^ ~ << >> ");

	auto& tokens = tokenizer.tokens();

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
	tokenizer.tokenize("= += -= *= **= /= //= %= &&= ||= &= |= ^= ~= <<= >>=");

	auto& tokens = tokenizer.tokens();

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