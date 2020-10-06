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
            sudoku[row].push_back(rawLine[(row*9)+col] - '0');
        }
    }
    return sudoku;
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
