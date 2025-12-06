#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

int main() {
    //к 1 и 2
    /*
    std::ifstream inputFile1("input.txt");
    if (!inputFile1.is_open()) {
        std::cerr << "Не удалось открыть файл input.txt" << std::endl;
        return 1;
    }
    
    std::string line;
    std::cout << "Содержимое файла input.txt:" << std::endl;
    
    while (std::getline(inputFile1, line))
        std::cout << line << std::endl;
    
    
    inputFile1.close();
    
    std::ifstream inputFile2("input.txt"); 
    std::ofstream outputFile("output.txt");
    
    if (!inputFile2.is_open()) {
        std::cerr << "Не удалось открыть файл input.txt повторно" << std::endl;
        return 1;
    }
    
    if (!outputFile.is_open()) {
        std::cerr << "Не удалось создать файл output.txt" << std::endl;
        return 1;
    }
    
    while (std::getline(inputFile2, line)) 
        outputFile << line << std::endl;
    
    
    inputFile2.close();
    outputFile.close();
    
    std::cout << "успешно скопировано" << std::endl;
    */

    // к 3
    std::ifstream input("input.txt");
    
    if (!input.is_open()) {
        std::cerr << "Не удалось открыть файл input.txt" << std::endl;
        return 1;
    }
    
    int N, M;
    input >> N >> M;
    
    std::vector<std::vector<int>> table(N, std::vector<int>(M));
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            input >> table[i][j];
            if (j < M - 1) {
                char comma;
                input >> comma; 
            }
        }
    }
    
    input.close();
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            std::cout << std::setw(10) << table[i][j];
            if (j < M - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
    
    return 0;
}