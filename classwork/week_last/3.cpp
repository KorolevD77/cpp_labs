#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream input("input.txt", std::ios::binary);
    if (!input) {
        std::cerr << "Файл input.txt не найден!" << std::endl;
        return 1;
    }
    
    std::string encrypted((std::istreambuf_iterator<char>(input)),
                         std::istreambuf_iterator<char>());
    input.close();
    
    for (int shift = 31; shift < 40; ++shift) {
        std::string decrypted;
        
        for (char c : encrypted) {
            char decrypted_char = c;
            
            if (c >= 32 && c <= 126) {
                int value = static_cast<int>(c) - shift;
                while (value < 32) {
                    value += (126 - 32 + 1);
                }
                while (value > 126) {
                    value -= (126 - 32 + 1);
                }
                decrypted_char = static_cast<char>(value);
            }
            
            decrypted += decrypted_char;
        }
        
        std::string output_filename = "output_shift_" + std::to_string(shift) + ".txt";
        std::ofstream output(output_filename);
        output << decrypted;
        output.close();
        
        std::cout << "Расшифровано со сдвигом " << shift << " -> " << output_filename << std::endl;
    }
        
    return 0;
}