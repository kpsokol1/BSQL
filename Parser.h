#ifndef BSQL_PARSER_H
#define BSQL_PARSER_H


#include "TokenType.h"
#include "ParseError.h"
#include <vector>
using namespace std;

class Parser {
private:
    const vector<Token> tokens;
    int current = 0; //points to the next token waiting to be parsed
    string havingStatement = "";

    bool match(int num_args, ...);

    Token consume(TokenType type, string message);

    bool check(TokenType type);

    Token advance();

    bool isAtEnd();

    Token peek();

    Token previous();

    void error(Token token, string message);
    ParseError parse_error(Token token, string message);

    void synchronize();
    void report(int line, string where, string message);

    string buildSelect();
    string buildDelete();
    string buildInsert();

    string buildJoinStatement();
    string buildWhereStatement();
    string buildOrderByStatement();
    string buildSelectStatement();
    string buildGroupByStatement();

    string getColumn();
    string getCondition();

    string getTableName(string column);

public:
    Parser(const vector<Token> tokens);
    vector<string> table_names;

    void parse();
};


#endif //BSQL_PARSER_H
