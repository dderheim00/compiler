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

struct TokensList {
    string token;
    string type;
};

vector<TokensList> tokensList; //defines tokensList as a vector of TokensList type

void readTxt(string txtFile, string kind){
    ifstream txtInputFile(txtFile);
    if(txtInputFile.is_open()){
        string line;
        while(getline(txtInputFile, line)){ // Read each line of the input file
            TokensList tokenEntry;
            istringstream ss(line);
            getline(ss, tokenEntry.token, '\t'); // Read each column of the line and assign to corresponding symbol fields
            getline(ss, tokenEntry.type, '\t');
            if(kind == "tokens"){
                // Add the symbol to the tokensList vector
                tokensList.push_back(tokenEntry);
            }
            else if(kind == "symbol"){
                // Add the symbol to the tokensList vector
                
            }
            else{
                cout << "Invalid input kind" << endl;
                break;
            }
        }
        txtInputFile.close();
    }
}



string quadGen(vector<TokensList> symbolTable) {
    stack<string> myStack;
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
            if(currentOp == ")"){
                currentOp = token;
            }
            else{
                lastOp = currentOp;
                currentOp = token;
            }
            
            if(currentOp == "⊥"){
                tableLocOfCurrentOp = 1;
            } else if(currentOp == "="){
                tableLocOfCurrentOp = 2;
            } else if(currentOp == "+"){
                tableLocOfCurrentOp = 3;
            } else if(currentOp == "-"){
                tableLocOfCurrentOp = 4;
            } else if(currentOp == "("){
                tableLocOfCurrentOp = 5;
            } else if(currentOp == ")"){ //this block never gets reached
                tableLocOfCurrentOp = 6;
            } else if(currentOp == "*"){
                tableLocOfCurrentOp = 7;
            } else if(currentOp == "/"){
                tableLocOfCurrentOp = 8;
            } else if(currentOp == "IF"){
                tableLocOfCurrentOp = 9;
            } else if(currentOp == "THEN"){
                tableLocOfCurrentOp = 10;
            } else if(currentOp == "=="){
                tableLocOfCurrentOp = 11;
            } else if(currentOp == "!="){
                tableLocOfCurrentOp = 12;
            } else if(currentOp == ">"){
                tableLocOfCurrentOp = 13;
            } else if(currentOp == "<"){
                tableLocOfCurrentOp = 14;
            } else if(currentOp == ">="){
                tableLocOfCurrentOp = 15;
            } else if(currentOp == "<="){
                tableLocOfCurrentOp = 16;
            } else if(currentOp == "{"){
                tableLocOfCurrentOp = 17;
            } else if(currentOp == "}"){
                tableLocOfCurrentOp = 18;
            } else if(currentOp == ";"){
                tableLocOfCurrentOp = 19;
            } else{
                cout << "Invalid operator" << endl;
            }
            //do for the rest of the operators when the time comes

            string precedence = string(1, csvPrec[tableLocOfLastOp][tableLocOfCurrentOp][0]);
            if(precedence[0] == 60 || precedence[0] == 61){ //if precedence == "<" or "="
                myStack.push(token);
                tableLocOfLastOp = tableLocOfCurrentOp;
            }
            else if(precedence[0] == 62) { //if precedence == ">"
                bool tokenPushed = false;
                bool parenthesesHit = false;

                while(!myStack.empty() && (csvPrec[tableLocOfLastOp][tableLocOfCurrentOp][0] == '>')){
                    if(myStack.size() == 3 && currentOp == ";"){ //maybe add && myStack.size() == 2
                        string rightOperand = myStack.top();
                        myStack.pop();
                        string op = myStack.top();
                        myStack.pop();
                        string leftOperand = myStack.top();
                        myStack.pop();
                        quad += op + ", " + leftOperand + ", " + rightOperand + "\n";
                    }
                    else{
                        string tempVar = "T" + to_string(++tempVarCount);
                        string rightOperand = myStack.top();
                        myStack.pop();
                        string op = myStack.top(); //operator goes here instead
                        myStack.pop();
                        string leftOperand = myStack.top();
                        myStack.pop();
                        myStack.push(tempVar);
                        quad += op + ", " + leftOperand + ", " + rightOperand + ", " + tempVar + "\n";

                        stack<string> tempStack; // Create a temporary stack to find the operator
                        tempStack = myStack;
                        while (!tempStack.empty()) {
                            string top = tempStack.top();
                            tempStack.pop();
                            if(top == "="){
                                tableLocOfLastOp = 2;
                                break;
                            } else if(top == "+"){
                                tableLocOfLastOp = 3;
                                break;
                            } else if(top == "-"){
                                tableLocOfLastOp = 4;
                                break;
                            } else if(top == "("){
                                tableLocOfLastOp = 5;
                                break;
                            } else if(top == ")"){
                                tableLocOfLastOp = 6;
                                break;
                            } else if(top == "*"){
                                tableLocOfLastOp = 7;
                                break;
                            } else if(top == "/"){
                                tableLocOfLastOp = 8;
                                break;
                            } else if(top == "IF"){
                                tableLocOfLastOp = 9;
                                break;
                            } else if(top == "THEN"){
                                tableLocOfLastOp = 10;
                                break;
                            } else if(top == "=="){
                                tableLocOfLastOp = 11;
                                break;
                            } else if(top == "!="){
                                tableLocOfLastOp = 12;
                                break;
                            } else if(top == ">"){
                                tableLocOfLastOp = 13;
                                break;
                            } else if(top == "<"){
                                tableLocOfLastOp = 14;
                                break;
                            } else if(top == ">="){
                                tableLocOfLastOp = 15;
                                break;
                            } else if(top == "<="){
                                tableLocOfLastOp = 16;
                                break;
                            } else if(top == "{"){
                                tableLocOfLastOp = 17;
                                break;
                            } else if(top == "}"){
                                tableLocOfLastOp = 18;
                                break;
                            } else if(top == ";"){
                                tableLocOfLastOp = 19;
                                break;
                            } else{

                            }
                        }
                        while(!tempStack.empty()){
                            tempStack.pop();
                        }

                        precedence = string(1, csvPrec[tableLocOfLastOp][tableLocOfCurrentOp][0]);
                        if(tokenPushed == false && precedence[0] != 62 && !myStack.empty() && currentOp != ";"){                         
                            myStack.push(token);
                            tokenPushed = true;
                            if(myStack.top() == ")" && !myStack.empty()){
                                myStack.pop();
                                string varInsideParentheses;
                                varInsideParentheses = myStack.top();
                                myStack.pop();
                                myStack.pop();
                                lastOp = myStack.top();
                                myStack.push(varInsideParentheses);
                                if(lastOp == "="){
                                    tableLocOfLastOp = 2;
                                    tableLocOfCurrentOp = 2;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == "+"){
                                    tableLocOfLastOp = 3;
                                    tableLocOfCurrentOp = 3;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == "-"){
                                    tableLocOfLastOp = 4;
                                    tableLocOfCurrentOp = 4;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == "("){
                                    tableLocOfLastOp = 5;
                                    tableLocOfCurrentOp = 5;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == ")"){
                                    tableLocOfLastOp = 6;
                                    tableLocOfCurrentOp = 6;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == "*"){
                                    tableLocOfLastOp = 7;
                                    tableLocOfCurrentOp = 7;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == "/"){
                                    tableLocOfLastOp = 8;
                                    tableLocOfCurrentOp = 8;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == "IF"){
                                    tableLocOfLastOp = 9;
                                    tableLocOfCurrentOp = 9;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == "THEN"){
                                    tableLocOfLastOp = 10;
                                    tableLocOfCurrentOp = 10;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == "=="){
                                    tableLocOfLastOp = 11;
                                    tableLocOfCurrentOp = 11;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == "!="){
                                    tableLocOfLastOp = 12;
                                    tableLocOfCurrentOp = 12;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == ">"){
                                    tableLocOfLastOp = 13;
                                    tableLocOfCurrentOp = 13;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == "<"){
                                    tableLocOfLastOp = 14;
                                    tableLocOfCurrentOp = 14;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == ">="){
                                    tableLocOfLastOp = 15;
                                    tableLocOfCurrentOp = 15;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == "<="){
                                    tableLocOfLastOp = 16;
                                    tableLocOfCurrentOp = 16;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == "{"){
                                    tableLocOfLastOp = 17;
                                    tableLocOfCurrentOp = 17;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == "}"){
                                    tableLocOfLastOp = 18;
                                    tableLocOfCurrentOp = 18;
                                    parenthesesHit = true;
                                    break;
                                } else if(lastOp == ";"){
                                    tableLocOfLastOp = 19;
                                    tableLocOfCurrentOp = 19;
                                    parenthesesHit = true;
                                    break;
                                } else{
                                }
                            }
                        }
                    } //end else
                } //end while
                if(parenthesesHit == false){
                    tableLocOfLastOp = tableLocOfCurrentOp;
                }
            } //end else if(precedence ">")
            else {
                cout << "Invalid operator precedence" << endl;
                invalid = true; //variable for debugging. does not serve a logical function
            }
        } //end if operator check
        else{ //is not an operator
            myStack.push(token);
        }
    }

    return quad;
}



void assemblyConvert(string outputQuad) {
    istringstream ss(outputQuad);
    string line;

    while (getline(ss, line)) {
        string op, leftOperand, rightOperand, result;
        istringstream ss_line(line);

        getline(ss_line, op, ',');
        getline(ss_line >> std::ws, leftOperand, ',');
        getline(ss_line >> std::ws, rightOperand, ',');
        getline(ss_line >> std::ws, result);

        if (op == "+") {
            cout << "ADD " << leftOperand << ", " << rightOperand << ", " << result << endl;
        }
        else if (op == "-") {
            cout << "SUB " << leftOperand << ", " << rightOperand << ", " << result << endl;
        }
        else if (op == "*") {
            cout << "MUL " << leftOperand << ", " << rightOperand << ", " << result << endl;
        }
        else if (op == "/") {
            cout << "DIV " << leftOperand << ", " << rightOperand << ", " << result << endl;
        }
        else if (op == "=") {
            cout << "MOV " << leftOperand << ", " << rightOperand << endl;
        }
    }
}



int main() {

    readCSV("CompilerTable.csv", "Data");
    readCSV("ReservedWords.csv", "Reserved");
    readCSV("PrecedenceTable.csv", "Precedence");
    readTxt("tokensList.txt", "tokens");
    readTxt("symbolTable.txt", "symbol");

    string outputQuad = quadGen(tokensList);
    cout << outputQuad << endl;

    assemblyConvert(outputQuad);

    return 0;
}
