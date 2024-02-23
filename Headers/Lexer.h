#pragma once
#include <cstddef>
#include <string>

typedef enum {
    TOKEN_END = 0,
    TOKEN_INVALID,    
    TOKEN_SYMBOL,
    TOKEN_NAME_CHARA,
    TOKEN_STRING_LITERAL,    
    TOKEN_EQUALS,
    TOKEN_NUMBER,
    TOKEN_COMMENT  
} Token_Kind;

typedef struct {
    Token_Kind kind;
    std::string text;
} Token;

class Lexer{
    public:        
        Lexer(std::string content);
        Lexer();
        ~Lexer();
        Token next();        
        std::string token_kind_name(Token_Kind kind);
    private:
        void trim_left();
        std::string content;
        size_t content_len;     
        size_t cursor;
        size_t line;
        void chop_char(size_t len);
        bool is_symbol_start(char x);
        bool is_symbol(char x);
        bool is_eof();
        char current_character();        
};
