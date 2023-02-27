/*
Current tasks/problems:
    -use the .csv file
    -use a struct (its like a record in ada)
*/


#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

vector<string> tokenizer(string input) {
    vector<string> operators;
    operators.push_back("=");
    operators.push_back(",");
    operators.push_back(";");
    operators.push_back("+");
    operators.push_back("-");
    operators.push_back("*");
    operators.push_back("/");
    operators.push_back("(");
    operators.push_back(")");
    operators.push_back("<=");
    operators.push_back(">=");
    operators.push_back("==");
    operators.push_back("!=");
    operators.push_back("<");
    operators.push_back(">");
    operators.push_back("{");
    operators.push_back("}");
    operators.push_back("/*");
    operators.push_back("*/");

    vector<string> tokens;
    string currentToken = "";

    for (int i = 0; i < input.size(); i++) {
        char currentChar = input[i];

        if (isspace(currentChar)) {
            continue;
        }
        else if (isalnum(currentChar)) {
            currentToken = currentToken + currentChar;
            while (i < input.size() - 1 && isalnum(input[i + 1])) {
                currentToken = currentToken + input[i + 1];
                i++;
            }
            bool foundOperator = false;
            for (int j = 0; j < operators.size(); j++) {
                if (currentToken == operators[j]) {
                    foundOperator = true;
                    break;
                }
            }
            if (foundOperator == false) {
                tokens.push_back(currentToken);
            }
            currentToken = "";
        }
        else {
            currentToken = currentToken + currentChar;
            bool foundOperator = false;
            for (int j = 0; j < operators.size(); j++) {
                if (currentToken == operators[j]) {
                    foundOperator = true;
                    break;
                }
            }
            if (foundOperator == false) {
                if (i < input.size() - 1) {
                    string combinedToken = currentToken + input[i + 1];
                    bool foundCombinedOperator = false;
                    for (int j = 0; j < operators.size(); j++) {
                        if (combinedToken == operators[j]) {
                            foundCombinedOperator = true;
                            break;
                        }
                    }
                    if (foundCombinedOperator) {
                        currentToken = combinedToken;
                        tokens.push_back(currentToken);
                        currentToken = "";
                        i++;
                    }
                    else {
                        tokens.push_back(currentToken);
                        currentToken = "";
                    }
                }
                else {
                    tokens.push_back(currentToken);
                    currentToken = "";
                }
            }
            else if (i < input.size() - 1) {
                string combinedToken = currentToken + input[i + 1];
                bool foundCombinedOperator = false;
                for (int j = 0; j < operators.size(); j++) {
                    if (combinedToken == operators[j]) {
                        foundCombinedOperator = true;
                        break;
                    }
                }
                if (foundCombinedOperator) {
                    currentToken = combinedToken;
                    tokens.push_back(currentToken);
                    currentToken = "";
                    i++;
                }
                else {
                    tokens.push_back(currentToken);
                    currentToken = "";
                }
            }
            else {
                tokens.push_back(currentToken);
                currentToken = "";
            }
        }
    }

    if (currentToken != "") {
        tokens.push_back(currentToken);
    }

    return tokens;
}



int main() {
    ifstream inputFile;
    inputFile.open("inputfile.txt");

    string input((istreambuf_iterator<char>(inputFile)), (istreambuf_iterator<char>())); //Reads input into string

    inputFile.close();

    vector<string> printTokens = tokenizer(input); //Feeds the string into the tokenizer
    for (int i = 0; i < printTokens.size(); i++) {
        cout << printTokens[i] << "\n";
    }

    return 0;
}

