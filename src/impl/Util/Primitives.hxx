#pragma once

struct iVector2
{
    int32_t x = 0;
    int32_t y = 0;

    iVector2() = default;
    iVector2(int32_t _x, int32_t _y) : x(_x), y(_y) {}

    iVector2 operator+(const iVector2& coord) const { return { x + coord.x, y + coord.y }; }
    iVector2 operator-(const iVector2& coord) const { return { x - coord.x, y - coord.y }; }
    iVector2 operator*(const iVector2& coord) const { return { x * coord.x, y * coord.y }; }
    iVector2 operator/(const iVector2& coord) const { return { x / coord.x, y / coord.y }; }
};

namespace NuShade::Parser
{
    enum class ShaderType
    {
        fragment, vertex
    };

    enum class NodeType
    {
        // https://astexplorer.net/ was really helpful
        Shader,
        VariableDeclaration,
        VariableDeclarator,
        FunctionDeclaration,
        BlockStatement,
        ExpressionStatement,
        CallExpression,
        MemberExpression,
        BinaryExpression,
        Literal,
        Identifier,
        Parameter,
    };
    
    struct ShaderNode;
    struct LiteralNode;

    struct NodeVisitor
    {
        virtual void visit(ShaderNode&) = 0;
        virtual void visit(LiteralNode&) = 0;
        // add other node types as needed
        virtual ~NodeVisitor() = default;
    };

    struct Node
    {
        iVector2 sel{ 0, 0 };
        NodeType type;

        explicit Node(NodeType t) : type(t) {}
        virtual ~Node() = default;
        virtual void accept(NodeVisitor&) = 0;
    };

    struct ShaderNode : Node
    {
        void accept(NodeVisitor& v) override { v.visit(*this); }
        
        std::vector<std::unique_ptr<Node>> scope;
        std::vector<std::unique_ptr<Node>> body;
        ShaderType shaderType;

        ShaderNode() : Node(NodeType::Shader) {}
        
    };

    struct LiteralNode : Node
    {
        void accept(NodeVisitor& v) override { v.visit(*this); }
        
        std::string value;
        std::string kind;

        LiteralNode() : Node(NodeType::Literal) {}
    };

    struct IdentifierNode : Node
    {
        std::string name;

        IdentifierNode() : Node(NodeType::Identifier) {}
    };

    struct ParameterNode : Node
    {
        std::string name;
        std::string kind;

        ParameterNode() : Node(NodeType::Parameter) {}
    };

    struct BinaryExpressionNode : Node
    {
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        std::string op;

        BinaryExpressionNode() : Node(NodeType::BinaryExpression) {}
    };

    struct VariableDeclaratorNode : Node
    {
        std::unique_ptr<IdentifierNode> id;
        std::unique_ptr<Node> init;

        VariableDeclaratorNode() : Node(NodeType::VariableDeclarator) {}
    };

    struct VariableDeclarationNode : Node
    {
        std::string kind;
        std::vector<std::unique_ptr<VariableDeclaratorNode>> declarations;

        VariableDeclarationNode() : Node(NodeType::VariableDeclaration) {}
    };

    struct MemberExpressionNode : Node
    {
        std::unique_ptr<IdentifierNode> object; // object that is member-of
        std::unique_ptr<IdentifierNode> property; 

        MemberExpressionNode() : Node(NodeType::MemberExpression) {}
    };

    struct CallExpressionNode : Node
    {
        std::unique_ptr<Node> callee;
        std::vector<std::unique_ptr<Node>> arguments;

        CallExpressionNode() : Node(NodeType::CallExpression) {}
    };

    struct ExpressionStatementNode : Node
    {
        std::unique_ptr<CallExpressionNode> expression;
        
        ExpressionStatementNode() : Node(NodeType::ExpressionStatement) {}
    };

    struct BlockStatementNode : Node
    {
        std::vector<std::unique_ptr<Node>> body;
        
        BlockStatementNode() : Node(NodeType::BlockStatement) {}
    };

    struct FunctionDeclarationNode : Node
    {
        std::unique_ptr<IdentifierNode> id;
        std::unique_ptr<BlockStatementNode> body;
        std::vector<std::unique_ptr<ParameterNode>> params;

        FunctionDeclarationNode() : Node(NodeType::FunctionDeclaration) {}
    };
}

namespace NuShade::Lexer
{
    inline static std::vector<const char*> Keywords
    {
        "float", "vec2", "vec3", "vec4", "uniform", "void", "return"
    };
    
    enum class TokenType
    {
        Identifier,     // variable names, function names
        Keyword,        // float, vec3, uniform, etc.
        Number,         // numeric constants
        Operator,       // + - * / = == < >
        Symbol,         // punctuation like ( ) { } ; , .
        EndOfFile,      // EOF marker
        Unknown
    };

    struct Token
    {
        TokenType type;
        std::string value;
        size_t line;
        size_t column;
    };
}