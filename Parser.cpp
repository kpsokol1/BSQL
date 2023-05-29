#include "Parser.h"
#include <stdarg.h>
#include <iostream>
#include <string>
#include <algorithm>

Parser::Parser(const vector<Token> tokens): tokens(tokens){}

void Parser::parse(){
    string sqlString = "";
    while(!isAtEnd()){
        try{
            table_names.clear();
            consume(TokenType::QUERY_TYPE, "Expected Token: QUERY_TYPE:");
            if(match(1,TokenType::QUESTION)){
                sqlString = buildSelect();
            }
            else if(match(1,TokenType::DELETE)){
                sqlString = buildDelete();
            }
            else if(match(1, TokenType::INSERT)){
                sqlString = buildInsert();
            }
            else{
                throw parse_error(peek(), "Expected QUESTION, DELETE, or INSERT after QUERY_TYPE:");
            }
            cout << "SQL QUERY: " << sqlString << endl;
        }
        catch(...){
            exit(65);
        }

    }
}
void Parser::error(Token token, string message){
    if (token.type == TokenType::_EOF) {
        report(token.line, " at end", message);
    } else {
        report(token.line, " at '" + token.lexeme + "'", message);
    }
}

void Parser::report(int line, string where, string message){
    cerr << "[line " << to_string(line) <<"] Error" << where << ": " << message << endl;
    hadError = true;
}

ParseError Parser::parse_error(Token token, string message){
    error(token,message);
    return ParseError();
}

bool Parser::match(int num_args, ...) {
    va_list valist;
    va_start(valist,num_args);
    for (int i = 0; i < num_args; i++) {
        if (check(va_arg(valist,TokenType))) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, string message){
    if(check(type)) return advance();
    throw parse_error(peek(),message);
}

bool Parser::check(TokenType type){
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance(){
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd(){
    return peek().type == TokenType::_EOF;
}

Token Parser::peek(){
    return tokens.at(current);
}

Token Parser::previous(){
    return tokens.at(current - 1);
}

string Parser::getColumn(){
    string aggregateFunction = "";
    string column;
    string distinct = "";
    if (match(5, TokenType::COUNT, TokenType::MIN, TokenType::MAX, TokenType::AVERAGE, TokenType::SUM)) {
        aggregateFunction = previous().lexeme;
        consume(TokenType::LEFT_PAREN, "Expected '(' after aggregate");
        if(match(1, TokenType::ALL)) {
            column= "*";
        }
        else{
            column = getColumn();
        }
    }
    if (match(1, TokenType::DISTINCT)) {            //fixme need to check later if distinct is even allowed and/or distinct is not used before ALL/*
        distinct = previous().lexeme + " ";
    }
    if(aggregateFunction != ""){
        consume(TokenType::RIGHT_PAREN, "Expected ')' after column name");
        return aggregateFunction + "(" + distinct + column + ")";
    }

    //base case of just a normal column
    consume(TokenType::IDENTIFIER, "Expected table name identifier");
    string table = previous().lexeme;
    if(find(table_names.begin(), table_names.end(), table) == table_names.end()){
        throw parse_error(previous(), "Cannot operate on table not in Tables list");
    }
    consume(TokenType::DOT, "Expected '.' after table name");
    consume(TokenType::IDENTIFIER, "Expected column name identifier");
    column = previous().lexeme;

    return distinct + table +"."+column;
}

string Parser::getCondition(){
    string whereStatement = "";
    string column1 = "";
    string conditionalOperator = "";
    string literalComparator;
    string column2 = "";

    column1 = getColumn();

    //get conditional operator
    if(match(6,TokenType::LESS, TokenType::LESS_EQUAL,TokenType::GREATER, TokenType::GREATER_EQUAL,TokenType::NOT_EQUAL,TokenType::EQUAL)){
        conditionalOperator = previous().lexeme;
    }

    //get comparator
    if(match(1, TokenType::STRING)){
        literalComparator = "\'" + previous().lexeme + "\'";
    }
    else if(match(2, TokenType::NUMBER, TokenType::_NULL)){
        literalComparator = previous().lexeme;
    }
    else if(check(TokenType::IDENTIFIER)){
        column2 = getColumn();
    }
    else{
        throw parse_error(peek(),"expected table.column, number, or string after: " + conditionalOperator);
    }

    whereStatement += column1 +" "+ conditionalOperator + " ";
    if(column2 != ""){
        whereStatement += column2;
    }
    else{
        whereStatement += literalComparator;     //fixme make sure strings have single quotes around them still
    }
    return whereStatement;
}

string Parser::buildJoinStatement() {
    string joinStatement = "";
    consume(TokenType::MATCHING_COLUMNS, "Expected MATCHING_COLUMNS keyword");

    if(!match(1,TokenType::NONE)) {
        for (int i = 1; i < table_names.size(); i++) {
            joinStatement += " JOIN " + table_names.at(i) + " ON ";
            consume(TokenType::LEFT_PAREN, "Expected '(' before matching columns");
            string column1 = getColumn();

            consume(TokenType::EQUAL, "Expected = after column name");

            string column2 = getColumn();
            consume(TokenType::RIGHT_PAREN, "Expected ')' after matching columns");

            if(getTableName(column1) == getTableName(column2)){
                throw parse_error(previous(),"Can't join on the same table");
            }
            joinStatement += column1 + " = " + column2;
        }
    }
    return joinStatement;
}

string Parser::getTableName(string column) {
    string runningString = "";
    for(int i =0; i < column.size(); i++){
        if(column.at(i) != '.'){
            runningString += column.at(i);
        }
        else{
            return runningString;
        }
    }
    return runningString;
}

string Parser::buildWhereStatement() {
    //get filters and having statements
    string whereStatement = "";
    consume(TokenType::FILTERS, "Expected Filters expression after MATCHING_COLUMNS expression");
    if(!match(1,TokenType::NONE)){
        whereStatement = " WHERE ";
        while(!check(TokenType::RESULTS_ORDER) && !isAtEnd()){
            //get first table/column/group
            if(match(1,TokenType::GROUPING)){
                consume(TokenType::LEFT_PAREN, "Expected '(' after GROUPING statement");
                havingStatement += " HAVING " + getCondition();
                consume(TokenType::RIGHT_PAREN, "Expected ')' after GROUPING statement");
            }
            else{
                whereStatement += getCondition();
            }

            if(match(2,TokenType::AND, TokenType::OR)){
                if(peek().type != TokenType::GROUPING){  //fixme make sure grouping has AND not OR before it
                    whereStatement += " " + previous().lexeme + " ";
                }
            }
            else if(!check(TokenType::RESULTS_ORDER) && !isAtEnd()){
                throw parse_error(peek(), "Expected logical operator before token");
            }
        }
    }
    return whereStatement;
}
string Parser::buildSelect(){
    consume(TokenType::TABLES, "Expected TABLES: after QUESTION:");
    //get table names
    while(!check(TokenType::MATCHING_COLUMNS) && !isAtEnd()){
        if(match(1, TokenType::IDENTIFIER)){
            if(find(table_names.begin(), table_names.end(), previous().lexeme) == table_names.end()){
                table_names.push_back(previous().lexeme);
            }
            else{
                throw parse_error(peek(), "Table names must be unique");
            }

        }
        else{
            throw parse_error(peek(), "Expected table name after TABLES:");
        }
    }

    string join = buildJoinStatement();
    string where = buildWhereStatement();
    string orderBy = buildOrderByStatement();
    string select = buildSelectStatement();
    string groupBy = buildGroupByStatement();

    string query = select + " FROM " + table_names.at(0) + join + where + groupBy + havingStatement + orderBy;
    return query;
}

string Parser::buildSelectStatement() {
    string select = "SELECT ";
    consume(TokenType::COLUMNS_DISPLAYED, "Expected COLUMNS_DISPLAYED, after RESULTS_ORDER:");
    if(match(1,TokenType::ALL)){
        select += "*";
    }
    else {
        while (!check(TokenType::COLUMN_GROUPING) && !isAtEnd()) {
            select += getColumn();
            if (!check(TokenType::COLUMN_GROUPING) && !isAtEnd()) {
                select += ", ";
            }
        }
    }
    return select;
}
string Parser::buildOrderByStatement() {
    consume(TokenType::RESULTS_ORDER, "Expected RESULTS_ORDER: after FILTERS");
    string orderByStatement = "";
    if(!match(1,TokenType::DEFAULT)) {
        orderByStatement  =" ORDER BY ";
        while(!check(TokenType::COLUMNS_DISPLAYED) && !isAtEnd()){
            string orderMethod = "";
            consume(TokenType::LEFT_PAREN,"Expected '(' before column name");
            string column = getColumn();
            if (column.find("COUNT") != string::npos || column.find("MIN") != string::npos || column.find("MAX") != string::npos || column.find("AVG") != string::npos || column.find("SUM") != string::npos){
                throw parse_error(previous(), "Aggregates not allowed in RESULTS_ORDER");
            }
            if(match(2,TokenType::ASC,TokenType::DESC)){
                orderMethod  = previous().lexeme;
            }

            consume(TokenType::RIGHT_PAREN, "Expected ')' after order type");
            orderByStatement += column + " " + orderMethod;

            if(!check(TokenType::COLUMNS_DISPLAYED) && !isAtEnd()){
                orderByStatement += ", ";
            }
        }
    }
    return orderByStatement;
}

string Parser::buildGroupByStatement() {
    string groupBy = "";
    consume(TokenType::COLUMN_GROUPING, "Expected COLUMN_GROUPING, after COLUMNS_DISPLAYED");
    if(!match(1,TokenType::NONE)){
       groupBy = " GROUP BY ";
        while(!isAtEnd()){
            string column = getColumn();
            if (column.find("COUNT") != string::npos || column.find("MIN") != string::npos || column.find("MAX") != string::npos || column.find("AVG") != string::npos || column.find("SUM") != string::npos){
                throw parse_error(previous(), "Aggregates not allowed in COLUMN_GROUPING");
            }
            groupBy += column;

            if(!isAtEnd()){
                groupBy += ", ";    //fixme does this work?
            }
        }
    }
    return groupBy;
}

string Parser::buildDelete(){
    string table = "";
    string column = "";
    string condition = "";
    consume(TokenType::DELETE_OBJECT, "Expected DELETE_OBJECT");
    consume(TokenType::IDENTIFIER, "Expected table name");
    table = previous().lexeme;
    table_names.push_back(table);
    if(match(1, TokenType::DOT)){
        consume(TokenType::IDENTIFIER, "Expected column name");
        column = previous().lexeme;
        return "ALTER TABLE "+ table + " DROP COLUMN " + column;
    }
    if(match(1, TokenType::FILTERS) && !check(TokenType::NONE)){
        return "DELETE FROM " + table + " WHERE " + getCondition();
    }
    consume(TokenType::NONE, "Expected NONE if deleting a table");
    return "DROP " + table;
}

string Parser::buildInsert() {
    string table = "";
    string column = "";
    consume(TokenType::INSERT_OBJECT, "Expected INSERT_OBJECT:");
    if(match(1,TokenType::TABLE)){
        string createStatement = "CREATE TABLE ";
        unordered_map<string,bool> columns; //false is number, true is letter
        consume(TokenType::TABLE_NAME, "Expected TABLE_NAME:");
        consume(TokenType::IDENTIFIER, "Expected Table name");
        table = previous().lexeme;
        createStatement += table + " (";
        consume(TokenType::COLUMNS_TO_INSERT, "Expected COLUMNS:");
        while(!isAtEnd() && !check(TokenType::QUERY_TYPE)){
            consume(TokenType::LEFT_PAREN, "Expected '(' after COLUMNS:");
            consume(TokenType::IDENTIFIER, "Expected column name");
            column = previous().lexeme;
            if(match(1,TokenType::isNumber)){
                columns.insert({column, false});
            }
            else if(match(1,TokenType::isLetters)){
                columns.insert({column, true});
            }
            else{
                throw parse_error(peek(),"Expected NUMBERS or LETTERS column identifier");
            }
            consume(TokenType::RIGHT_PAREN, "Expected ')' after NUMBERS or LETTERS");
        }
        bool isFirst = true;
        for(auto const& [key, val] : columns){
            if(!isFirst){
                createStatement += ", ";
            }
            createStatement += key;
            if(val){
                createStatement += " varchar(255)";
            }
            else{
                createStatement += " int";
            }
            isFirst = false;
        }
        createStatement += ")"; //fixme do we need a ; at the end of a create statement?
        return createStatement;
    }
    else if(match(1,TokenType::ROW)){
        consume(TokenType::TABLE_NAME, "Expected TABLE_NAME:");
        consume(TokenType::IDENTIFIER, "Expected table name identifier");
        string table = previous().lexeme;
        consume(TokenType::VALUES_TO_INSERT, "Expected VALUES:");
        unordered_map<string,string> columnStringMap;
        unordered_map<string,int> columnNumberMap;
        vector<string> columnNames;
        while(!isAtEnd()){
            consume(TokenType::LEFT_PAREN, "Expected '('");
            consume(TokenType::IDENTIFIER, "Expected column name identifier");
            column = previous().lexeme;
            if(match(1,TokenType::NUMBER)){
                columnNumberMap.insert({column, stoi(previous().lexeme)});
                columnNames.push_back(column);
            }
            else if(match(1, TokenType::STRING)){
                columnStringMap.insert({column, previous().lexeme});
                columnNames.push_back(column);
            }
            else if(match(1, TokenType::_NULL)){
                columnNames.push_back(column);
            }
            else{
                throw parse_error(peek(), "Expected number or string");
            }
            consume(TokenType::RIGHT_PAREN, "Expected ')'");
        }
        string insertIntoStatement = "INSERT INTO " + table + " (";
        bool isFirst = true;
        for(int i = 0; i < columnNames.size(); i++){
            if(!isFirst){
                insertIntoStatement += ",";
            }
            insertIntoStatement += columnNames.at(i);
            isFirst = false;
        }
        insertIntoStatement += ") VALUES (";
        isFirst = true;
        for(int i = 0; i < columnNames.size(); i++){
            if(!isFirst){
                insertIntoStatement += ",";
            }
            if(columnStringMap.find(columnNames.at(i)) != columnStringMap.end()){
                insertIntoStatement += "\'" + columnStringMap[columnNames.at(i)] + "\'";
            }
            else if(columnNumberMap.find(columnNames.at(i)) != columnNumberMap.end()){
                insertIntoStatement += to_string(columnNumberMap[columnNames.at(i)]);
            }
            else{
                insertIntoStatement += "NULL";
            }
            isFirst = false;
        }
        insertIntoStatement += ")"; //fixme do we need a semi colon at the end?

        return insertIntoStatement;
    }
    else if(match(1,TokenType::COLUMN)){
        consume(TokenType::TABLE_NAME, "Expected TABLE_NAME:");
        consume(TokenType::IDENTIFIER, "Expected table name identifier");
        string table = previous().lexeme;
        consume(TokenType::COLUMN_TO_INSERT, "Expected COLUMN_TO_INSERT");
        consume(TokenType::LEFT_PAREN, "Expected '(' after COLUMN:");
        consume(TokenType::IDENTIFIER, "Expected column name");
        column = previous().lexeme;
        if(match(1,TokenType::isNumber)){
            consume(TokenType::RIGHT_PAREN, "Expected ')'");
            return "ALTER TABLE " + table + " ADD " + column + " int";
        }
        else if(match(1,TokenType::isLetters)){
            consume(TokenType::RIGHT_PAREN, "Expected ')'");
            return "ALTER TABLE " + table + " ADD " + column + " varchar(255)";
        }
        else{
            throw parse_error(peek(), "Expected number or string");
        }
    }
    else{
        throw parse_error(peek(), "Expected TABLE, COLUMN, or ROW");
    }
}

