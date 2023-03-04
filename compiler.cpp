    /*
        
    */


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

vector<vector<string> > csvData;

void readInput(string csvFile){
    ifstream csvInputFile(csvFile);
    if(csvInputFile.is_open()) {
        string line;
        while(getline(csvInputFile, line)) {
            vector<string> row;
            stringstream ss(line);
            string field;
            while(getline(ss, field, ',')){
                row.push_back(field);
            }
            csvData.push_back(row);
        }
    }
    csvInputFile.close();
}

vector<pair<string, string> > tokenizer(string input){
    vector<pair<string, string> > tokens;
    int column;
    string tokenValue;
    string number;
    int j, k, l, m, n, p, q, r;

    for(int i = 0; i < input.size(); i++){
        char ch = input[i];
        switch(ch) {
            default:
                break;

            case 'a'...'z':
            case 'A'...'Z':
                column = 1;
                tokenValue = "";
                tokenValue = tokenValue + ch;
                j = i + 1;
                while(j < input.size() && (isalpha(input[j]) || isdigit(input[j]))){
                    char nextChar = input[j];
                    tokenValue = tokenValue + nextChar;
                    j++;
                }
                tokens.push_back(pair<string, string>(tokenValue, csvData[3][column]));
                i = j - 1;
                break;

            case '0'...'9':
                column = 2;
                number = "";
                number = number + ch;
                k = i + 1;
                while(k < input.size() && input[k] >= '0' && input[k] <= '9'){
                    char nextChar = input[k];
                    number = number + nextChar;
                    k++;
                }
                tokens.push_back(pair<string, string>(number, csvData[4][column]));
                i = k - 1;
                break;

            case '=':
                column = 3;
                l = i + 1;
                if(l < input.size() && input[l] == '='){
                    tokens.push_back(pair<string, string>("==", csvData[16][column]));
                    i = l; //skips the second symbol
                }
                else{
                    tokens.push_back(pair<string, string>("=", csvData[5][column]));
                }
                break;

            case ',':
                column = 4;
                tokens.push_back(pair<string, string>(",", csvData[6][column]));
                break;

            case ';':
                column = 5;
                tokens.push_back(pair<string, string>(";", csvData[7][column]));
                break;
                
            case '+':
                column = 6;
                tokens.push_back(pair<string, string>("+", csvData[8][column]));
                break;

            case '-':
                column = 7;
                tokens.push_back(pair<string, string>("-", csvData[9][column]));
                break;

            case '*':
                column = 8;
                m = i + 1;
                if(m < input.size() && input[m] == '/'){
                    tokens.push_back(pair<string, string>("*/", csvData[23][column]));
                    i = m; //skips the second symbol
                }
                else{
                    tokens.push_back(pair<string, string>("*", csvData[10][column]));
                }
                break;

            case '/':
                column = 9;
                n = i + 1;
                if(n < input.size() && input[n] == '*'){
                    tokens.push_back(pair<string, string>("/*", csvData[22][column]));
                    i = n; //skips the second symbol
                }
                else{
                    tokens.push_back(pair<string, string>("/", csvData[11][column]));
                }
                break;

            case '(':
                column = 10;
                tokens.push_back(pair<string, string>("(", csvData[12][column]));
                break;

            case ')':
                column = 11;
                tokens.push_back(pair<string, string>("(", csvData[13][column]));
                break;

            case '!':
                column = 12;
                p = i + 1;
                if(p < input.size() && input[p] == '='){
                    tokens.push_back(pair<string, string>("<=", csvData[17][column]));
                }
                else{
                    cout << "Error: ! without =";
                    exit(1);
                }
                break;

            case '<':
                column = 13;
                q = i + 1;
                if(q < input.size() && input[q] == '='){
                    tokens.push_back(pair<string, string>("<=", csvData[14][column]));
                    i = q; //skips the second symbol
                }
                else{
                    tokens.push_back(pair<string, string>("<", csvData[18][column]));
                }
                break;

            case '>':
                column = 14;
                r = i + 1;
                if(r < input.size() && input[r] == '='){
                    tokens.push_back(pair<string, string>(">=", csvData[15][column]));
                    i = r; //skips the second symbol
                }
                else{
                    tokens.push_back(pair<string, string>(">", csvData[19][column]));
                }
                break;

            case '{':
                column = 15;
                tokens.push_back(pair<string, string>("(", csvData[20][column]));
                break;

            case '}':
                column = 16;
                tokens.push_back(pair<string, string>("(", csvData[21][column]));
                break;
        }
    }

    return tokens;
}

int main(){
    readInput("CompilerTable.csv");
    ifstream inputFile("inputfile.txt");
    string input; //store input file into this string
    if (inputFile.is_open()){
        getline(inputFile, input, '\0');
    }
    inputFile.close();

    vector<pair<string, string> > tokens = tokenizer(input);

    for (int i = 0; i < tokens.size(); i++){
        pair<string, string> token = tokens[i];
        cout << token.first << " : " << token.second << endl;
    }

    return 0;
}
