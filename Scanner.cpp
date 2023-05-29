#include <algorithm>
#include "Scanner.h"
using namespace std;
Scanner::Scanner(string source): source(source){}
vector<Token> Scanner::scanTokens(){
    while(!isAtEnd()){
        //We are at the beginning of the next lexeme
        start = current;
        scanToken();
    }
    Token token = createToken(TokenType::_EOF,"", nullptr,line);
    tokens.push_back(token);
    return tokens;
}

bool Scanner::isAtEnd(){
    return current >= source.length();
}

void Scanner::scanToken(){
    char c = advance();
    switch(c){
        //one token identifiers
        case '(': addToken(TokenType::LEFT_PAREN, "("); break;
        case ')': addToken(TokenType::RIGHT_PAREN, ")"); break;
        case '.': addToken(TokenType::DOT, "."); break;
        case '=': addToken(TokenType::EQUAL, "="); break;
        case '<':
            if(match('=')){
                addToken(TokenType::LESS_EQUAL,"<=");
            }
            else if(match('>')){
                addToken(TokenType::NOT_EQUAL,"<>");
            }
            else{
                addToken(TokenType::LESS, "<");
            }
            break;
        case '>':
            if(match('=')){
                addToken(TokenType::GREATER_EQUAL, ">=");
            }
            else{
                addToken(TokenType::GREATER, ">");
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;

        case '\n':
            line++;
            break;

            //literals
        case '\'': _string(); break;
        default:
            if (isDigit(c)) {
                number();
            }
            else if (isAlpha(c)){
                identifier();
            }
            else {
                error(line, "Unexpected character.");
            }
            break;
    }
}

void Scanner::_string(){
    while (peek() != '\'' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }
    if (isAtEnd()) {
        error(line, "Unterminated string.");
        return;
    }
    // The closing ".
    advance();

    //Trim the surrounding quotes
    string value = source.substr(start+1,current-1-(start+1));

    addToken(TokenType::STRING, value, value);
}

bool Scanner::isDigit(char c){
    return c >= '0' && c <= '9';
}

void Scanner::number(){
    while(isDigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the "."
        advance();

        while (isDigit(peek())) advance();
    }
    addToken(TokenType::NUMBER,source.substr(start, current-start),stod(source.substr(start, current-start)));
}
bool Scanner::isAlpha(char c){
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_' || c == ':';
}

void Scanner::identifier(){
    while (isAlphaNumeric(peek())) advance();

    string text = source.substr(start,current-start);
    TokenType type;

    //no type found
    string up_text = text;
    // convert string to back to lower case
    transform(up_text.begin(), up_text.end(), up_text.begin(), ::toupper);
    if(keywords.find(up_text) == keywords.end()){
        type = TokenType::IDENTIFIER;
    }
    else{
        type = keywords[up_text];
    }
    addToken(type,text,nullptr);
}

bool Scanner::isAlphaNumeric(char c){
    return isAlpha(c) || isDigit(c);
}
char Scanner::advance(){
    return source.at(current++);
}
void Scanner::addToken(TokenType type, string lexeme){
    addToken(type, lexeme, nullptr);
}
void Scanner::addToken(TokenType type, string text, any literal){
    tokens.push_back(createToken(type,text,literal,line));
}

Token Scanner::createToken(TokenType type, string lexeme, any literal, int line){
    Token token = {type, lexeme,literal,line};
    return token;
}

bool Scanner::match(char expected){
    if(isAtEnd()) return false;
    if(source.at(current) != expected) return false;
    current++;
    return true;
}

char Scanner::peek(){
    if(isAtEnd()) return '\0';
    return source.at(current);
}
char Scanner::peekNext(){
    if (current + 1 >= source.length()) return '\0';
    return source.at(current + 1);
}

void Scanner::error(int line, string message){
    report(line, "", message);
}

void Scanner::report(int line, string where, string message){
    cerr << "[line " << to_string(line) <<"] Error" << where << ": " << message << endl;
    hadError = true;
}