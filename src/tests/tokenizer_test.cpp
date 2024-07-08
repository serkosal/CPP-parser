#include <gtest/gtest.h>

#include "tokenizer.hpp"

Tokenizer tokenizer;

TEST(IdentificatorCreation, SimpleIdentificator)
{
	tokenizer.reset();
	tokenizer.tokenize("variable");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_TRUE(tokens[0].m_type == Token::Type::identificator);
}

TEST(IdentificatorCreation, IdSpaceSeparated)
{
	tokenizer.reset();
	tokenizer.tokenize("first second\nthird\tfourth");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(4));

	EXPECT_TRUE(tokens[0].m_type == Token::Type::identificator);
	EXPECT_TRUE(tokens[1].m_type == Token::Type::identificator);
	EXPECT_TRUE(tokens[2].m_type == Token::Type::identificator);
	EXPECT_TRUE(tokens[3].m_type == Token::Type::identificator);
}

TEST(IdentificatorCreation, IdWithUnderscores)
{
	tokenizer.reset();
	tokenizer.tokenize("var _var va_r var_ rav");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(5));

	EXPECT_TRUE(tokens[0].m_type == Token::Type::identificator);
	EXPECT_TRUE(tokens[1].m_type == Token::Type::identificator);
	EXPECT_TRUE(tokens[2].m_type == Token::Type::identificator);
	EXPECT_TRUE(tokens[3].m_type == Token::Type::identificator);
	EXPECT_TRUE(tokens[4].m_type == Token::Type::identificator);
}

TEST(IdentificatorCreation, IdWithDigits)
{
	tokenizer.reset();
	tokenizer.tokenize("var var1 va2r 3var");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(4));

	EXPECT_TRUE(tokens[0].m_type == Token::Type::identificator);
	EXPECT_TRUE(tokens[1].m_type == Token::Type::identificator);
	EXPECT_TRUE(tokens[2].m_type == Token::Type::identificator);
	EXPECT_TRUE(tokens[3].m_type == Token::Type::invalid);
}

TEST(IdentificatorCreation, IdWithDigitsUnderscore)
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

	EXPECT_TRUE(tokens[0].m_type == Token::Type::identificator);	// var
	EXPECT_TRUE(tokens[1].m_type == Token::Type::identificator);	// _1var
	EXPECT_TRUE(tokens[2].m_type == Token::Type::identificator);	// _va2r
	EXPECT_TRUE(tokens[3].m_type == Token::Type::identificator);	// _var3
	EXPECT_TRUE(tokens[4].m_type == Token::Type::invalid);			// 4_var
	EXPECT_TRUE(tokens[5].m_type == Token::Type::invalid);			// 5va_r
	EXPECT_TRUE(tokens[6].m_type == Token::Type::invalid);			// 6var_
	EXPECT_TRUE(tokens[7].m_type == Token::Type::identificator);	// v7_ar
	EXPECT_TRUE(tokens[8].m_type == Token::Type::identificator);	// va8_r
	EXPECT_TRUE(tokens[9].m_type == Token::Type::identificator);	// var9_
	EXPECT_TRUE(tokens[10].m_type == Token::Type::identificator);	// var_10
}


TEST(IntegerCreation, IntegerPositive)
{
	tokenizer.reset();
	tokenizer.tokenize("1234567890");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_TRUE(tokens[0].m_type == Token::Type::integer);
}

TEST(IntegerCreation, IntegerNegative)
{
	tokenizer.reset();
	tokenizer.tokenize("-30");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_TRUE(tokens[0].m_type == Token::Type::integer);
}

TEST(IntegerCreation, IntegerZero)
{
	tokenizer.reset();
	tokenizer.tokenize("0");

	auto& tokens = tokenizer.tokens();

	ASSERT_EQ(tokens.size(), size_t(1));

	EXPECT_TRUE(tokens[0].m_type == Token::Type::integer);
}