#include "BSQL.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Scanner.h"
#include "Parser.h"
#include <regex>

using namespace std;

int BSQL::main(int argc, char *argv[]) {
    hadError = false;
    if(argc > 2){
        cout << "Usage: jlox [script]" << endl;
        return 64;
    }
    else if(argc == 2){
        runFile(argv[1]);        //remember argv[0] is the name of the file
    }
    else{
        runPrompt();
    }
    return 0;
}

void BSQL::runFile(string path){
    ifstream inputFile(path);
    stringstream buffer;
    buffer << inputFile.rdbuf();
    string fileContents = buffer.str();
    run(fileContents);
    if(hadError) exit(65);
    if(hadRuntimeError) exit(70);           //don't care about runtime errors in REPL just continue on
}


void BSQL::runPrompt(){
    while (true){
        string line;
        cout << "> QUERY_TYPE: ";
        cout.flush();
        stringstream contents;
        contents << "QUERY_TYPE: ";
        std::regex r("\\s+");
        getline(cin, line);
        line = std::regex_replace(line, r, "");
        transform(line.begin(), line.end(), line.begin(), ::toupper);
        if(line == "QUESTION"){
            cout << "> TABLES: ";
            contents << "QUESTION\nTABLES: ";
            getline(cin, line);
            contents << line;

            cout << "> MATCHING_COLUMNS: ";
            contents << "\nMATCHING_COLUMNS: ";
            getline(cin, line);
            contents << line;

            cout << "> FILTERS: ";
            contents << "\nFILTERS: ";
            getline(cin, line);
            contents << line;

            cout << "> RESULTS_ORDER: ";
            contents << "\nRESULTS_ORDER: ";
            getline(cin, line);
            contents << line;


            cout << "> COLUMNS_DISPLAYED: ";
            contents << "\nCOLUMNS_DISPLAYED: ";
            getline(cin, line);
            contents << line;

            cout << "> COLUMN_GROUPING: ";
            contents << "\nCOLUMN_GROUPING: ";
            getline(cin, line);
            contents << line;
        }
        else if(line == "DELETE"){
            cout << "> DELETE_OBJECT: ";
            contents << "DELETE\nDELETE_OBJECT: ";
            getline(cin, line);
            contents << line;

            if (line.find('.') == string::npos){
                cout << "> FILTERS: ";
                contents << "\nFILTERS: ";
                getline(cin, line);
                contents << line;
            }
        }
        else if(line == "INSERT"){
            cout << "> INSERT_OBJECT: ";
            contents << "INSERT\nINSERT_OBJECT: ";
            getline(cin, line);
            contents << line;
            string insertObject = std::regex_replace(line, r, "");
            cout << "> TABLE_NAME: ";
            contents << "\nTABLE_NAME: ";
            getline(cin, line);
            contents << line;

            if(insertObject == "COLUMN"){
                cout << "> COLUMN: ";
                contents << "\nCOLUMN: ";
                getline(cin, line);
                contents << line;
            }
            else if(insertObject == "TABLE"){
                cout << "> COLUMNS: ";
                contents << "\nCOLUMNS: ";
                getline(cin, line);
                contents << line;
            }
            else if(insertObject == "ROW"){
                cout << "> VALUES: ";
                contents << "\nVALUES: ";
                getline(cin, line);
                contents << line;
            }
        }
        run(contents.str());
        cout << endl;
    }

}

void BSQL::run(string source){
    Scanner *scanner = new Scanner(source);
    vector<Token> tokens = scanner->scanTokens();

    /*for(int i = 0; i < tokens.size(); i++){
         cout << tokens.at(i).to_string() << endl;
    }*/
    //stop if there was a syntax error
    if(hadError) return;

    Parser parser(tokens);
    parser.parse();

    //stop if there was a syntax error
    if(hadError) return;
}

int main(int argc, char *argv[]) {
    return BSQL::main(argc, argv);
}