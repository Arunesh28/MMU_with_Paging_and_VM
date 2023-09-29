#include<map>
using namespace std;

map<string,int> COMMAND_NUMBER = {{"load",    1},
                                  {"run",     2},
                                  {"kill",    3},
                                  {"listpr",  4},
                                  {"pte",     5},
                                  {"pteall",  6},
                                  {"swapout", 7},
                                  {"swapin",  8},
                                  {"print",   9},
                                  {"exit",    10}};

map<string,int> OPCODE_NUMBER = {{"add",   1},
                                 {"sub",   2},
                                 {"load",  3},
                                 {"print", 4}};