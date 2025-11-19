#include "cpu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

int main() {
    std::cout << "Please enter your input path: ";

    std::string path;
    std::getline(std::cin, path);

    std::ifstream input_file(path);

    if (!input_file.is_open())
    {
        std::cerr << "Error: Could not open file \"" << path << "\"" << std::endl;
        return 1;
    }

    std::ofstream output_file("output.txt");

    std::string firstLine;
    if (!std::getline(input_file, firstLine)) {
        std::cerr << "Error: Input file is empty" << std::endl;
        return 1;
    }
    std::vector<std::string> tokens;
    std::istringstream iss(firstLine);
    std::string temp;
    while (iss >> temp) {
        tokens.push_back(temp);
    }

    int wordsPerBlock = std::stoi(tokens[0], nullptr, 16);
    int numBlocks = std::stoi(tokens[1], nullptr, 16);

    CPU cpu(numBlocks, wordsPerBlock);

     std::string line;
    while (std::getline(input_file, line)) {
        if (line.empty()) continue;

        cpu.executeInstruction(line);

        if (cpu.getInstructionCount() % 100 == 0) {
            output_file << cpu.printOutputString();
        }
    }

    // Print remaining instructions if not a multiple of 100
    if (cpu.getInstructionCount() % 100 != 0) {
        output_file << cpu.printOutputString();
    }

    std::cout << "Complete. Output written to output.txt\n";
    return 0;
}