#pragma once

#include <impl/Util/Primitives.hxx>

namespace NuShade::Lexer
{	
	class c_Tokenizer
	{
	private:
		std::string shaderData;
		std::vector<Token> tokens;

		auto IsOperatorChar(char c) const -> bool
		{
			static const std::string ops = "+-*/=<>!&|";
			return ops.find(c) != std::string::npos;
		}

		auto IsWhiteSpace(const char& _char, size_t& _line, size_t& _col, size_t& _index) -> bool
		{
			if (!isspace(_char))
				return false;

			if (_char == '\n')
			{
				_col = 1;
				_line++;
				_index++;
				return true;
			}

			_index++; _col++;

			return true;
		}

		auto IsAlphanumerical(const char& _char, size_t& _line, size_t& _col, size_t& _index) -> bool
		{
			const size_t start = _index;
			const size_t len = shaderData.size();

			if (!isalpha(_char) && _char != '_')
				return false;

			// loop till nonalpha character
			while (_index < len && (isalnum(shaderData[_index]) || shaderData[_index] == '_'))
				_index++;
			
			std::string value = shaderData.substr(start, _index - start);

			TokenType type = TokenType::Identifier;
			if (std::find(Keywords.begin(), Keywords.end(), value) != Keywords.end())
				type = TokenType::Keyword;

			tokens.push_back({ type, value, _line, _col });
			_col += value.size();

			return true;
		}

		auto IsNumerical(const char& _char, size_t& _line, size_t& _col, size_t& _index) -> bool
		{
			if (!isdigit(_char))
				return false;
			
			size_t start = _index;
			const size_t len = shaderData.size();

			while (_index < len && (isdigit(shaderData[_index]) || shaderData[_index] == '.'))
				_index++;

			std::string value = shaderData.substr(start, _index - start);
			tokens.push_back({ TokenType::Number, value, _line, _col });

			_col += value.size();
			return true;
		}

		auto IsOperator(const char& _char, size_t& _line, size_t& _col, size_t& _index) -> bool
		{
			bool isOperator = IsOperatorChar(_char);
			bool isSymbol = ispunct(_char);

			if (!isOperator && !isSymbol)
				return false;

			tokens.push_back({
				(isOperator) ? TokenType::Operator : TokenType::Symbol,
				std::string(1, _char),
				_line, _col
			});

			_index++; _col++;
			
			return true;
		}

		auto Tokenize(std::vector<Token>& _buffer) -> bool
		{
			size_t i = 0;
			size_t line = 1, col = 1;
			const size_t len = shaderData.size();

			while (i < len)
			{
				char c = shaderData[i];

				// Skip Whitespace
				if (IsWhiteSpace(c, line, col, i))
					continue;

				// Detect Keywords
				if (IsAlphanumerical(c, line, col, i))
					continue;

				// Detect Numbers
				if (IsNumerical(c, line, col, i))
					continue;

				// Detect Operators and Symbols
				if (IsOperator(c, line, col, i))
					continue;

				tokens.push_back({ TokenType::Unknown, std::string(1, c), line, col });
				i++; col++;
			}

			return true;
		}

	public:
		auto GetTokens() -> std::vector<Token>
		{
			return tokens;
		}
		
		c_Tokenizer(std::string& _data)
		{
			shaderData = _data;
			
			if (!Tokenize(tokens))
				return;

		}
	};
}