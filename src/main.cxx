#include <includes.h>

static std::unique_ptr<NuShade::File::c_ShaderData> shader;
static std::unique_ptr<NuShade::Lexer::c_Tokenizer> tokenizer;
static std::unique_ptr<NuShade::Parser::c_SyntaxTree> syntaxTree;


auto main() -> int
{
    std::string_view file = "src/examples/test2.nus";
    shader = std::make_unique<NuShade::File::c_ShaderData>(file);
    
    std::string buffer;
    shader->GetLines(buffer);

    if (buffer.empty())
    {
        std::cout << "couldnt get file" << std::endl;
        std::cin.get();
        return 1;
    }

    tokenizer = std::make_unique<NuShade::Lexer::c_Tokenizer>(buffer);
    std::vector<NuShade::Lexer::Token> Tokens = tokenizer->GetTokens();

    for (auto& t : Tokens)
    {
        std::cout << (int)t.type << " : " << t.value << std::endl;
    }

    syntaxTree = std::make_unique<NuShade::Parser::c_SyntaxTree>(Tokens);
    
    std::cin.get();
    return 1;
}
