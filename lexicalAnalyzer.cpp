#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
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
    bool comment = false;
    bool isProgramName = false;

    for(int i = 0; i < input.size(); i++){
        char currentChar = input[i];
        switch(currentChar) {
            default:
                break;

            case 'a'...'z':
            case 'A'...'Z':
                int j;
                if(comment == false){
                    column = 1;
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
                        tokens.push_back(pair<string, string>(token, "$CLASS"));
                        isProgramName = true;
                    }
                    else if(token == csvRW[6]){
                        tokens.push_back(pair<string, string>(token, "$CIN"));
                    }
                    else if(token == csvRW[7]){
                        tokens.push_back(pair<string, string>(token, "$COUT"));
                    }
                    else{
                        if(isProgramName == true){
                            tokens.push_back(pair<string, string>(token, "PGM NAME"));
                            isProgramName = false;
                        }
                        else{
                            tokens.push_back(pair<string, string>(token, csvData[3][column]));
                        }
                    }
                    i = j - 1;
                }
                break;

            case '0'...'9':
                if(comment == false){
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
                }
                break;

            case '=':
                if(comment == false){
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
                }
                break;

            case ',':
                if(comment == false){
                    column = 4;
                    tokens.push_back(pair<string, string>(",", csvData[6][column]));
                }
                break;

            case ';':
                if(comment == false){
                    column = 5;
                    tokens.push_back(pair<string, string>(";", csvData[7][column]));
                }
                break;
                
            case '+':
                if(comment == false){
                    column = 6;
                    tokens.push_back(pair<string, string>("+", csvData[8][column]));
                }
                break;

            case '-':
                if(comment == false){
                    column = 7;
                    tokens.push_back(pair<string, string>("-", csvData[9][column]));
                }
                break;

            case '*':
                if((comment == true) && (input[i+1] == '/')){
                    column = 8;
                    int m;
                    m = i + 1;
                    if(m < input.size() && input[m] == '/'){
                        i = m; //skips the second symbol
                        comment = false;
                    }
                    else{
                        tokens.push_back(pair<string, string>("*", csvData[10][column]));
                    }
                }
                else{
                    if(comment == false){
                        tokens.push_back(pair<string, string>("*", csvData[10][column]));
                    }
                }
                break;
                

            case '/':
                if(comment == false){
                    column = 9;
                    int n;
                    n = i + 1;
                    if(n < input.size() && input[n] == '*'){
                        i = n; //skips the second symbol
                        comment = true;
                    }
                    else{
                        tokens.push_back(pair<string, string>("/", csvData[11][column]));
                    }
                }
                break;

            case '(':
                if(comment == false){
                    column = 10;
                    tokens.push_back(pair<string, string>("(", csvData[12][column]));
                }
                break;

            case ')':
                if(comment == false){
                    column = 11;
                    tokens.push_back(pair<string, string>(")", csvData[13][column]));
                }
                break;

            case '!':
                if(comment == false){
                    column = 12;
                    int p;
                    p = i + 1;
                    if(p < input.size() && input[p] == '='){
                        tokens.push_back(pair<string, string>("!=", csvData[17][column]));
                        i = p; //skips the second symbol
                    }
                    else{
                        cout << "Error: ! without =";
                        exit(1);
                    }
                }
                break;

            case '<':
                if(comment == false){
                    column = 13;
                    int q;
                    q = i + 1;
                    if(q < input.size() && input[q] == '='){
                        tokens.push_back(pair<string, string>("<=", csvData[14][column]));
                        i = q; //skips the second symbol
                    }
                    else{
                        tokens.push_back(pair<string, string>("<", csvData[18][column]));
                    }
                }
                break;

            case '>':
                if(comment == false){
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
                }
                break;

            case '{':
                if(comment == false){
                    column = 15;
                    tokens.push_back(pair<string, string>("{", csvData[20][column]));
                }
                break;

            case '}':
                if(comment == false){
                    column = 16;
                    tokens.push_back(pair<string, string>("}", csvData[21][column]));
                }
                break;
        }
    }

    return tokens;
}



struct Symbol {
    string token;
    string type;
    string address;
    string segment;
    string value;
};

bool symbolExists(vector<Symbol>& symbolTable, string token){
    for(Symbol& symbol : symbolTable){
        if(symbol.token == token){
            return true;
        }
    }
    return false;
}

vector<Symbol> createSymbolTable(vector<pair<string, string> > tokens){
    vector<Symbol> symbolTable;

    int address = 0;
    bool initialValue = false;
    bool constVal = false;

    for(int i = 0; i < tokens.size(); i++){
        string tokenStr = tokens[i].first;
        string typeStr = tokens[i].second;
        string addressStr = to_string(address);
        string val = "";
        
        if(typeStr == "$CLASS" || typeStr == "$VAR" || typeStr == "$CONST" || typeStr == "$CIN" || typeStr == "$COUT"|| tokenStr == "=" || tokenStr == "+" || tokenStr == "-" || tokenStr == "(" || tokenStr == ")" || tokenStr == "*" || tokenStr == "/" || tokenStr == "IF" || tokenStr == "THEN" || tokenStr == "==" || tokenStr == "!=" || tokenStr == ">" || tokenStr == "<" || tokenStr == ">=" || tokenStr == "<=" || tokenStr == "{" || tokenStr == "}" || tokenStr == ";"){
            //exclude from symbol table
        }
        else if(constVal == true){
            constVal = false;
        }
        else if(typeStr == "PGM NAME"){
            Symbol symbol = {tokenStr, typeStr, addressStr, "CS", ""};
            symbolTable.push_back(symbol);
        }
        else{
            if(initialValue == true){
                initialValue = false;
            }
            else{
                //checks if token is a number
                bool isNumber = false;
                for(char c : tokenStr){
                    if(isdigit(c)){
                        isNumber = true;
                        break;
                    }
                }
                if(isNumber){
                    val = tokenStr;
                }
                initialValue = false;

                //CONST handling and initial value for CONST
                if(tokens[i-1].first == "CONST" && tokens[i+1].first == "=" && isdigit(static_cast<int>(tokens[i+2].first[0]))){
                        Symbol symbol = {tokenStr, "CONST", addressStr, "DS", tokens[i+2].first};
                        symbolTable.push_back(symbol);
                        address = address + 2;
                        constVal = true;
                }

                //adds token to symbol table
                else if(isNumber == true){
                    if(!symbolExists(symbolTable, tokenStr)){
                        Symbol symbol = {tokenStr, "NUM LIT", addressStr, "DS", val};
                        symbolTable.push_back(symbol);
                        address = address + 2;
                    }
                }
                else{
                    if(!symbolExists(symbolTable, tokenStr)){
                        //handles inital value for VAR
                        if(tokens[i+1].first == "=" && isdigit(static_cast<int>(tokens[i+2].first[0])) && tokens[i+3].first == ";"){
                            Symbol symbol = {tokenStr, "VAR", addressStr, "DS", tokens[i+2].first};
                            symbolTable.push_back(symbol);
                            address = address + 2;
                            initialValue = true;
                        }
                        else{
                            Symbol symbol = {tokenStr, "VAR", addressStr, "DS", val};
                            symbolTable.push_back(symbol);
                            address = address + 2;
                        }
                    }
                    
                }
            }

        }

    }
    for(auto it = symbolTable.begin(); it != symbolTable.end(); ){
        if(it->token == "="){
            it = symbolTable.erase(it);
        }
        else{
            ++it;
        }
    }

    return symbolTable;
}



int main(){
    readCSV("CompilerTable.csv", "Data");
    readCSV("ReservedWords.csv", "Reserved");
    ifstream inputFile("InputFile.txt");
    string input; //store input file here
    if (inputFile.is_open()){
        getline(inputFile, input, '\0');
    }
    inputFile.close();
    
    //lexical
    vector<pair<string, string> > tokensList;
    pair<string, string> tokenPair;
    tokensList = tokenizer(input);
    vector<Symbol> symbolTableResult = createSymbolTable(tokensList);

    ofstream tokensFile("tokensList.txt"); //output to tokensList.txt
    for(const auto& tokenPair : tokensList){
        tokensFile << tokenPair.first << "\t" << " : " << "\t" << tokenPair.second << endl;
    }
    tokensFile.close();

    ofstream symbolTableFile("symbolTable.txt"); //output to symbolTable.txt
    for(const auto& symbol : symbolTableResult){
        symbolTableFile << symbol.token << "\t" << symbol.type << "\t" << symbol.address << "\t" << symbol.segment << "\t" << symbol.value << endl;
    }
    symbolTableFile.close();

    return 0;
}
