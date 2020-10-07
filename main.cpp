#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::string INPUT; // will be '/' if set to stdin
std::string OUTPUT; // will be '/' if set to stdout


bool isArgValid(int argc, char* argv[]){
    // nie su ziadne argumenty
    if (argc == 1){
        INPUT = "/";
        OUTPUT = "/";
        return true;
    }
    char set = 0; // i = input, o = output, b = both
    std::string currentArg;
    for (size_t i = 1; i < argc; i++){
        currentArg = argv[i];
        if (currentArg == "-i" && argv[i +1]){
            INPUT = argv[++i];
            set = set != 0 ? 'b' : 'i';
            continue;
        }
        if (currentArg == "-o" && argv[i + 1]){
            OUTPUT = argv[++i];
            set = set != 0 ? 'b' : 'o';
            continue;
        }
        return false;
    }
    OUTPUT = set=='i' ? "/" : OUTPUT;
    INPUT = set=='o' ? "/" : INPUT;
    std::cout << "INPUT: " << INPUT << "\n";
    std::cout << "OUTPUT: " << OUTPUT << "\n";
    return true;
}

std::ifstream readInput(){
    return std::ifstream(INPUT);
}

std::vector<std::vector<char>> getSudokuFromLine(std::string& rawLine){
    std::vector<std::vector<char>> sudoku(9);
    std::vector<char> sudokuRow;
    for (size_t row = 0; row < 9;row++){
        for (int col = 0; col < 9; col++) {
            if (rawLine[(row*9)+col] == '.'){
                sudoku[row].push_back(0);
                continue;
            }
            sudoku[row].push_back(rawLine[(row*9)+col] - '0');
        }
    }
    return sudoku;
}

bool isColumnPossible(std::vector<std::vector<char>>& sudoku, size_t row, size_t col, char value){
    for (size_t i = 0; i < 9; i++){
        if (i != col && sudoku[i][col] == value){
            return false;
        }
    }
    return true;
}

bool isRowPossible(std::vector<std::vector<char>>& sudoku, size_t row, size_t col, char value){
    for (size_t i = 0; i < 9; i++){
        if (i != col && sudoku[row][i] == value){
            return false;
        }
    }
    return true;
}

bool isSquarePossible(std::vector<std::vector<char>>& sudoku, size_t row, size_t col, char value){

}


bool isPossible(std::vector<std::vector<char>>& sudoku, size_t row, size_t col, char value){
    return true;
}

void solve(std::vector<std::vector<char>>& sudoku){
    for (size_t row = 0; row < 9; row++){
        for (size_t col = 0; col < 9; col++){
            if (sudoku[row][col] == 0){
                for (char i = 1; i < 10; i++){
                    if (isPossible(sudoku,row,col,i)){
                        sudoku[row][col] = i;
                        solve(sudoku);
                        sudoku[row][col] = 0;
                    }
                }
            }
        }
    }
}



int main(int argc, char* argv[]) {
    if (!isArgValid(argc,argv)){
        std::cerr << "BRUH" << "\n";
        return 1;
    }
    std::ifstream inputFile = readInput();
    std::string buffer;
    while (std::getline(inputFile,buffer)){
        getSudokuFromLine(buffer);

    }
    return 0;
}
