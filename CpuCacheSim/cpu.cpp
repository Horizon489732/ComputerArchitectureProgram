#include "cpu.h"
#include <iostream>
#include <iomanip>
#include <regex>
#include <sstream>

// constructor
CPU::CPU(int blocks, int words)
    : cache(blocks)
{
    this->hit = 0;
    this->miss = 0;
    this->instructionCount = 0;
    this->words = words;
}

// getters
int CPU::getHitCount() {
    return hit;
}

int CPU::getMissCount() {
    return miss;
}

int CPU::getInstructionCount() {
    return instructionCount;
}

int CPU::getWords() {
    return words;
}

Cache CPU::getCache() {
    return cache;
}

// setters
void CPU::setHitCount(int hit) {
    this->hit = hit;
}

void CPU::setMissCount(int miss) {
    this->miss = miss;
}

void CPU::setInstructionCount(int instructionCount) {
    this->instructionCount = instructionCount;
}

void CPU::setWords(int words) {
    this->words = words;
}

void CPU::setCache(const Cache& cache) {
    this->cache = cache;
}

void CPU::incrementHitCount() {
    this->hit++;
}

void CPU::incrementMissCount() {
    this->miss++;
}

void CPU::incrementInstructionCount() {
    this->instructionCount++;
}

void CPU::executeInstruction(std::string address) {
    incrementInstructionCount();

    // spliting string learned from GeeksforGeeks
    // Delimiter pattern
    std::regex delimiter(" ");

    // Create a regex_token_iterator 
    // to split the string
    std::sregex_token_iterator it(address.begin(), address.end(), delimiter, -1);

    // End iterator for the 
    // regex_token_iterator
    std::sregex_token_iterator end;

    // Splitting the Address
    std::vector<std::string> tokens;
    while (it != end) {
        tokens.push_back(it -> str());
        it++;
    }

    if (tokens.empty()) {
        return;
    }

    std::string command = tokens[0];

    std::cout << "\n-------[DEBUG] Executing instruction: " << command << "\n";
    std::cout << "[DEBUG] Words per block: " << this->words << "\n";
    std::cout << "[DEBUG] Current instruction count: " << this->instructionCount << "\n";

    // start with DEL
    if(command.find("DEL") == 0) {

        // Check block number to delete
        try {
            unsigned long long blockNum = std::stoull(tokens[1], nullptr, 16);
            this->cache.setMemoryAt(static_cast<int>(blockNum), 0); //Delete the block
            std::cout << "Deleted block " << blockNum << " from cache.\n";
        } catch (const std::out_of_range& e) {//This error catch block is written by ChatGPT
            std::cerr << "Error: Block number " << tokens[1] << " is out of range.\n";
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid block number format: " << tokens[1] << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Unexpected error: " << e.what() << "\n";
        }
    } else if (command.find("CLEAR") == 0) {
        for (int i = 0; i < this->cache.getMemorySize(); ++i) {
            this->cache.setMemoryAt(i, 0);
        }
        std::cout << "Cache cleared.\n";
    } else {
        try {
        // Convert hex string to decimal address
        unsigned long long decimal = std::stoull(command, nullptr, 16);

        // Compute which block in the cache this address maps to
        unsigned long long blockNum = decimal / this->words; 
        int blockIndex = static_cast<int>(blockNum % this->cache.getMemorySize());
        std::cout << "[DEBUG] Decimal address: " << decimal << "\n";
        std::cout << "[DEBUG] Block number: " << blockNum << "\n";
        std::cout << "[DEBUG] Block index in cache: " << blockIndex << "\n";

        int i = 0;
        bool isContinue = true;
        
        // Looking for block in the cache. If found, it is a hit
        while (isContinue && i < this->cache.getMemorySize()) {
            if (this->cache.getMemoryAt(i) == blockNum) {
                incrementHitCount();
                isContinue = false;
            } else {
                i++;
            }
        }

        // Reach the end and still can not find block. It is a miss
        if (i == this->cache.getMemorySize()) {
            this->cache.setMemoryAt(blockIndex, blockNum);
            incrementMissCount();
        }

    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid address format: " << command << "\n";
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Address " << command << " is out of range\n";
    }
    }
}

std::string CPU::printOutputString() {
    std::ostringstream out;

    double total = static_cast<double>(hit + miss);
    double hitRatio = (total == 0.0) ? 0.0 : static_cast<double>(hit) / total;

    out << "Instruction# - " << instructionCount << "\n";
    out << "H - " << std::fixed << std::setprecision(3) << hitRatio << "\n";

    for (int i = 0; i < cache.getMemorySize(); ++i) {
        if (i > 0) out << "  "; // space between number

        int block = cache.getMemoryAt(i);
        if (block == 0) {
            out << std::setw(10) << "---------"; 
        } else {
            out << block;
        }
    }
    out << "\n\n";

    return out.str();
}