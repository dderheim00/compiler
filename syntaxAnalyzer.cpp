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
    if(csvInputFile.is_open()){
        string temp;
        while(getline(csvInputFile, temp)){
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
                for(int i = 0; i < row.size(); i++){
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

struct TokensList{
    string token;
    string type;
};
vector<TokensList> tokensList; //defines tokensList as a vector of TokensList type

struct SymbolTable {
    string token;
    string type;
    string address;
    string segment;
    string value;
};
vector<SymbolTable> symbolTable;

void readTxt(string txtFile, string kind){
    ifstream txtInputFile(txtFile);
    if(txtInputFile.is_open()){
        string line;
        while(getline(txtInputFile, line)){
            if(kind == "tokens"){
                TokensList tokenEntry;
                istringstream ss(line);
                getline(ss, tokenEntry.token, '\t');
                getline(ss, tokenEntry.type, '\t');
                tokensList.push_back(tokenEntry);
            }
            else if(kind == "symbol"){
                SymbolTable symbolEntry;
                istringstream ss(line);
                getline(ss, symbolEntry.token, '\t');
                getline(ss, symbolEntry.type, '\t');
                getline(ss, symbolEntry.address, '\t');
                getline(ss, symbolEntry.segment, '\t');
                getline(ss, symbolEntry.value, '\t');
                symbolTable.push_back(symbolEntry);
            }
            else{
                cout << "Invalid input kind" << endl;
                break;
            }
        }
        txtInputFile.close();
    }
}



int lCount = 0;
void setlCount(int value) {
    lCount = value;
}
string getlCount() {
    return "L" + to_string(lCount);
}



string quadGen(vector<TokensList> tokensList){
    stack<string> myStack;
    int tableLocOfLastOp = 1;
    int tableLocOfCurrentOp = 1;
    string quad = "";
    string token;
    int tempVarCount = 0;
    string lastOp = "⊥";
    string currentOp = "⊥";
    bool invalid; //for testing
    bool conditionalStatement = false;
    bool isConstOrVar = false;
    bool className = false;
    bool CIN_COUT = false;
    int lCount = 0;

    for(int i = 0; i < tokensList.size(); i++){
        token = tokensList[i].token;

        if(token == "CONST" || token == "VAR"){
            isConstOrVar = true;
        }

        if(className == true){
            className = false;
        }
        else if(token == "CLASS"){
            className = true;
        }
        else if(token == "CIN"){
            CIN_COUT = true;
            quad += "CIN, " + tokensList[i+1].token + "\n";
        }
        else if(token == "COUT"){
            CIN_COUT = true;
            quad += "COUT, " + tokensList[i+1].token + "\n";
        }
        else if(CIN_COUT == true){
            CIN_COUT = false;
            i += 1;
        }
        else if(token == "{" || token == "}"){
        }
        else if(token == "=" || token == "+" || token == "-" || token == "(" || token == ")" || token == "*" || token == "/" || token == "IF" || token == "THEN" || token == "==" || token == "!=" || token == ">" || token == "<" || token == ">=" || token == "<=" || token == ";"){
            if(currentOp == ")"){
                currentOp = token;
            }
            else if(currentOp == ";" && myStack.size() == 0){
                lastOp = "⊥";
                tableLocOfLastOp = 1;
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
                conditionalStatement = true;
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

            string precedence = string(1, csvPrec[tableLocOfLastOp][tableLocOfCurrentOp][0]);
            if(precedence[0] == 60 || precedence[0] == 61){ //if precedence == "<" or "="
                myStack.push(token);
                tableLocOfLastOp = tableLocOfCurrentOp;
            }
            else if(precedence[0] == 62){ //if precedence == ">"
                bool tokenPushed = false;
                bool parenthesesHit = false;

                while(!myStack.empty() && (csvPrec[tableLocOfLastOp][tableLocOfCurrentOp][0] == '>')){
                    if((myStack.size() == 3 && currentOp == ";") || (myStack.size() == 4 && currentOp == ";" && isConstOrVar == true)){
                        string rightOperand = myStack.top();
                        myStack.pop();
                        string op = myStack.top();
                        myStack.pop();
                        string leftOperand = myStack.top();
                        myStack.pop();
                        quad += op + ", " + leftOperand + ", " + rightOperand + "\n";
                        if(isConstOrVar == true){
                            myStack.pop();
                            isConstOrVar = false;
                        }
                    }
                    else{
                        if(tableLocOfLastOp == 2){
                            string rightOperand = myStack.top();
                            myStack.pop();
                            string op = myStack.top();
                            myStack.pop();
                            string leftOperand = myStack.top();
                            myStack.pop();
                            quad += op + ", " + leftOperand + ", " + rightOperand + "\n";
                        }
                        else if(lastOp != "==" && lastOp != "!=" && lastOp != ">" && lastOp != "<" && lastOp != ">=" && lastOp != "<="){
                            string tempVar = "T" + to_string(++tempVarCount);
                            string rightOperand = myStack.top();
                            myStack.pop();
                            string op = myStack.top();
                            myStack.pop();
                            string leftOperand = myStack.top();
                            myStack.pop();
                            myStack.push(tempVar);
                            quad += op + ", " + leftOperand + ", " + rightOperand + ", " + tempVar + "\n";
                        }
                        else{ //if relop
                            string rightOperand = myStack.top();
                            myStack.pop();
                            string op = myStack.top();
                            myStack.pop();
                            string leftOperand = myStack.top();
                            myStack.pop();
                            quad += op + ", " + leftOperand + ", " + rightOperand + "\n";
                            conditionalStatement = true;
                        }
                        
                        stack<string> tempStack; //temporary stack to find the top operator
                        tempStack = myStack;
                        while(!tempStack.empty()){
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

                        if(myStack.size() == 2 && myStack.top() == "THEN"){
                            myStack.pop();
                            myStack.pop();
                            tableLocOfLastOp = 1;
                            conditionalStatement = false;
                            setlCount(++lCount);
                            quad += getlCount() + "\n";
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
                                if(myStack.size() == 0){
                                    tableLocOfLastOp = 1;
                                    tableLocOfCurrentOp = 1;
                                    parenthesesHit = true;
                                    break;
                                }
                                else if(lastOp == "="){
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
            else{
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



string assemblyConvert(string outputQuad){
    istringstream ss(outputQuad);
    string line;
    string assembly = "";
    int assemblylCount = 0;

    while(getline(ss, line)){
        string op, leftOperand, rightOperand, result;
        istringstream ss_line(line);

        getline(ss_line, op, ',');
        getline(ss_line >> ws, leftOperand, ',');
        getline(ss_line >> ws, rightOperand, ',');
        getline(ss_line >> ws, result);

        if(op == "CIN"){
            if(isdigit(leftOperand.at(0))){
                assembly += "call PrintString\ncall GetAnInteger\nmov ax,[ReadInt]\nmov " + leftOperand + ", ax\n";
            }
            else{
                assembly += "call PrintString\ncall GetAnInteger\nmov ax,[ReadInt]\nmov [" + leftOperand + "], ax\n";
            }
            assembly += "\n";
        }
        else if(op == "COUT"){
            if(isdigit(leftOperand.at(0))){
                assembly += "mov ax," + leftOperand + "\ncall ConvertIntegerToString\nmov eax, 4\nmov ebx, 1\nmov ecx, Result\nmov edx, ResultEnd\nint 80h\n";
            }
            else{
                assembly += "mov ax,[" + leftOperand + "]\ncall ConvertIntegerToString\nmov eax, 4\nmov ebx, 1\nmov ecx, Result\nmov edx, ResultEnd\nint 80h\n";
            }
            assembly += "\n";
        }
        else if(op == "="){
            if((isdigit(rightOperand.at(0))) && (isdigit(leftOperand.at(0)))){
                assembly += "mov ax," + rightOperand + "\nmov " + leftOperand + ",ax\n";
            }
            else if((isdigit(rightOperand.at(0))) && (!isdigit(leftOperand.at(0)))){
                assembly += "mov ax," + rightOperand + "\nmov [" + leftOperand + "],ax\n";
            }
            else if((!isdigit(rightOperand.at(0))) && (isdigit(leftOperand.at(0)))){
                assembly += "mov ax,[" + rightOperand + "]\nmov " + leftOperand + ",ax\n";
            }
            else{
                assembly += "mov ax,[" + rightOperand + "]\nmov [" + leftOperand + "],ax\n";
            }
            assembly += "\n";
        }
        else if(op.substr(0, 1) == "L" && op.length() == 2 && isupper(op[0])) {
            assembly += "L" + string(1, op[1]) + ": nop\n\n";
        }
        else if(op == "+"){
            if((isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\nadd ax," + rightOperand + "\nmov [" + result + "],ax\n";
            }
            else if((isdigit(leftOperand.at(0))) && (!isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\nadd ax,[" + rightOperand + "]\nmov [" + result + "],ax\n";
            }
            else if((!isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax,[" + leftOperand + "]\nadd ax," + rightOperand + "\nmov [" + result + "],ax\n";
            }
            else{
                assembly += "mov ax,[" + leftOperand + "]\nadd ax,[" + rightOperand + "]\nmov [" + result + "],ax\n";
            }
            assembly += "\n";
        }
        else if(op == "-"){
            if((isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\nadd ax," + rightOperand + "\nmov [" + result + "],ax\n";
            }
            else if((isdigit(leftOperand.at(0))) && (!isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\nsub ax,[" + rightOperand + "]\nmov [" + result + "],ax\n";
            }
            else if((!isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax,[" + leftOperand + "]\nsub ax," + rightOperand + "\nmov [" + result + "],ax\n";
            }
            else{
                assembly += "mov ax,[" + leftOperand + "]\nsub ax,[" + rightOperand + "]\nmov [" + result + "],ax\n";
            }
            assembly += "\n";
        }
        else if(op == "*"){
            if((isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\nmov bx," + rightOperand + "\nmul bx\nmov [" + result + "],ax\n";
            }
            else if((isdigit(leftOperand.at(0))) && (!isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\nmov bx,[" + rightOperand + "]\nmul bx\nmov [" + result + "],ax\n";
            }
            else if((!isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax,[" + leftOperand + "]\nmov bx," + rightOperand + "\nmul bx\nmov [" + result + "],ax\n";
            }
            else{
                assembly += "mov ax,[" + leftOperand + "]\nmov bx,[" + rightOperand + "]\nmul bx\nmov [" + result + "],ax\n";
            }
            assembly += "\n";
        }
        else if(op == "/"){
            if((isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov dx,0\nmov ax," + leftOperand + "\nmov bx," + rightOperand + "\ndiv bx\nmov [" + result + "],ax\n";
            }
            else if((isdigit(leftOperand.at(0))) && (!isdigit(rightOperand.at(0)))){
                assembly += "mov dx,0\nmov ax," + leftOperand + "\nmov bx,[" + rightOperand + "]\ndiv bx\nmov [" + result + "],ax\n";
            }
            else if((!isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov dx,0\nmov ax,[" + leftOperand + "]\nmov bx," + rightOperand + "\ndiv bx\nmov [" + result + "],ax\n";
            }
            else{
                assembly += "mov dx,0\nmov ax,[" + leftOperand + "]\nmov bx,[" + rightOperand + "]\ndiv bx\nmov [" + result + "],ax\n";
            }
            assembly += "\n";
        }
        else if(op == "=="){
            if((isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\ncmp ax," + rightOperand + "\nJNE " + to_string(++assemblylCount) + "\n";
            }
            else if((isdigit(leftOperand.at(0))) && (!isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\ncmp ax,[" + rightOperand + "]\nJNE " + to_string(++assemblylCount) + "\n";
            }
            else if((!isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax,[" + leftOperand + "]\ncmp ax," + rightOperand + "\nJNE " + to_string(++assemblylCount) + "\n";
            }
            else{
                assembly += "mov ax,[" + leftOperand + "]\ncmp ax,[" + rightOperand + "]\nJNE " + to_string(++assemblylCount) + "\n";
            }
            assembly += "\n";
        }
        else if(op == "!="){
            if((isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\ncmp ax," + rightOperand + "\nJE " + to_string(++assemblylCount) + "\n";
            }
            else if((isdigit(leftOperand.at(0))) && (!isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\ncmp ax,[" + rightOperand + "]\nJE " + to_string(++assemblylCount) + "\n";
            }
            else if((!isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax,[" + leftOperand + "]\ncmp ax," + rightOperand + "\nJE " + to_string(++assemblylCount) + "\n";
            }
            else{
                assembly += "mov ax,[" + leftOperand + "]\ncmp ax,[" + rightOperand + "]\nJE " + to_string(++assemblylCount) + "\n";
            }
            assembly += "\n";
        }
        else if(op == ">"){
            if((isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\ncmp ax," + rightOperand + "\nJLE " + to_string(++assemblylCount) + "\n";
            }
            else if((isdigit(leftOperand.at(0))) && (!isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\ncmp ax,[" + rightOperand + "]\nJLE " + to_string(++assemblylCount) + "\n";
            }
            else if((!isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax,[" + leftOperand + "]\ncmp ax," + rightOperand + "\nJLE " + to_string(++assemblylCount) + "\n";
            }
            else{
                assembly += "mov ax,[" + leftOperand + "]\ncmp ax,[" + rightOperand + "]\nJLE " + to_string(++assemblylCount) + "\n";
            }
            assembly += "\n";
        }
        else if(op == "<"){
            if((isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\ncmp ax," + rightOperand + "\nJGE " + to_string(++assemblylCount) + "\n";
            }
            else if((isdigit(leftOperand.at(0))) && (!isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\ncmp ax,[" + rightOperand + "]\nJGE " + to_string(++assemblylCount) + "\n";
            }
            else if((!isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax,[" + leftOperand + "]\ncmp ax," + rightOperand + "\nJGE " + to_string(++assemblylCount) + "\n";
            }
            else{
                assembly += "mov ax,[" + leftOperand + "]\ncmp ax,[" + rightOperand + "]\nJGE " + to_string(++assemblylCount) + "\n";
            }
            assembly += "\n";
        }
        else if(op == ">="){
            if((isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\ncmp ax," + rightOperand + "\nJL " + to_string(++assemblylCount) + "\n";
            }
            else if((isdigit(leftOperand.at(0))) && (!isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\ncmp ax,[" + rightOperand + "]\nJL " + to_string(++assemblylCount) + "\n";
            }
            else if((!isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax,[" + leftOperand + "]\ncmp ax," + rightOperand + "\nJL " + to_string(++assemblylCount) + "\n";
            }
            else{
                assembly += "mov ax,[" + leftOperand + "]\ncmp ax,[" + rightOperand + "]\nJL " + to_string(++assemblylCount) + "\n";
            }
            assembly += "\n";
        }
        else if(op == "<="){
            if((isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\ncmp ax," + rightOperand + "\nJG " + to_string(++assemblylCount) + "\n";
            }
            else if((isdigit(leftOperand.at(0))) && (!isdigit(rightOperand.at(0)))){
                assembly += "mov ax," + leftOperand + "\ncmp ax,[" + rightOperand + "]\nJG " + to_string(++assemblylCount) + "\n";
            }
            else if((!isdigit(leftOperand.at(0))) && (isdigit(rightOperand.at(0)))){
                assembly += "mov ax,[" + leftOperand + "]\ncmp ax," + rightOperand + "\nJG " + to_string(++assemblylCount) + "\n";
            }
            else{
                assembly += "mov ax,[" + leftOperand + "]\ncmp ax,[" + rightOperand + "]\nJG " + to_string(++assemblylCount) + "\n";
            }
            assembly += "\n";
        }
    }
    assembly += "int 0x80\nmov ebx,0\nmov eax,1\nint 0x80\n";

    return assembly;
}



string IO_File(string assemblyCode, vector<SymbolTable> symbolTable){
    string dataSection = "";
    for(int i = 0; i < symbolTable.size(); i++){
        if(symbolTable[i].type == "CONST" || symbolTable[i].type == "VAR"){
            dataSection += "\t" + symbolTable[i].token + "\t" + "DW" + "\t" + symbolTable[i].value + "\n";
        }
    }

    string IO_Output = "";
    IO_Output = 
    "sys_exit\tequ\t1\nsys_read\tequ\t3\nsys_write\tequ\t4\nstdin\tequ\t0\nstdout\tequ\t1\nstderr\tequ\t3\n\n"

    //.data section
    "section .data\nuserMsg db 'Enter an input: '\nlenUserMsg equ $-userMsg\ndisplayMsg db 'You entered: '\nlenDisplayMsg equ $-displayMsg\nnewline db 0xA\n"
    "printTempchar db 'Tempchar = : '\nlenprintTempchar equ $-printTempchar\nResult db 'Answer: '\nResultValue db 'aaaaa'\ndb 0xA\nResultEnd equ $-Result\n"
    "num times 6 db 'ABCDEF'\nnumEnd equ $-num\n" + dataSection + "\n"

    "section\t.bss\n\n\tTempChar\tRESW\t1\n\ttestchar\tRESW\t1\n\tReadInt\tRESW\t1\n\ttempint\tRESW\t1\n\tnegflag\tRESW\t1\n"
    "T1\tRESW\t1\nT2\tRESW\t1\nT3\tRESW\t1\nT4\tRESW\t1\nT5\tRESW\t1\nT6\tRESW\t1\nT7\tRESW\t1\nT8\tRESW\t1\nT9\tRESW\t1\nT10\tRESW\t1\nglobal _start\n"

    //.start_ section
    "section .text\n\n_start:\n" + assemblyCode + "\n"

    "fini:\n\tmov eax,sys_exit\n\txor ebx,ebx\n\tint 80h\n\n"
    "PrintString:\n\tpush\tax\n\tpush\tdx\n\tmov eax, 4\n\tmov ebx, 1\n\tmov ecx, userMsg\n\tmov edx, lenUserMsg\n\tint\t80h\n\tpop\tdx\n\tpop\tax\n\tret\n\n"
    "GetAnInteger:\n\tmov eax,3\n\tmov ebx,2\n\tmov ecx,num\n\tmov edx,6\n\tint 0x80\n\n\tmov edx,eax\n\tmov eax, 4\n\tmov ebx, 1\n\tmov ecx, num\n\tint 80h\n\n"

    "ConvertStringToInteger:\n\tmov ax,0\n\tmov [ReadInt],ax\n\tmov ecx,num\n\n\tmov bx,0\n\tmov bl, byte [ecx]\nNext:\tsub bl,'0'\n\tmov ax,[ReadInt]\n\tmov dx,10\n\t"
	"mul dx\n\tadd ax,bx\n\tmov [ReadInt], ax\n\n\tmov bx,0\n\tadd ecx,1\n\tmov bl, byte[ecx]\n\n\tcmp bl,0xA\n\tjne Next\n\tret\n\n"

    "ConvertIntegerToString:\n\tmov ebx, ResultValue + 4\n\nConvertLoop:\n\tsub dx,dx\n\tmov cx,10\n\tdiv cx\n\tadd dl,'0'\n\tmov [ebx], dl\n\tdec ebx\n\tcmp ebx,ResultValue\n\tjge ConvertLoop\n\n\tret";

    return IO_Output;
}



int main(){

    readCSV("CompilerTable.csv", "Data");
    readCSV("ReservedWords.csv", "Reserved");
    readCSV("PrecedenceTable.csv", "Precedence");
    readTxt("tokensList.txt", "tokens");
    readTxt("symbolTable.txt", "symbol");

    string outputQuad = quadGen(tokensList);
    cout << outputQuad << endl;

    string assemblyCode = assemblyConvert(outputQuad);

    cout << assemblyCode << endl;

    ofstream ioF("IO1Nasm32.asm");
    string out = IO_File(assemblyCode, symbolTable);
    ioF << out;
    ioF.close();

    return 0;
}
