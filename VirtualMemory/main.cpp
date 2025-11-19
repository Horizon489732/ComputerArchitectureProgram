#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <tuple>

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

        int getNumPages() {
            return numPages;
        }

        vector<int> getTable() {
            return table;
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

class ProgramStats {
    private:
        int pageFaults;
        int accesses;
        vector<vector<int>> snapshots; // snapshots of page table after each access

    public:
        ProgramStats() : pageFaults(0), accesses(0) {}

        void increaseAccess() { accesses++; }
        void increasePageFault() { pageFaults++; }
        void addSnapshot(const vector<int>& snapshot) { snapshots.push_back(snapshot); }

        double getPageFaultRate() const {
            return accesses == 0 ? 0.0 : (double)pageFaults / accesses * 100;
        }

        vector<vector<int>> getSnapshots() { return snapshots; }
};

class CPU {
    
    private:
        MainMemory &memory;
        vector<PageTable> pageTables; //all pages seen
        vector<ProgramStats> statsPerProgram;
        int currentProgramId;

    public:
        CPU(MainMemory& memory) : memory(memory), currentProgramId(-1) {}

        void newProgram(int numPages, int pageSize) {
            pageTables.push_back(PageTable(numPages, pageSize));
            statsPerProgram.push_back(ProgramStats());
            currentProgramId++;
        }

        void executeAddress(int virtualAddress) {
            PageTable &pt = pageTables[currentProgramId];
            ProgramStats &stats = statsPerProgram[currentProgramId];

            stats.increaseAccess();
            int pageNumber = pt.getPageNumber(virtualAddress);  

            //Used AI for this check
            if (pageNumber < 0 || pageNumber >= pt.getNumPages()) {
                cerr << "ERROR: Virtual address " << virtualAddress
                    << " refers to page " << pageNumber
                    << " but program only has " << pt.getNumPages() << " pages." 
                    << endl;
                exit(1);
            }
            
            //Page Fault
            if (!pt.isPageLoaded(pageNumber)) {
                stats.increasePageFault();

                int frame = memory.addFrameToMemory(pageNumber, currentProgramId);
                pt.setFrame(pageNumber, frame);
            }

            stats.addSnapshot(pt.getTable());
        }

        void getOuputResult(ofstream &out) {
            for (int prog = 0; prog < pageTables.size(); prog++) {
                ProgramStats& stats = statsPerProgram[prog];
                vector<vector<int>> snapshots = stats.getSnapshots();

                for (auto& snapshot : snapshots) {
                    // Print page fault rate first
                    double rate = stats.getPageFaultRate();
                    out << fixed << setprecision(3) << rate << "% ";

                    // Print first 8 entries on the same line as the page fault rate
                    int i = 0;
                    for (; i < 8 && i < snapshot.size(); i++) {
                        out << (snapshot[i] == -1 ? "-" : to_string(snapshot[i])) << " ";
                    }
                    out << "\n";

                    // Print remaining entries
                    if (i < snapshot.size()) {
                        out << "        ";
                        int count = 0;
                        for (; i < snapshot.size(); i++) {
                            out << (snapshot[i] == -1 ? "-" : to_string(snapshot[i])) << " ";
                            count++;
                            if (count % 8 == 0 && i+1 < snapshot.size()) out << "\n        ";
                        }
                        out << "\n";
                    }
                }
            }
        }
};

int main() {
    string filename;
    cout << "Enter input file name: ";
    cin >> filename;

    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file " << filename << endl;
        return 1;
    }

    int pageSize, memorySize;
    file >> pageSize >> memorySize;

    MainMemory memory(memorySize);
    CPU cpu(memory);

    string line;
    while (file >> line) {
        if (line == "NEW") {
            int numPages;
            file >> numPages;
            cpu.newProgram(numPages, pageSize);
        } else {
            int virtualAddress = stoi(line, nullptr, 2);
            cpu.executeAddress(virtualAddress);
        }
    }

    ofstream outFile("output.txt");
    cpu.getOuputResult(outFile);
    outFile.close();

    cout << "Output written to output.txt\n";
}