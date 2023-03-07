#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

vector<vector<string> > csvData;
vector<string> csvRW;

void readInput(string csvFile, string kind){
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
            else{
                exit(1);
            }
        }
    }
    csvInputFile.close();
}



vector<pair<string, string> > tokenizer(string input){
    vector<pair<string, string> > tokens;
    int column;
    string token;
    string num;

    for(int i = 0; i < input.size(); i++){
        char currentChar = input[i];
        switch(currentChar) {
            default:
                break;

            case 'a'...'z':
            case 'A'...'Z':
                column = 1;
                int j;
                token = "";
                token = token + currentChar;
                j = i + 1;
                while(j < input.size() && (isalpha(input[j]) || isdigit(input[j]))){
                    char nextChar = input[j];
                    token = token + nextChar;
                    j++;
                }
                if(token == csvRW[1]){
                    tokens.push_back(pair<string, string>(token, "$CONST"));
                }
                else if(token == csvRW[2]){
                    tokens.push_back(pair<string, string>(token, "$IF"));
                }
                else if(token == csvRW[3]){
                    tokens.push_back(pair<string, string>(token, "$VAR"));
                }
                else if(token == csvRW[4]){
                    tokens.push_back(pair<string, string>(token, "$THEN"));
                }
                else if(token == csvRW[5]){
                    tokens.push_back(pair<string, string>(token, "$PROCEDURE"));
                }
                else if(token == csvRW[6]){
                    tokens.push_back(pair<string, string>(token, "$WHILE"));
                }
                else if(token == csvRW[7]){
                    tokens.push_back(pair<string, string>(token, "$CALL"));
                }
                else if(token == csvRW[8]){
                    tokens.push_back(pair<string, string>(token, "$DO"));
                }
                else if(token == csvRW[9]){
                    tokens.push_back(pair<string, string>(token, "$ODD"));
                }
                else if(token == csvRW[10]){
                    tokens.push_back(pair<string, string>(token, "$CLASS"));
                }
                else {
                    tokens.push_back(pair<string, string>(token, csvData[3][column]));
                }
                i = j - 1;
                break;

            case '0'...'9':
                column = 2;
                int k;
                num = "";
                num = num + currentChar;
                k = i + 1;
                while(k < input.size() && input[k] >= '0' && input[k] <= '9'){
                    char nextChar = input[k];
                    num = num + nextChar;
                    k++;
                }
                tokens.push_back(pair<string, string>(num, csvData[4][column]));
                i = k - 1;
                break;

            case '=':
                column = 3;
                int l;
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
                int m;
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
                int n;
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
                int p;
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
                int q;
                j = i + 1;
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
                int r;
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
                tokens.push_back(pair<string, string>("{", csvData[20][column]));
                break;

            case '}':
                column = 16;
                tokens.push_back(pair<string, string>("}", csvData[21][column]));
                break;
        }
    }

    return tokens;
}


int main(){
    readInput("CompilerTable.csv", "Data");
    readInput("ReservedWords.csv", "Reserved");
    ifstream inputFile("InputFile.txt");
    string input; //store input file here
    if (inputFile.is_open()){
        getline(inputFile, input, '\0');
    }
    inputFile.close();
    
    vector<pair<string, string> > tokensList;
    pair<string, string> tokenPair;
    tokensList = tokenizer(input);

    for (int i = 0; i < tokensList.size(); i++){
        tokenPair = tokensList[i];
        cout << tokenPair.first << " : " << tokenPair.second << endl;
    }

    return 0;
}
