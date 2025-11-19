#include <iostream>
#include <vector>

using namespace std;

class PageTable {
    private: 
        vector<int> table;   // page → frame, -1 means not loaded
        int numPages;        // number of pages in virtual memory
        int pageSize;        // words per page

    
    public:
        PageTable(int numPages, int pageSize): numPages(numPages), pageSize(pageSize), table(numPages, -1) {}

        int getPageNumber(int virtualAddress) {
            return virtualAddress / pageSize;
        }

        bool isPageLoaded(int pageNumber) {
            return table[pageNumber] != -1;
        }

        void setFrame(int pageNumber, int frameNumber) {
            table[pageNumber] = frameNumber;
        }

        int getFrame(int pageNumber) {
            return table[pageNumber];
        }

        int getPageSize() {
            return pageSize;
        }
};

class MainMemory {
    private:
        int memorySize; //max number of pages/frames
        vector<tuple<int, int>> frames; //(programId, pageNumber)
        int nextFrame;

    public: 
        MainMemory(int memorySize) : memorySize(memorySize), frames(memorySize, {-1,-1}), nextFrame(0) {}

        int addFrameToMemory(int pageNumber, int programId) {

            int currentFrame = this->nextFrame;

            //replace the frame if occupied
            frames[currentFrame] = {programId, pageNumber};

            this->nextFrame = (currentFrame + 1) % memorySize;

            return currentFrame;
        }
};

class CPU {
    
    private:
        MainMemory &memory;
        vector<PageTable> pageTable;
        int currentProgramId;
        int pageFaults;
        int accesses;

    public:
        CPU(MainMemory memory) : memory(memory), currentProgramId(-1), pageFaults(0), accesses(0) {}

        void newProgram(int numPages, int pageSize) {
            pageTable.push_back(PageTable(numPages, pageSize));
            currentProgramId++;
            pageFaults = 0;
            accesses = 0;
        }

        void executeAddress(int virtualAddress) {
            accesses++;
            PageTable &pt = pageTable[currentProgramId];
            int pageNumber = pt.getPageNumber(virtualAddress);
            
            //Page Fault
            if (!pt.isPageLoaded(pageNumber)) {
                pageFaults++;

                int frame = memory.addFrameToMemory(pageNumber, currentProgramId);
                pt.setFrame(pageNumber, frame);
            }

            int frame = pt.getFrame(pageNumber);
            int offset = virtualAddress % pt.getPageSize();
            int physicalAddress = frame * pat.getPageSize() + offset;
        }
};