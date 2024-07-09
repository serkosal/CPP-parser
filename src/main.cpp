#include <iostream>

#include "tokenizer.hpp"

void print_tokens(const std::vector<Token>& tokens)
{
	for (auto& el : tokens)
	{
		std::cout << "Token value: " << el.m_value;

		switch (el.m_type)
		{
		case Token::Type::invalid:
			std::cout << " of invalid type\n";
			break;
		case Token::Type::integer:
			std::cout << " of integer type";
			break;
		case Token::Type::floating:
			std::cout << " of floating type";
			break;
		case Token::Type::string:
			std::cout << " of string type";
			break;
		case Token::Type::identificator:
			std::cout << " of identificator type";
			break;
		case Token::Type::keyword:
			std::cout << " of keyword type";
			break;
		case Token::Type::bracket:
			std::cout << " of bracket type";
			break;
		case Token::Type::_operator:
			std::cout << " of operator type";
			break;
		}
		std::cout << "\n\n";
	}

}

int main()
{
	Tokenizer tokenizer;

	std::string line;
	while (std::getline(std::cin, line))
	{
		tokenizer.tokenize(line);
		print_tokens(tokenizer.tokens());
		tokenizer.reset();
	}


	return 0;
}