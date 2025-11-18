#include "cache.h"
#include <iostream>

Cache::Cache(int blocks) {
    this->memory.resize(blocks);
}

std::vector<unsigned long long> Cache::getMemory() {
    return memory;
}

void Cache::setMemory(std::vector<unsigned long long>& mem) {
    memory = mem;
}

unsigned long long Cache::getMemoryAt(int index) {
    if (index < 0 || index >= static_cast<int>(memory.size()))
        throw std::out_of_range("Cache index out of range");
    return memory[index];
}

void Cache::setMemoryAt(int index, unsigned long long value) {
    if (index < 0 || index >= static_cast<int>(memory.size()))
        throw std::out_of_range("Cache index out of range");
    memory[index] = value;
}

int Cache::getMemorySize() {
    return static_cast<int>(memory.size());
}
