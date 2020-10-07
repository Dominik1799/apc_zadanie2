#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>

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
    return true;
}

std::ifstream readInput(){
    return std::ifstream(INPUT);
}

std::vector<std::vector<char>> getSudokuFromLine(std::string& rawLine){
    std::vector<std::vector<char>> sudoku(9);
    std::vector<char> sudokuRow;
    int tempNumber;
    for (size_t row = 0; row < 9;row++){
        for (int col = 0; col < 9; col++) {
            if (rawLine[(row*9)+col] == '.'){
                sudoku[row].push_back(0);
                continue;
            }
//            sudoku[row].push_back(rawLine[(row*9)+col] - '0');
            tempNumber = rawLine[(row*9)+col] - '0';
            if (tempNumber < 10 && tempNumber >= 0){
                sudoku[row].push_back(tempNumber);
            }
        }
        if(sudoku[row].size() != 9){
            std::cerr << "Invalid input";
            exit(1);
        }
    }
    return sudoku;
}

bool isColumnPossible(std::vector<std::vector<char>>& sudoku, size_t row, size_t col, char value){
    for (size_t i = 0; i < 9; i++){
        if (i != row && sudoku[i][col] == value){
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
    size_t squareRow = row - (row % 3);
    size_t squareCol = col - (col % 3);
    for (int i = squareRow; i < squareRow+3; ++i) {
        for (int j = squareCol; j < squareCol+3; ++j) {
            if (row != i && col != j && sudoku[i][j] == value){
                return false;
            }
        }
    }
    return true;
}


bool isPossible(std::vector<std::vector<char>>& sudoku, size_t row, size_t col, char value){
    if (isRowPossible(sudoku,row,col,value) && isColumnPossible(sudoku,row,col,value) && isSquarePossible(sudoku,row,col,value)){
        return true;
    }
    return false;
}

std::string sudokuToString(std::vector<std::vector<char>> sudoku){
    std::string result;
    for (size_t row = 0; row < 9;row++){
        for (int col = 0; col < 9; col++) {
            result.push_back((char)(sudoku[row][col] + '0'));
        }
    }
    return result;
}

void printOutput(std::vector<std::string>& results){
    std::ofstream  outputFile(OUTPUT);
    for(const auto& line : results){
        (OUTPUT != "/" ? outputFile : std::cout) << line << "\n";
    }
    if (OUTPUT != "/" && outputFile.fail()) {
        std::cerr << "Output error";
        exit(1);
    }
}


void solve(std::vector<std::vector<char>>& sudoku,bool& isSolved){
    for (size_t row = 0; row < 9; row++){
        for (size_t col = 0; col < 9; col++){
            if (sudoku[row][col] == 0){
                for (char i = 1; i < 10; i++){
                    if (isPossible(sudoku,row,col,i)){
                        sudoku[row][col] = i;
                        solve(sudoku,isSolved);
                        if (!isSolved){
                            sudoku[row][col] = 0;
                        } else
                            return;
                    }
                }
                return;
            }
        }
    }
    isSolved = true;
}

bool isSolvedCorrectly(std::vector<std::vector<char>>& sudoku){
    for (size_t row = 0; row < 9;row++){
        for (int col = 0; col < 9; col++) {
            if (!isPossible(sudoku,row,col,sudoku[row][col]))
                return false;
        }
    }
    return true;
}





int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    if (!isArgValid(argc,argv)){
        std::cerr << "Arguments are not valid" << "\n";
        return 1;
    }
    std::vector<std::vector<char>> currentGame;
    std::vector<std::string> results;
    std::ifstream inputFile = readInput();
    std::string buffer;
    bool isSolved = false;
    bool isFileEmpty = true;
    while (std::getline(INPUT != "/" ? inputFile : std::cin,buffer)){
        isFileEmpty = false;
        currentGame = getSudokuFromLine(buffer);
        solve(currentGame, isSolved);
        if(isSolved && isSolvedCorrectly(currentGame)){
            results.push_back(sudokuToString(currentGame));
        } else{
            results.emplace_back("");
        }
        isSolved = false;
    }
    if (isFileEmpty){
        std::cerr << "input file error. Empty or non-existent";
        return 1;
    }
    printOutput(results);
    if (INPUT != "/" && !inputFile.eof())
        return 1;
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << " microseconds";

    return 0;
}
