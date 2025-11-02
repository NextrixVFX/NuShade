#pragma once

#include <impl/Util/Primitives.hxx>

namespace NuShade::Parser
{
	class c_SyntaxTree // aka AST (Abstract Syntax Tree)
	{
	private:
		size_t currentIndex = 0;

		std::vector<NuShade::Lexer::Token> Tokens;
		std::unique_ptr<NuShade::Parser::ShaderNode> Tree;
		
		auto GetCurrentToken() -> NuShade::Lexer::Token&
		{
			return Tokens[currentIndex];
		}

		auto Advance() -> void
		{
			if (currentIndex < Tokens.size())
				currentIndex++;
		}

		auto IsIdentifier(NuShade::Lexer::Token token) -> bool
		{
			return true;
		}
		
		auto IsKeyword(NuShade::Lexer::Token token) -> bool
		{
			return true;
		}

		auto IsNumber(NuShade::Lexer::Token token) -> bool
		{
			return true;
		}

		auto IsOperator(NuShade::Lexer::Token token) -> bool
		{
			return true;
		}

		auto IsSymbol(NuShade::Lexer::Token token) -> bool
		{
			return true;
		}

		auto IsEndOfFile(NuShade::Lexer::Token token) -> bool
		{
			return true;
		}

		auto IsUnknown(NuShade::Lexer::Token token) -> void
		{
			std::cout << "Invalid Syntax parsed at " << token.line << ":" << token.column << std::endl;
			std::cout << "[!]\t" << token.value << std::endl;
			std::cout << std::endl;
		}

		auto Parse() -> bool
		{

			if (Tokens.empty())
				return false;

			while (currentIndex < Tokens.size())
			{
				auto& t = GetCurrentToken();

				switch (t.type)
				{
				case NuShade::Lexer::TokenType::Unknown:
					IsUnknown(t); return false; break;

				case NuShade::Lexer::TokenType::Identifier:
					IsIdentifier(t); break;

				case NuShade::Lexer::TokenType::Keyword:
					IsKeyword(t); break;

				case NuShade::Lexer::TokenType::Number:
					IsNumber(t); break;

				case NuShade::Lexer::TokenType::Operator:
					IsOperator(t); break;

				case NuShade::Lexer::TokenType::Symbol:
					IsSymbol(t); break;

				case NuShade::Lexer::TokenType::EndOfFile:
					IsEndOfFile(t); break;
				}
			}

			return true;
		}

	public:
		c_SyntaxTree(std::vector<NuShade::Lexer::Token>& _tokens)
		{
			Tokens = _tokens;

			Tree = std::make_unique<NuShade::Parser::ShaderNode>();
			
			if (!Parse())
				return;

		}
	};
}