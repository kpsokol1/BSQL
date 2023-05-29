#pragma once
#ifndef C_LOX_LOX_H
#define C_LOX_LOX_H
#include <fstream>
#include <sstream>
//#include "Interpreter.h"

using namespace std;

class Scanner;
class BSQL{
public:
    static int main(int argc, char *argv[]);

private:
    static void runFile(string path);

    static void runPrompt();

    static void run(string source);
};
#endif //C_LOX_LOX_H
