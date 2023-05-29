#pragma once
#ifndef BSQL_SCANNER_H
#define BSQL_SCANNER_H
#include <string>
#include <vector>
#include "TokenType.h"


using namespace std;
class Scanner {
public:
    Scanner(string source);
    vector<Token> scanTokens();
    unordered_map<string, TokenType> create_keyword_map(){
        unordered_map<string, TokenType> map;
        map["AND"] = TokenType::AND;
        map["OR"] = TokenType::OR;
        map["NONE"] =  TokenType::NONE;
        map["QUERY_TYPE:"] =   TokenType::QUERY_TYPE;
        map["TABLES:"] =  TokenType::TABLES;
        map["FILTERS:"] =    TokenType::FILTERS;
        map["ALL"] =    TokenType::ALL;
        map["MATCHING_COLUMNS:"] =     TokenType::MATCHING_COLUMNS;
        map["DELETE_OBJECT:"] =     TokenType::DELETE_OBJECT;
        map["INSERT_OBJECT:"] =     TokenType::INSERT_OBJECT;
        map["RESULTS_ORDER:"] =    TokenType::RESULTS_ORDER;
        map["DEFAULT"] =     TokenType::DEFAULT;
        map["COLUMNS_DISPLAYED:"] =  TokenType::COLUMNS_DISPLAYED;
        map["DESC"] = TokenType::DESC;
        map["ASC"] =  TokenType::ASC;
        map["COUNT"] =   TokenType::COUNT;
        map["MIN"] =   TokenType::MIN;
        map["MAX"] =    TokenType::MAX;
        map["AVG"] =  TokenType::AVERAGE;
        map["SUM"] = TokenType::SUM;
        map["DELETE"] =  TokenType::DELETE;
        map["COUNT"] =   TokenType::COUNT;
        map["MIN"] =   TokenType::MIN;
        map["MAX"] =    TokenType::MAX;
        map["SUM"] =  TokenType::SUM;
        map["DELETE"] = TokenType::DELETE;
        map["TABLE"] =  TokenType::TABLE;
        map["ROW"] =   TokenType::ROW;
        map["COLUMN"] =   TokenType::COLUMN;
        map["INSERT"] =    TokenType::INSERT;
        map["TABLE_TO_INSERT_INTO:"] =  TokenType::TABLE_TO_INSERT_INTO;
        map["VALUES:"] =    TokenType::VALUES_TO_INSERT;
        map["COLUMNS:"] =    TokenType::COLUMNS_TO_INSERT;
        map["COLUMN:"] =    TokenType::COLUMN_TO_INSERT;
        map["QUESTION"] =    TokenType::QUESTION;
        map["DISTINCT"] =    TokenType::DISTINCT;
        map["COLUMN_GROUPING:"] =    TokenType::COLUMN_GROUPING;
        map["GROUPING"] =    TokenType::GROUPING;
        map["NUMBER"] = TokenType::isNumber;
        map["LETTERS"] = TokenType::isLetters;
        map["TABLE_NAME:"] = TokenType::TABLE_NAME;
        map["NULL"] = TokenType::_NULL;

        return map;
    }
    unordered_map<string, TokenType> keywords = create_keyword_map();
private:
    const string source;
    vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    bool isAtEnd();

    void scanToken();

    char advance();

    void addToken(TokenType type, string lexeme);
    void addToken(TokenType type, string text, any literal);

    Token createToken(TokenType type, string lexeme, any literal, int line);

    bool match(char expected);

    char peek();
    void _string();

    bool isDigit(char c);

    void number();

    char peekNext();

    void identifier();

    bool isAlpha(char c);

    bool isAlphaNumeric(char c);

    void error(int line, string message);

    void report(int line, string where, string message);
};


#endif //BSQL_SCANNER_H
