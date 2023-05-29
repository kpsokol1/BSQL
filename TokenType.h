#pragma once
#ifndef C_LOX_TOKENTYPE_H
#define C_LOX_TOKENTYPE_H
#include <unordered_map>
#include "global.h"
using namespace std;
enum class TokenType{
    LEFT_PAREN, RIGHT_PAREN, DOT,

    NOT_EQUAL,
    EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    IDENTIFIER, STRING, NUMBER,

    AND, NONE, OR,
    QUERY_TYPE, TABLES,FILTERS,ALL,
    MATCHING_COLUMNS,RESULTS_ORDER,DEFAULT,COLUMNS_DISPLAYED,ASC,DESC,
    COUNT,MIN,MAX,AVERAGE,SUM,DELETE,DELETE_OBJECT,INSERT_OBJECT,TABLE,ROW,COLUMN, INSERT,TABLE_TO_INSERT_INTO,
    VALUES_TO_INSERT,COLUMNS_TO_INSERT,COLUMN_TO_INSERT,TABLE_NAME,QUESTION, DISTINCT, COLUMN_GROUPING, GROUPING, isNumber, isLetters,_NULL,

    _EOF
};


typedef struct{
    const TokenType type;
    const string lexeme;
    const any literal;
    const int line;
    string to_string(){
        unordered_map<TokenType, string> token_names;
        token_names[TokenType::LEFT_PAREN] = "LEFT_PAREN";
        token_names[TokenType::RIGHT_PAREN] = "RIGHT_PAREN";
        token_names[TokenType::NOT_EQUAL] = "NOT_EQUAL";
        token_names[TokenType::NONE] = "NONE";
        token_names[TokenType::QUERY_TYPE] = "QUERY_TYPE";
        token_names[TokenType::DOT] = "DOT";
        token_names[TokenType::TABLES] = "TABLES";
        token_names[TokenType::FILTERS] = "FILTERS";
        token_names[TokenType::ALL] = "ALL";
        token_names[TokenType::MATCHING_COLUMNS] = "MATCHING_COLUMNS";
        token_names[TokenType::DELETE_OBJECT] = "DELETE_OBJECT";
        token_names[TokenType::INSERT_OBJECT] = "INSERT_OBJECT";
        token_names[TokenType::RESULTS_ORDER] = "RESULTS_ORDER";
        token_names[TokenType::DEFAULT] = "DEFAULT";
        token_names[TokenType::COLUMNS_DISPLAYED] = "COLUMNS_DISPLAYED";
        token_names[TokenType::EQUAL] = "EQUAL";
        token_names[TokenType::ASC] = "ASC";
        token_names[TokenType::GREATER] = "GREATER";
        token_names[TokenType::GREATER_EQUAL] = "GREATER_EQUAL";
        token_names[TokenType::LESS] = "LESS";
        token_names[TokenType::LESS_EQUAL] = "LESS_EQUAL";
        token_names[TokenType::IDENTIFIER] = "IDENTIFIER";
        token_names[TokenType::STRING] = "STRING";
        token_names[TokenType::NUMBER] = "NUMBER";
        token_names[TokenType::AND] = "AND";
        token_names[TokenType::OR] = "OR";
        token_names[TokenType::DESC] = "DESC";
        token_names[TokenType::COUNT] = "COUNT";
        token_names[TokenType::MIN] = "MIN";
        token_names[TokenType::MAX] = "MAX";
        token_names[TokenType::AVERAGE] = "AVERAGE";
        token_names[TokenType::SUM] = "SUM";
        token_names[TokenType::DELETE] = "DELETE";
        token_names[TokenType::TABLE] = "TABLE";
        token_names[TokenType::ROW] = "ROW";
        token_names[TokenType::COLUMN] = "COLUMN";
        token_names[TokenType::INSERT] = "INSERT";
        token_names[TokenType::TABLE_TO_INSERT_INTO] = "TABLE_TO_INSERT_INTO";
        token_names[TokenType::VALUES_TO_INSERT] = "VALUES_TO_INSERT";
        token_names[TokenType::COLUMNS_TO_INSERT] = "COLUMNS_TO_INSERT";
        token_names[TokenType::COLUMN_TO_INSERT] = "COLUMN_TO_INSERT";
        token_names[TokenType::_EOF] = "EOF";
        token_names[TokenType::QUESTION] = "QUESTION";
        token_names[TokenType::DISTINCT] = "DISTINCT";
        token_names[TokenType::COLUMN_GROUPING] = "COLUMN_GROUPING";
        token_names[TokenType::GROUPING] = "GROUPING";
        token_names[TokenType::isLetters] = "isLetters";
        token_names[TokenType::isNumber] = "isNumber";
        token_names[TokenType::TABLE_NAME] = "TABLE_NAME";
        token_names[TokenType::_NULL] = "NULL";


        return token_names[type] + " " + lexeme;
    }
} Token;

#endif //C_LOX_TOKENTYPE_H
