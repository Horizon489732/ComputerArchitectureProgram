README file is written by ChatGPT

CPU Cache Simulator

This program simulates a simple CPU cache system. It reads hexadecimal memory addresses, tracks cache hits and misses, and supports cache operations such as CLEAR and DEL.

Compilation:
g++ -std=c++17 *.cpp -o main

Running:
./main

Example Output:
Instruction# - 100
H - 0.940
138453056 138453057 138453058 --------- --------- 138453053 138453054 138453055

Instruction# - 200
H - 0.945
138453056 138453057 138453058 138453059 44741092 44741093 44741094 138453055

Notes:
Instruction# shows how many instructions have been executed.
H shows the cache hit ratio.
--------- indicates an empty cache block.