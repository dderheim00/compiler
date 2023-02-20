/*

    maybe:
    
    if current character == letter or number{
        if next character is also letter or number{
            continue parsing token
        }
        else{
            end token
        }
    }

    else if current character == =,,,;,+,-,*,/,(,),<,>,{,} {
        if current character == =, >, <, !, /, or * { //these can be the first of a multi-char token, such as >= or !=
            if next character = the next char in a multi-char token{
                parse it then end token
            }
            else{
                not a multi-char token. end token
            }
        }

    }

    else if next character = space{
        end token
    }

*/


//I should create a datatype that can handle


#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct operator_data_type {
    std::string operators = "=,+,-,*,/,<,>,!,%,^,&,|,~,<<,>>,==,!=,<=,>=,&&,||,++,--";
    bool is_operator(char c) {
        return operators.find(c) != std::string::npos;
    }
};

vector<string> tokenize(string input) {
    operator_data_type op_type;
    vector<string> tokens;
    string current_token = "";
    for (int i = 0; i < input.size(); i++) {
        char current_char = input[i];

        if (isalnum(current_char) || op_type.is_operator(current_char)) {
            current_token += current_char;
            if (i < input.size() - 1 && !isalnum(input[i + 1])) {
                tokens.push_back(current_token);
                current_token = "";
            }
        }
        else if (current_char == '=' || current_char == ',' || current_char == ';' || current_char == '+' || current_char == '-' || current_char == '*' || current_char == '/' || current_char == '(' || current_char == ')' || current_char == '<' || current_char == '>' || current_char == '{' || current_char == '}') {
            current_token += current_char;
            if (current_char == '=' || current_char == '<' || current_char == '>' || current_char == '!' || current_char == '/' || current_char == '*') {
                if (i < input.size() - 1) {
                    char next_char = input[i + 1];
                    if ((current_char == '=' && next_char == '=') || (current_char == '>' && next_char == '=') || (current_char == '<' && next_char == '=') || (current_char == '!' && next_char == '=') || (current_char == '/' && next_char == '*') || (current_char == '*' && next_char == '/')) {
                        current_token += next_char;
                        i++;
                    }
                }
            }
            tokens.push_back(current_token);
            current_token = "";
        }
        else if (i < input.size() - 1 && input[i + 1] == ' ') {
            current_token += current_char;
            tokens.push_back(current_token);
            current_token = "";
        }
    }
    if (current_token != "") {
        tokens.push_back(current_token);
    }
    return tokens;
}


int main() {
    string input;
    cout << "Type your input: ";
    getline(cin, input);
    vector<string> printTokens;
    printTokens = tokenize(input);

    cout << "Tokens:\n";
    for (const auto& token : printTokens) {
        cout << token << "\n";
    }
    cout << endl;

    return 0;
}
