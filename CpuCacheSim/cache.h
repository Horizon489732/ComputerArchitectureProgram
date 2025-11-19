#ifndef CACHE_H
#define CACHE_H
#include <vector>

class Cache
{
private:
    std::vector<unsigned long long> memory;

public:
    Cache(int blocks);

    std::vector<unsigned long long> getMemory();
    void setMemory(std::vector<unsigned long long>& memory);

    unsigned long long getMemoryAt(int index);
    void setMemoryAt(int index, unsigned long long value);

    int getMemorySize();
};

#endif