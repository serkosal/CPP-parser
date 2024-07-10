#pragma once
#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <token.hpp>

#include <set>
#include <string>
#include <vector>

/**

	\brief Tokenizer class is represents a set of rules of how to create tokens from string.

	Use public fields operators_start, operators, delimiters, brackets, forbidden
	for customise the work of tokenizing process.

**/
class Tokenizer
{
public:
	enum class State
	{
		start,

		new_token,

		identificator,
		integer,
		floating,

		commentary,

		_operator,

		string,
		string_escape,

		invalid,

		end
	};

	Tokenizer();

	const std::vector<Token>&	tokens()		const
	{
		return m_tokens;
	}
	const Token&				last_token()	const
	{
		return m_tokens.back();
	}
	const State&				cur_state()		const
	{
		return m_state;
	}

	void reset()
	{
		m_tokens.clear();
		m_cur_line = 1;
		m_cur_col = 0;
		m_state = State::new_token;
	}
	const std::vector<Token>& tokenize(const std::string& str);

private:
	std::set<char>				m_pot_op;		// potential operator start
	std::set<std::string>		m_actual_ops;		// actual operators
	std::set<char>				m_forbidden;		// forbidden for use characters
	std::set<char>				m_delimiters;		// delimiters char
	std::set<char>				m_brackets;			// brackets and parenthesis
	std::set<char>				m_escape_sequence;	// escape sequence

	std::vector<Token>			m_tokens;			//
	size_t						m_cur_line	= 1,
								m_cur_col	= 0;

	State						m_state = State::new_token;

	Token& last_token() { return m_tokens.back(); }

	void state_change(State new_state);
	void push_token(const Token& token)		{ m_tokens.push_back(token); }
};

#endif // !TOKENIZER_HPP