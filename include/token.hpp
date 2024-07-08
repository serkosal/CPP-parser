#pragma once
#ifndef TOKEN_HPP
#define TOKEN_HPP


#include <string>
#include <vector>

/**
	\brief Token class is represents an lexem with some semantic meanings with it.
**/
struct Token
{
	enum class Type
	{
		empty,
		invalid = -1,

		integer,
		floating,
		string,

		commentary,

		identificator,
		keyword,

		bracket,

		_operator,
	};

	size_t		m_line	= 0,
				m_col	= 0;
	Type		m_type	= Type::empty;
	std::string m_value	= "";

	Token(
		size_t		line	= 0, 
		size_t		column	= 0, 
		Type		type	= Type::empty,
		std::string value	= ""
	)	
		: m_type(type), m_value(value), m_line(line), m_col(column)
	{}
};

#endif // !TOKEN_HPP