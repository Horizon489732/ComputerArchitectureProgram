#ifndef CPU_H
#define CPU_H

#include <vector>
#include "cache.h"
#include <string>


class CPU
{
private:
    int hit;
    int miss;
    Cache cache;           
    int instructionCount;
    int words;

public:
    CPU(int blocks, int words);

    int getHitCount();
    int getMissCount();
    int getInstructionCount();
    int getWords();
    Cache getCache();

    void setHitCount(int hit);
    void setMissCount(int miss);
    void setInstructionCount(int count);
    void setWords(int words);
    void setCache(const Cache& cache);

    void incrementHitCount();
    void incrementMissCount();
    void incrementInstructionCount();

    void executeInstruction(std::string address);
    std::string printOutputString();
};

#endif
