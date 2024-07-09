#include "tokenizer.hpp"

Tokenizer::Tokenizer()
{
	m_state = State::new_token;
	m_cur_line = 1;
	m_cur_col = 0;

	m_pot_op_start = "+-*/%" "=!<>" "&|^~" ",";
	m_operators =
	{
		"+", "-", "*", "**", "/", "//", "%", "++", "--",
		"==", "!=",
		"<", "<=", ">", ">=",
		"&&", "||",
		"&", "|",  "^", "~", "<<", ">>"
		"=",
		"+=", "-=", "*=", "**=", "/=", "//=", "%=",
		"&&=", "||=",
		"&=", "|=", "^=", "~=", "<<=", ">>=",
		","
	};
	m_escape_sequence = "ntvabfr\\\"";
	m_forbidden = "#$':?@\\`";

	m_brackets = "(){}[]";

	m_delimiters = ";," + m_pot_op_start + m_brackets;
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
		last_token().m_type = Token::Type::invalid;
		break;
	}
}

void Tokenizer::tokenize(const std::string& str)
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
			else if (m_pot_op_start.find(cur_char) != std::string::npos)
				state_change(State::_operator);
			// brackets and parenthesis
			else if (m_brackets.find(cur_char) != std::string::npos)
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
			if (m_delimiters.find(cur_char) != std::string::npos)
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
			if (m_escape_sequence.find(cur_char) != std::string::npos)
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
			if (m_delimiters.find(cur_char) != std::string::npos)
			{
				state_change(State::new_token);
				++i;
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
			if (m_delimiters.find(cur_char) != std::string::npos)
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
			if (m_delimiters.find(cur_char) != std::string::npos)
			{
				state_change(State::new_token);
				++i;
				continue;
			}
			break;
		}
		case State::_operator:
		{
			if (m_pot_op_start.find(cur_char) != std::string::npos)
			{
				auto cur_pot_op = last_token().m_value;

				if (std::find(m_operators.begin(), m_operators.end(), cur_pot_op + cur_char) != m_operators.end())
					state_change(State::new_token);
				else if (std::find(m_operators.begin(), m_operators.end(), cur_pot_op) != m_operators.end())
				{
					state_change(State::new_token);
					++i;
					continue;
				}
				else
					state_change(State::invalid);
			}
			break;
		}
		}

		++i;
		last_token().m_value += cur_char;
	}

	if (m_state == State::string || m_state == State::string_escape)
		last_token().m_type = Token::Type::invalid;
	if (m_state == State::_operator)
		last_token().m_type = Token::Type::invalid;
}