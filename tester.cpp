#include <iostream>
#include <fstream>
#include <string>

using namespace std;


void Test1();
void Test2();
void Test3();
void Test4();
void Test5();
void Test6();
void Test7();
void Test8();
void Test9();
void Test10();
void Test11();
void Test12();
void Test13();
void Test14();
void Test15();
void Test16();
void Test17();
void Test18();
void compareFiles(string resultsFile, string correctFile, int testNumber);

int main(){
    cout << "Begin Testing: "<< endl;
    system("g++ BSQL.cpp global.cpp Scanner.cpp Parser.cpp  -std=c++17");
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    Test6();
    Test7();
    Test8();
    Test9();
    Test10();
    Test11();
    Test12();
    Test13();
    Test14();
    Test15();
    Test16();
    Test17();
    Test18();
    cout << "Testing Finished" << endl;
    return 0;
}
void compareFiles(string resultsFile, string correctFile, string testname){
    ifstream results(resultsFile);
    ifstream correct(correctFile);

    string resultLine;
    string correctLine;
    int lineNumber = 1;
    while(!results.eof() || !correct.eof()) {
        getline(results, resultLine);
        getline(correct, correctFile);
        if (resultLine != correctFile) {
            cout << "Test: (" << testname << ") failed at line: " << to_string(lineNumber) << endl;
            return;
        }
        lineNumber++;
    }
    cout << "Test: (" << testname << ") : pass" << endl;
}

void Test1(){
    system("./a.out test_inputs/aggregates.bsql > test_results/aggregates_results.txt 2>&1");
    compareFiles("test_results/aggregates_results.txt", "test_expected_output/aggregates_correct.txt","aggregates");
}

void Test2(){
    system("./a.out test_inputs/basic.bsql > test_results/basic_results.txt 2>&1");
    compareFiles("test_results/basic_results.txt", "test_expected_output/basic_correct.txt","basic");
}

void Test3(){
    system("./a.out test_inputs/columns_displayed.bsql > test_results/columns_displayed_results.txt 2>&1");
    compareFiles("test_results/columns_displayed_results.txt", "test_expected_output/columns_displayed_correct.txt","columns_displayed");
}

void Test4(){
    system("./a.out test_inputs/combination.bsql > test_results/combination_results.txt 2>&1");
    compareFiles("test_results/combination_results.txt", "test_expected_output/combination_correct.txt","combination");
}

void Test5(){
    system("./a.out test_inputs/delete.bsql > test_results/delete_results.txt 2>&1");
    compareFiles("test_results/delete_results.txt", "test_expected_output/delete_correct.txt","delete");
}

void Test6(){
    system("./a.out test_inputs/filters.bsql > test_results/filters_results.txt 2>&1");
    compareFiles("test_results/filters_results.txt", "test_expected_output/filters_correct.txt","filters");
}

void Test7(){
    system("./a.out test_inputs/insert.bsql > test_results/insert_results.txt 2>&1");
    compareFiles("test_results/insert_results.txt", "test_expected_output/insert_correct.txt","insert");
}

void Test8(){
    system("./a.out test_inputs/multiple_tables.bsql > test_results/multiple_tables_results.txt 2>&1");
    compareFiles("test_results/multiple_tables_results.txt", "test_expected_output/multiple_tables_correct.txt","multiple_tables");
}

void Test9(){
    system("./a.out test_inputs/results_order.bsql > test_results/results_order_results.txt 2>&1");
    compareFiles("test_results/results_order_results.txt", "test_expected_output/results_order_correct.txt","results_order");
}

void Test10(){
    system("./a.out test_inputs/semantic_error_1.bsql > test_results/semantic_error_1_results.txt 2>&1");
    compareFiles("test_results/semantic_error_1_results.txt", "test_expected_output/semantic_error_1_correct.txt","semantic_error_1");
}

void Test11(){
    system("./a.out test_inputs/semantic_error_2.bsql > test_results/semantic_error_2_results.txt 2>&1");
    compareFiles("test_results/semantic_error_2_results.txt", "test_expected_output/semantic_error_2_correct.txt","semantic_error_2");
}

void Test12(){
    system("./a.out test_inputs/semantic_error_3.bsql > test_results/semantic_error_3_results.txt 2>&1");
    compareFiles("test_results/semantic_error_3_results.txt", "test_expected_output/semantic_error_3_correct.txt","semantic_error_3");
}

void Test13(){
    system("./a.out test_inputs/semantic_error_4.bsql > test_results/semantic_error_4_results.txt 2>&1");
    compareFiles("test_results/semantic_error_4_results.txt", "test_expected_output/semantic_error_4_correct.txt","semantic_error_4");
}

void Test14(){
    system("./a.out test_inputs/semantic_error_5.bsql > test_results/semantic_error_5_results.txt 2>&1");
    compareFiles("test_results/semantic_error_5_results.txt", "test_expected_output/semantic_error_5_correct.txt","semantic_error_5");
}

void Test15(){
    system("./a.out test_inputs/syntax_error_1.bsql > test_results/syntax_error_1_results.txt 2>&1");
    compareFiles("test_results/syntax_error_1_results.txt", "test_expected_output/syntax_error_1_correct.txt","syntax_error_1");
}

void Test16(){
    system("./a.out test_inputs/syntax_error_2.bsql > test_results/syntax_error_2_results.txt 2>&1");
    compareFiles("test_results/syntax_error_2_results.txt", "test_expected_output/syntax_error_2_correct.txt","syntax_error_2");
}

void Test17(){
    system("./a.out test_inputs/syntax_error_3.bsql > test_results/syntax_error_3_results.txt 2>&1");
    compareFiles("test_results/semantic_error_3_results.txt", "test_expected_output/semantic_error_3_correct.txt","syntax_error_3");
}

void Test18(){
    system("./a.out test_inputs/syntax_error_4.bsql > test_results/syntax_error_4_results.txt 2>&1");
    compareFiles("test_results/syntax_error_4_results.txt", "test_expected_output/syntax_error_4_correct.txt","syntax_error_4");
}



