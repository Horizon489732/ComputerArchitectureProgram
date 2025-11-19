README file is written by ChatGPT

This program simulates paging in virtual memory. It models a CPU accessing virtual addresses of multiple programs and keeps track of page tables, page faults, and frame allocation in main memory.


The program reads an input file containing:
First line: Page size (number of words per page)
Second line: Main memory size (the maximum number of pages the main memory can hold)
Program instructions: Every NEW command will be followed by the size of the virtual memory for that program
Rest of the lines are virtual addresses to be executed in the same order as given in the input file.
Binary numbers representing virtual addresses to access.


samples_program100 folder is provided by the Instructor


Compilation:
g++ main.cpp -o main


Running:
./main


Example Output:
XXX.XXX% XX XX XX XX XX XX XX XX
         XX XX XX XX XX XX XX XX XX XX