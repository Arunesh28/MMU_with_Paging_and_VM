#include <bits/stdc++.h>
#include <fstream>
#include "helper.h"
#include "cmd.h"
using namespace std;

class pagetable_entry
{
public:
    int page_frame_no;
    int valid_bit;
    int present_bit;
    // int protection_bit;
    // int dirty_bit;
};
class PageTable
{
public:
    int no_pages;
    map<int, pagetable_entry> pagetable;
    PageTable(int no_pages)
    {
        this->no_pages = no_pages;
    }
    void add_pte(int page_no,int page_frame,int present)
    {
        pagetable[page_no].page_frame_no = page_frame;
        pagetable[page_no].valid_bit = 1;
        pagetable[page_no].present_bit = present;
    }
};

int MAIN_MEMORY_SIZE;
int VIRTUAL_MEMORY_SIZE;
int PAGE_SIZE;
int NO_PAGE_FRAMES;
int NO_PAGES_VM;
vector<vector<int>> MAIN_MEMORY;
vector<vector<int>> VIRTUAL_MEMORY;
vector<int> MM_TO_PID;
vector<int> VM_TO_PID;
map<int, string> PID_TO_FILE;
// list of free page frames in MM
vector<int> FREE_LIST;
vector<int> VM_FREE_LIST;
map<int, PageTable> PID_PAGETABLE;
int PID = 0;

void init()
{
    NO_PAGE_FRAMES = (MAIN_MEMORY_SIZE * 1024) / PAGE_SIZE;
    NO_PAGES_VM = (VIRTUAL_MEMORY_SIZE * 1024) / PAGE_SIZE;
    MAIN_MEMORY = vector<vector<int>>(NO_PAGE_FRAMES, vector<int>(PAGE_SIZE, 0));
    VIRTUAL_MEMORY = vector<vector<int>>(NO_PAGES_VM, vector<int>(PAGE_SIZE, 0));
    MM_TO_PID = vector<int>(NO_PAGE_FRAMES, 0);
    VM_TO_PID = vector<int>(NO_PAGES_VM, 0);
    for (int i = 0; i < MAIN_MEMORY.size(); i++)
        FREE_LIST.push_back(i);
    for (int i = 0; i < VIRTUAL_MEMORY.size(); i++)
        VM_FREE_LIST.push_back(i);
}

int check_no_free_pages(int no_pages)
{
    int count1 = 0;
    for (int i = 0; i < MM_TO_PID.size(); i++)
        if (MM_TO_PID[i] == 0)
            count1++;
    if (no_pages <= count1)
        return 1;

    int count2 = 0;
    for (int i = 0; i < VM_TO_PID.size(); i++)
        if (VM_TO_PID[i] == 0)
            count2++;
    if (no_pages <= count2)
        return 2;

    return 0;
}

// void memory_access()

void load(stringstream &s)
{
    vector<string> files;
    string filename;
    while (s >> filename)
    {
        files.push_back(filename);
    }
    for (int i = 0; i < files.size(); i++)
    {
        // check if the file exists
        ifstream file;
        file.open(files[i]);
        if (!file)
        {
            cout << files[i] << " could not be loaded - file does not exist" << endl;
            continue;
        }
        file.close();

        // get the number of pages to be allocated to the current exe
        int filesize = get_file_size(files[i]);
        int no_pages = filesize * 1024 / PAGE_SIZE;

        // check if the MM or VM can fit the file
        switch (check_no_free_pages(no_pages))
        {
        case 1:
        {
            // add all the page into MM
            PageTable newpt(no_pages);
            for (int i = 0; i < no_pages; i++)
            {
                int index = FREE_LIST[0];
                FREE_LIST.erase(FREE_LIST.begin());
                MM_TO_PID[index] = PID;
                newpt.add_pte(i,index,1);
            }
            cout << files[i] << " is loaded in main memory and is assigned process id " << PID << endl;
            PID_TO_FILE[PID] = files[i];
            PID_PAGETABLE[PID] = newpt;
            PID++;

            break;
        }
        case 2:
        {
            // add all pages into VM
            PageTable newpt(no_pages);
            for (int i = 0; i < no_pages; i++)
            {
                int index = VM_FREE_LIST[0];
                VM_FREE_LIST.erase(VM_FREE_LIST.begin());
                VM_TO_PID[index] = PID;
                newpt.add_pte(i,index,0);
            }
            cout << files[i] << " is loaded in virtual memory and is assigned process id " << PID << endl;
            PID_TO_FILE[PID] = files[i];
            PID_PAGETABLE[PID] = newpt;
            PID++;

            break;
        }
        default:
        {
            cout << files[i] << " could not be loaded - memory is full" << endl;
            continue;
        }
        }
    }
}

void run(int pid)
{
    // open the file corresponding to the process id
    fstream fd;
    fd.open(PID_TO_FILE[pid]);
    string line;
    // Now start reading the file/exe line by line
    getline(fd, line);
    while (getline(fd, line))
    {
        stringstream s(line);
        string opcode;
        s >> opcode;
        switch (OPCODE_NUMBER[opcode])
        {
        case 1:
        {
            // add
            int x, y, z;
            s >> x >> y >> z;

            break;
        }
        case 2:
        {
            // sub
            int x, y, z;
            s >> x >> y >> z;
            break;
        }
        case 3:
        {
            // load
            int a, y;
            s >> a >> y;
            break;
        }
        case 4:
        {
            // print
            int x;
            s >> x;
            break;
        }
        default:
        {
            cout << "Invalid opcode in file " << PID_TO_FILE[pid] << endl;
            break;
        }
        }
    }
}

void kill(int pid)
{
    string filename = PID_TO_FILE[pid];
    if (filename == "")
    {
        cout << "Invalid pid !" << endl;
        return;
    }
}

void listpr()
{
    vector<int> MM_processes;
    for (int i = 0; i < MM_TO_PID.size(); i++)
    {
        if (MM_TO_PID[i] != 0)
        {
            MM_processes.push_back(MM_TO_PID[i]);
        }
    }
    sort(MM_TO_PID.begin(), MM_TO_PID.end());
    cout << "processes in main memory are : ";
    for (auto i : MM_processes)
        cout << i << " ";
    cout << endl;

    vector<int> VM_processes;
    for (int i = 0; i < VM_TO_PID.size(); i++)
    {
        if (VM_TO_PID[i] != 0)
        {
            VM_processes.push_back(VM_TO_PID[i]);
        }
    }
    sort(VM_TO_PID.begin(), VM_TO_PID.end());
    cout << "processes in virtual memory are : ";
    for (auto i : VM_processes)
        cout << i << " ";
    cout << endl;
}

void pte(int pid, string filename)
{
    // Print all the page table entries to the specified output file in ascending pid
    // order (starting at pid 1).
    // If the file already exists, append the output to the file. Include the date/time
    // at the start of output data each time this command is invoked.
}

void pteall(string filename)
{
}

void swapout(string pid)
{
}

void swapin(string pid)
{
}

void print(int memloc, int length)
{
}

void exit()
{
}

int main(int argc, char *argv[])
{
    string infile, outfile;
    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        char *val = argv[++i];
        switch (arg[1])
        {
        case 'M':
            MAIN_MEMORY_SIZE = atoi(val);
            break;
        case 'V':
            VIRTUAL_MEMORY_SIZE = atoi(val);
            break;
        case 'P':
            PAGE_SIZE = atoi(val);
            break;
        case 'i':
            infile = val;
            break;
        case 'o':
            outfile = val;
            break;
        }
    }
    init();
    fstream fd;
    fd.open(infile, ios::in);
    string instr;
    while (getline(fd, instr))
    {
        stringstream s(instr);
        string opcode;
        s >> opcode;
        switch (COMMAND_NUMBER[opcode])
        {
        case 1:
        {
            load(s);
            break;
        }
        case 2:
        {
            int pid = get_arg(s);
            break;
        }
        case 3:
        {
            int pid = get_arg(s);
            break;
        }
        case 4:
        {
            break;
        }
        case 5:
        {
            int pid = get_arg(s);
            break;
        }
        case 6:
        {
            string filename;
            s >> filename;
            break;
        }
        case 7:
        {
            int pid = get_arg(s);
            break;
        }
        case 8:
        {
            int pid = get_arg(s);
            break;
        }
        case 9:
        {
            int memloc, length;
            memloc = get_arg(s);
            length = get_arg(s);
            break;
        }
        case 10:
        {
            break;
        }
        default:
        {
            cout << "Invalid command " << opcode << endl;
            exit(0);
        }
        }
    }

    return 0;
}