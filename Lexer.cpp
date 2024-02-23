#include "./Headers/Lexer.h"
#include <map>
std::map<char, Token_Kind> map_tokens = {
    {'=', TOKEN_EQUALS},
    {'[', TOKEN_OPEN_SQRBRACKET},
    {']',TOKEN_CLOSE_SQRBRACKET},
    {':',TOKEN_COLON},
};


Lexer::Lexer(std::string content){
    this->content = content;
    this->content_len = content.length();
    this->cursor = 0;
    this->line = 0;
}
Lexer::Lexer(){

}

Lexer::~Lexer(){
    
}

void Lexer::chop_char(size_t len = 1){
    for(size_t i = 0; i < len; i++){
        // assert(l->cursor < l->content_len);
        char x = this->content[this->cursor];
        this->cursor += 1;        
        if (x == '\n')
        {
            this->line += 1;            
        }
    }      
}

bool Lexer::is_symbol_start(char x)
{
    return isalpha(x) || x == '_';
}

bool Lexer::is_symbol(char x)
{
    return isalnum(x) || x == '_' || isspace(x);
}

void Lexer::trim_left(){
    while(this->cursor < this->content_len && isspace(this->content[this->cursor])){
        this->chop_char();
    }
}
bool Lexer::is_eof(){
    return this->cursor >= this->content_len;
}
bool map_contains(char c){
    return map_tokens.find(c) != map_tokens.end();
}

char Lexer::current_character(){
    return this->content[this->cursor];
}

Token Lexer::next(){
    this->trim_left();
    Token token = {
        .kind = TOKEN_END,
        .text = "",        
    };
    size_t len = 0;
    size_t start = this->cursor;
    if(this->is_eof()) return token;
    if(map_contains(this->current_character())){
        token.kind = map_tokens[this->current_character()];
        token.text = this->content.substr(start,1);
        this->chop_char();
        return token;
    }

    if(this->current_character() == '%'){
        token.kind = TOKEN_COMMENT;
        while(!this->is_eof() && this->current_character() !='\n'){
            this->chop_char();
            len+=1;
        }
        token.text = this->content.substr(start,len);                
        return token;   
    }
    if( isdigit( this->current_character() ) || this->current_character() == '-' ){
        token.kind = TOKEN_NUMBER;
        if(this->current_character() == '-'){
            len += 1;
            this->chop_char();
        }
            
        while(!this->is_eof() && (isdigit(this->current_character()) || this->current_character() == '.' )  && this->current_character() != '\n'){
            this->chop_char();
            len += 1;
        }
        token.text = this->content.substr(start,len);
        return token;
    }

    if(is_symbol_start( this->current_character() )){
        
        while(!this->is_eof() && this->current_character() != ' ' && this->is_symbol(this->current_character()) && this->current_character() != '\n'){
            this->chop_char();
            len += 1;
        }  
        
        token.text = this->content.substr(start,len);        
        token.kind = TOKEN_SYMBOL;
        return token;
    }
    // if( this->current_character() == '[' ){
    //     token.kind = TOKEN_NAME_CHARA;
        
    //     this->chop_char();
    //     while(!this->is_eof() && (this->current_character() != ']')){
    //         this->chop_char();
    //         len += 1;
    //     }
    //     if(!this->is_eof()){
    //         len += 1;
    //         this->chop_char();
    //     }
    //     token.text = this->content.substr(start + 1,len - 1);
    //     return token;
    // }
    if(this->current_character() == '"'){
        token.kind = TOKEN_STRING_LITERAL;
        
        this->chop_char();
        while(!this->is_eof() && this->current_character() != '"'){
            this->chop_char();
            len += 1;
        }
        if(!this->is_eof()){
            this->chop_char();
            len += 1;
        }
        token.text = this->content.substr(start + 1,len - 1);
        return token;
    }
    this->chop_char();
    token.kind = TOKEN_INVALID;
    token.text = "";
    return token;
}

std::string Lexer::token_kind_name(Token_Kind kind){
    switch(kind){
        case TOKEN_END: return "token kind end";
        case TOKEN_INVALID: return "token kind invalid";
        case TOKEN_SYMBOL: return "token kind symbol";
        case TOKEN_NAME_CHARA: return "token kind name chara";
        case TOKEN_STRING_LITERAL: return "token kind string literal";
        case TOKEN_EQUALS: return "token kind equals";
        case TOKEN_COMMENT: return "token kind coment";
        case TOKEN_NUMBER: return "token kind number";
        case TOKEN_CLOSE_SQRBRACKET: return "token close bracket";
        case TOKEN_OPEN_SQRBRACKET: return "token open bracket";
        case TOKEN_COLON: return "token colon";
    }
    return "NULL";
}