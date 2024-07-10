#include "tokenizer.hpp"

Tokenizer::Tokenizer()
{
	m_state = State::new_token;
	m_cur_line = 1;
	m_cur_col = 0;

	m_pot_op.insert ({ '+', '-', '*', '/', '%', '=', '!', '<', '>', '&', '|', '^', '~', ',', ';'});
	m_actual_ops.insert(
	{
		"+", "-", "*", "**", "/", "//", "%", "++", "--",
		"==", "!", "!=",
		"<", "<=", ">", ">=",
		"&&", "||",
		"&", "|",  "^", "~", "<<", ">>",
		"=",
		"+=", "-=", "*=", "**=", "/=", "//=", "%=",
		"&&=", "||=",
		"&=", "|=", "^=", "~=", "<<=", ">>=",
		",", ";"
	});
	m_escape_sequence.insert({ 'n', 't', 'v', 'a', 'b', 'f', 'r', '\\', '\"' });
	m_forbidden.insert({ '#','$', ':', '?', '@', '/', '`' });

	m_brackets.insert({ '(', ')', '{', '}', '[', ']' });

	m_delimiters.insert(m_pot_op.begin(), m_pot_op.end());
	m_delimiters.insert(m_brackets.begin(), m_brackets.end());
	m_delimiters.insert(m_forbidden.begin(), m_forbidden.end());
}

void Tokenizer::state_change(State new_state)
{
	m_state = new_state;

	switch (new_state)
	{
	case State::identificator:
		last_token().m_type = Token::Type::identificator;
		break;
	case State::commentary:
		last_token().m_type = Token::Type::commentary;
		break;
	case State::string:
		last_token().m_type = Token::Type::string;
		break;
	case State::integer:
		last_token().m_type = Token::Type::integer;
		break;
	case State::floating:
		last_token().m_type = Token::Type::floating;
		break;
	case State::invalid:
		last_token().m_type = Token::Type::invalid;
		break;
	case State::_operator:
		last_token().m_type = Token::Type::_operator;
		break;
	}
}

const std::vector<Token>& Tokenizer::tokenize(const std::string& str)
{
	auto n = str.size();

	for (int i = 0; i < n;)
	{
		auto cur_char = str[i];

		if (isspace(cur_char))
		{
			if (cur_char == '\n')
			{
				++m_cur_line;
				m_cur_col = 1;

				if (m_state == State::commentary)
					m_state = State::new_token;
			}

			if (m_state == State::string)
				last_token().m_value += cur_char;
			else if (m_state == State::string_escape)
				state_change(State::invalid);
			else if (m_state == State::integer && last_token().m_value == "-")
			{
				m_tokens.back().m_type = Token::Type::_operator;
				state_change(State::new_token);
			}
			else
				state_change(State::new_token);

			++i;
			continue;
		}

		++m_cur_col;

		switch (m_state)
		{
		case State::new_token:
		{
			push_token({ m_cur_line, m_cur_col });

			// integer literals
			if (cur_char == '-' || isdigit(cur_char))
				state_change(State::integer);
			// identificators
			else if (cur_char == '_' || isalpha(cur_char))
				state_change(State::identificator);
			// floating pointer numbers
			else if (cur_char == '.')
				state_change(State::floating);
			// string literals
			else if (cur_char == '"')
				state_change(State::string);
			// commented lines
			else if (cur_char == '#')
				state_change(State::commentary);
			// operators
			else if (m_pot_op.count(cur_char))
				state_change(State::_operator);
			// brackets and parenthesis
			else if (m_brackets.count(cur_char))
			{
				last_token().m_type = Token::Type::bracket;
				state_change(State::new_token);
			}
			else
				state_change(State::invalid);

			break;
		}
		case State::identificator:
		{
			if (m_delimiters.count(cur_char))
			{
				state_change(State::new_token);
				++i;
				continue;
			}
			else if (!isalnum(cur_char) && cur_char != '_')
				state_change(State::invalid);
			break;
		}
		case State::string:
		{
			if (cur_char == '"')
				state_change(State::new_token);
			else if (cur_char == '\\')
			{
				state_change(State::string_escape);
				++i;
				continue;
			}

			break;
		}
		case State::string_escape:
		{
			if (m_escape_sequence.count(cur_char))
				state_change(State::string);
			else
			{
				last_token().m_value += '\\';
				state_change(State::invalid);
			}
			break;
		}
		case State::integer:
		{
			if (last_token().m_value == "-" && m_pot_op.count(cur_char))
			{
				if (cur_char == '-' || cur_char == '=')
					state_change(State::_operator);
				else
					state_change(State::invalid);
			}
			else if (m_delimiters.count(cur_char))
			{
				state_change(State::new_token);
				continue;
			}
			else if (cur_char == '.')
				state_change(State::floating);
			else if (!isdigit(cur_char))
				state_change(State::invalid);
			break;
		}
		case State::floating:
		{
			if (m_delimiters.count(cur_char))
			{
				state_change(State::new_token);
				++i;
				continue;
			}
			else if (!isdigit(cur_char))
				state_change(State::invalid);
			break;
		}
		case State::invalid:
		{
			if (m_delimiters.count(cur_char))
			{
				state_change(State::new_token);
				++i;
				continue;
			}
			break;
		}
		case State::_operator:
		{
			auto cur_pot_op = last_token().m_value;
			// if cur char is in the list of operator's characters 
			// then we will check is that sequence form an valid operator
			if (m_pot_op.count(cur_char))
			{
				if (m_actual_ops.count(cur_pot_op + cur_char) == 0)
					state_change(State::invalid);
			}
			else
			{
				if (m_actual_ops.count(cur_pot_op))
				{
					state_change(State::new_token);
					continue;
				}

				else
				{
					last_token().m_type = Token::Type::invalid;
					state_change(State::new_token);
					++i;
					continue;
				}
			}
		}
		}

		++i;
		last_token().m_value += cur_char;
	}

	if (m_state == State::string || m_state == State::string_escape)
		last_token().m_type = Token::Type::invalid;
	if (m_state == State::_operator && m_actual_ops.count(last_token().m_value) == 0)
		last_token().m_type = Token::Type::invalid;

	return m_tokens;
}