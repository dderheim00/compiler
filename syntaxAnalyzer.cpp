#include <iostream>
#include <string>
#include <vector> //for reading in files
#include <fstream>
#include <sstream>
#include <stack>
#include <iomanip> //for table formatting

using namespace std;


vector<vector<string> > csvData;
vector<string> csvRW;
vector<vector<string> > csvPrec;

void readCSV(string csvFile, string kind){
    ifstream csvInputFile(csvFile);
    if(csvInputFile.is_open()) {
        string temp;
        while(getline(csvInputFile, temp)) {
            vector<string> row;
            stringstream strsrm(temp);
            string cell;
            while(getline(strsrm, cell, ',')){
                row.push_back(cell);
            }
            if(kind == "Data"){
                csvData.push_back(row);
            }
            else if(kind == "Reserved"){
                for (int i = 0; i < row.size(); i++) {
                    string j = row[i];
                    csvRW.push_back(j);
                }
            }
            else if(kind == "Precedence"){
                csvPrec.push_back(row);
            }
            else{
                exit(1);
            }
        }
    }
    csvInputFile.close();
}

struct Symbol {
    string token;
    string type;
    string address;
    string segment;
    string value;
};

vector<Symbol> symbolTable; //define symbolTable as a vector of Symbol type

void readTxt(string txtFile, string kind){
    ifstream txtInputFile(txtFile);
    if(txtInputFile.is_open()){
        string line;
        while(getline(txtInputFile, line)){ // Read each line of the input file
            Symbol symbol;
            istringstream ss(line);
            getline(ss, symbol.token, '\t'); // Read each column of the line and assign to corresponding symbol fields
            getline(ss, symbol.type, '\t');
            getline(ss, symbol.address, '\t');
            getline(ss, symbol.segment, '\t');
            getline(ss, symbol.value, '\t');
            if(kind == "tokens"){
                // Add the symbol to the txtTokensList vector
                //txtTokensList.push_back(symbol);
            }
            else if(kind == "symbol"){
                // Add the symbol to the symbolTable vector
                symbolTable.push_back(symbol);
            }
            else{
                // Handle invalid kind input
                cout << "Invalid input kind" << endl;
                break;
            }
        }
        txtInputFile.close();
    }
}



string quadGen(vector<Symbol> symbolTable) {
    stack<string> opStack;
    stack<string> valStack;
    int tableLocOfLastOp = 1;
    int tableLocOfCurrentOp = 1;
    string quad = "";
    string token;
    int tempVarCount = 0;
    string lastOp = "⊥";
    string currentOp = "⊥";
    bool invalid;

    for (int i = 0; i < symbolTable.size(); i++) {
        token = symbolTable[i].token;

        if (token == "=" || token == "+" || token == "-" || token == "(" || token == ")" || token == "*" || token == "/" || token == "IF" || token == "THEN" || token == "==" || token == "!=" || token == ">" || token == "<" || token == ">=" || token == "<=" || token == "{" || token == "}" || token == ";") {
            lastOp = currentOp;
            currentOp = token;

            if(currentOp == "⊥"){
                tableLocOfCurrentOp = 1;
            }
            else if(currentOp == "="){
                tableLocOfCurrentOp = 2;
            }
            else if(currentOp == "+"){
                tableLocOfCurrentOp = 3;
            }
            else if(currentOp == "-"){
                tableLocOfCurrentOp = 4;
            }
            else if(currentOp == "("){
                tableLocOfCurrentOp = 5;
            }
            else if(currentOp == ")"){
                tableLocOfCurrentOp = 6;
            }
            else if(currentOp == "*"){
                tableLocOfCurrentOp = 7;
            }
            else if(currentOp == "/"){
                tableLocOfCurrentOp = 8;
            }
            else if(currentOp == ";"){
                tableLocOfCurrentOp = 19;
            }
            else{
                cout << "Invalid operator" << endl;
            }
            //do for the rest of the operators when the time comes


            string precedence = string(1, csvPrec[tableLocOfLastOp][tableLocOfCurrentOp][0]);
            
            //PROBLEM LIES IN THIS 'IF'
            if (precedence[0] == 60 || precedence[0] == 61) { //if precedence == "<" or "="
                opStack.push(token);
                tableLocOfLastOp = tableLocOfCurrentOp;
            }
            else if(precedence[0] == 62) { //if precedence == ">"
                bool tokenPushed = false;
                while(!opStack.empty() && (csvPrec[tableLocOfLastOp][tableLocOfCurrentOp][0] == '>' || csvPrec[tableLocOfLastOp][tableLocOfCurrentOp][0] == '=')){
                    if(opStack.top() == "=" && currentOp == ";"){ //maybe add && opStack.size() == 2
                        string rightOperand = valStack.top();
                        valStack.pop();
                        string leftOperand = valStack.top();
                        valStack.pop();
                        quad += opStack.top() + ", " + leftOperand + ", " + rightOperand + "\n";
                        opStack.pop();
                    }
                    else{
                        string tempVar = "T" + to_string(++tempVarCount);
                        string rightOperand = valStack.top();
                        valStack.pop();
                        string leftOperand = valStack.top();
                        valStack.pop();
                        quad += opStack.top() + ", " + leftOperand + ", " + rightOperand + ", " + tempVar + "\n";
                        valStack.push(tempVar);
                        opStack.pop();
                        int tableLocOfStackTop = 1; //initialize to default value
                        if(!opStack.empty()){
                            string topOp = opStack.top();
                            if (topOp == "⊥") {
                                tableLocOfStackTop = 1;
                            } else if (topOp == "=") {
                                tableLocOfStackTop = 2;
                            } else if (topOp == "+") {
                                tableLocOfStackTop = 3;
                            } else if (topOp == "-") {
                                tableLocOfStackTop = 4;
                            } else if (topOp == "(") {
                                tableLocOfStackTop = 5;
                            } else if (topOp == ")") {
                                tableLocOfStackTop = 6;
                            } else if (topOp == "*") {
                                tableLocOfStackTop = 7;
                            } else if (topOp == "/") {
                                tableLocOfStackTop = 8;
                            } else if (topOp == ";") {
                                tableLocOfStackTop = 19;
                            } else {
                                cout << "Invalid operator" << endl;
                            }
                        }
                        tableLocOfLastOp = tableLocOfStackTop;
                        precedence = string(1, csvPrec[tableLocOfLastOp][tableLocOfCurrentOp][0]);
                        if(tokenPushed == false && precedence[0] != 62){
                            if(!opStack.empty() && currentOp != ";"){
                                opStack.push(token); //same token gets pushed multiple times deswegen the while loop?
                                tokenPushed = true;
                            }
                        }
                    } //end else

                } //end while

                tableLocOfLastOp = tableLocOfCurrentOp;
            } //end else if(precedence ">")

            else {
                cout << "Invalid operator precedence" << endl;
                invalid = true;
            }

        }
        else{
            valStack.push(token);
        }
    }

    return quad;
}




int main() {

    readCSV("CompilerTable.csv", "Data");
    readCSV("ReservedWords.csv", "Reserved");
    readCSV("PrecedenceTable.csv", "Precedence");
    readTxt("tokensList.txt", "tokens");
    readTxt("symbolTable.txt", "symbol");

    string outputQuad = quadGen(symbolTable);
    cout << outputQuad;

    return 0;
}