#include <map>
#include <string>
using namespace std;
class Process
{
public:
    string filename;
    int size;
    int pid;
    int present;
    int lru_bit;
    map<int, int> pagetable;
    void set(string fname, int sz, int p, map<int, int> pt, int present_bit)
    {
        filename = fname;
        size = sz;
        pid = p;
        pagetable = pt;
        present = present_bit;
        lru_bit = 0;
    }
    void add_pte(int page_no, int page_frame)
    {
        pagetable[page_no] = page_frame;
    }
};