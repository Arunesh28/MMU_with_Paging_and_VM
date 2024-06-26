#include <bits/stdc++.h>
#include <fstream>
#include <string.h>
#include "helper.h"
#include "cmd.h"
#include "Process.h"
using namespace std;

fstream out;
int MAIN_MEMORY_SIZE = 32;
int VIRTUAL_MEMORY_SIZE = 32;
int PAGE_SIZE = 512;
int NO_PAGE_FRAMES;
int NO_PAGES_VM;
vector<vector<int>> MAIN_MEMORY;
vector<vector<int>> VIRTUAL_MEMORY;
vector<int> MM_TO_PID;
vector<int> VM_TO_PID;
vector<int> MM_FREE_LIST;
vector<int> VM_FREE_LIST;
map<int, Process> PROCESSES;
int PID;

void print_vec(vector<int> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec[i] << " ";
    }
    cout << endl;
}
void printall()
{
    cout << "MM_TO_PID" << endl;
    print_vec(MM_TO_PID);
    cout << "VM_TO_PID" << endl;
    print_vec(VM_TO_PID);
    cout << "FREE_LIST" << endl;
    print_vec(MM_FREE_LIST);
    cout << "VM_TO_FREE_LIST" << endl;
    print_vec(VM_FREE_LIST);
    cout << "PID_TO_FILE" << endl;
    for (auto i : PROCESSES)
    {
        cout << i.first << " " << i.second.filename << "_";
    }
    cout << endl;
}

vector<pair<int, int>> process_lru_list()
{
    vector<pair<int, int>> vp;
    for (auto p : PROCESSES)
    {
        if (p.second.present == 1)
        {
            vp.push_back({p.second.lru_bit, p.first});
        }
    }
    sort(vp.begin(), vp.end());
    reverse(vp.begin(), vp.end());
    return vp;
}

void init()
{
    NO_PAGE_FRAMES = (MAIN_MEMORY_SIZE * 1024) / PAGE_SIZE;
    NO_PAGES_VM = (VIRTUAL_MEMORY_SIZE * 1024) / PAGE_SIZE;
    MAIN_MEMORY = vector<vector<int>>(NO_PAGE_FRAMES, vector<int>(PAGE_SIZE, 0));
    VIRTUAL_MEMORY = vector<vector<int>>(NO_PAGES_VM, vector<int>(PAGE_SIZE, 0));
    PID = 1;
    MM_TO_PID = vector<int>(NO_PAGE_FRAMES, 0);
    VM_TO_PID = vector<int>(NO_PAGES_VM, 0);
    for (int i = 0; i < MAIN_MEMORY.size(); i++)
        MM_FREE_LIST.push_back(i);
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

pair<int, int> address_translation(int pid, int virtual_addr)
{
    int no_offset_bits = log2(PAGE_SIZE);
    int page_no = virtual_addr >> no_offset_bits;
    int bitmask = (1 << no_offset_bits) - 1;
    int offset = virtual_addr & bitmask;
    int pageframe = PROCESSES[pid].pagetable[page_no];
    int physical_addr = (pageframe << no_offset_bits) | offset;
    return {pageframe, offset};
}

int memory_load(int pid, int virtual_address)
{
    pair<int, int> phy_addr = address_translation(pid, virtual_address);
    int pageframe = phy_addr.first;
    int offset = phy_addr.second;
    return MAIN_MEMORY[pageframe][offset];
}

void memory_store(int pid, int virtual_address, int value)
{
    pair<int, int> phy_addr = address_translation(pid, virtual_address);
    int pageframe = phy_addr.first;
    int offset = phy_addr.second;
    MAIN_MEMORY[pageframe][offset] = value;
}

int check_valid_addr(int pid,int addr)
{
    int psize = PROCESSES[pid].size*PAGE_SIZE;
    if(addr>=psize)
    {
        out<<"Invalid address "<<addr<<" !"<<endl;
        return 0;
    }
    return 1;
}

void print_page_processes()
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
    out << "processes in main memory are : ";
    for (auto i : MM_processes)
        out << i << " ";
    out << endl;

    vector<int> VM_processes;
    for (int i = 0; i < VM_TO_PID.size(); i++)
    {
        if (VM_TO_PID[i] != 0)
        {
            VM_processes.push_back(VM_TO_PID[i]);
        }
    }
    sort(VM_TO_PID.begin(), VM_TO_PID.end());
    out << "processes in virtual memory are : ";
    for (auto i : VM_processes)
        out << i << " ";
    out << endl;
}

void print_mm_vm_processes()
{
    vector<pair<int,int>> mm_processes;
    vector<pair<int,int>> vm_processes;
    for(auto p : PROCESSES)
    {
        if(p.second.present == 1)
        {
            mm_processes.push_back({p.second.size,p.second.pid});
        }
        else
        {
            vm_processes.push_back({p.second.size,p.second.pid});
        }
    }

    sort(mm_processes.begin(),mm_processes.end());
    out << "Main Memory Processes : ";
    for(auto p : mm_processes) out << p.second << " ";
    out<<endl;

    sort(vm_processes.begin(),vm_processes.end());
    out << "Virtual Memory Processes : ";
    for(auto p : vm_processes) out << p.second << " ";
    out<<endl;
}

void tim(){
    time_t now = time(0);
    tm* ltm = localtime(&now);
    if((5 + ltm->tm_hour+(30 + ltm->tm_min)/60)>24)
        out<<"Date : "<< 1+ltm->tm_mday<< "/"<< 1 + ltm->tm_mon<<"/"<< 1900 + ltm->tm_year << " ";
    else 
        out<<"Date : "<< ltm->tm_mday<< "/"<< 1 + ltm->tm_mon<<"/"<< 1900 + ltm->tm_year << " ";
    out<< "Time: " << (5 + ltm->tm_hour+(30 + ltm->tm_min)/60)%24<< ":";  
    out<< (30 + ltm->tm_min)%60<< ":";  
    out<< ltm->tm_sec << endl;
}

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
            out << files[i] << " could not be loaded - file does not exist" << endl;
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
            map<int, int> newpt;
            for (int i = 0; i < no_pages; i++)
            {
                int index = MM_FREE_LIST[0];
                MM_FREE_LIST.erase(MM_FREE_LIST.begin());
                MM_TO_PID[index] = PID;
                newpt[i] = index;
            }
            out << files[i] << " is loaded in main memory and is assigned process id " << PID << endl;
            Process p;
            p.set(files[i], no_pages, PID, newpt, 1);
            PROCESSES[PID] = p;
            PID++;
            break;
        }
        case 2:
        {
            // add all pages into VM
            map<int, int> newpt;
            for (int i = 0; i < no_pages; i++)
            {
                int index = VM_FREE_LIST[0];
                VM_FREE_LIST.erase(VM_FREE_LIST.begin());
                VM_TO_PID[index] = PID;
                newpt[i] = index;
            }
            out << files[i] << " is loaded in virtual memory and is assigned process id " << PID << endl;
            Process p;
            p.set(files[i], no_pages, PID, newpt, 0);
            PROCESSES[PID] = p;
            PID++;

            break;
        }
        default:
        {
            out << files[i] << " could not be loaded - memory is full" << endl;
            continue;
        }
        }
        // printall();
    }
}

void kill(int pid)
{
    string filename = PROCESSES[pid].filename;
    if (filename == "")
    {
        out << "Invalid pid !" << endl;
        return;
    }
    if (PROCESSES[pid].present == 1)
    {
        for (auto i : PROCESSES[pid].pagetable)
        {
            int mm_page_frame = i.second;
            MM_FREE_LIST.push_back(mm_page_frame);
            MM_TO_PID[mm_page_frame] = 0;
        }
    }
    else
    {
        for (auto i : PROCESSES[pid].pagetable)
        {
            int vm_page_frame = i.second;
            VM_FREE_LIST.push_back(vm_page_frame);
            VM_TO_PID[vm_page_frame] = 0;
        }
    }
    PROCESSES.erase(pid);
    out<<"Process "<<pid<<" killed \n";
}

void listpr()
{
    vector<pair<int,int>> all_processes;
    for(auto p : PROCESSES) all_processes.push_back({p.second.size,p.second.pid});
    sort(all_processes.begin(),all_processes.end());
    out << "Processes sorted size wise : ";
    for(auto p : all_processes) out << p.second << " ";
    out<<endl;
}

void pte(int pid, string filename)
{
    ofstream fout;
    fout.open(filename, ios::out | ios::app);
    // tim();
    fout<<"Process id : "<<pid<<endl;
    fout<<"|\tPage Number |\t Page Frame Number\t|\n";
    fout<<"|---------------|-----------------------|"<<endl;
    for (auto i : PROCESSES[pid].pagetable)
    {
        fout<<"|\t\t" << i.first << "\t\t|\t\t\t" << i.second <<"\t\t\t|"<< endl;
    }
    fout<<endl;
    fout.close();
}

void pteall(string filename)
{
    // Print all the page table entries to the specified output file in ascending pid
    // order (starting at pid 1).
    // If the file already exists, append the output to the file. Include the date/time
    // at the start of output data each time this command is invoked.
    tim();
    ofstream fout;
    fout.open(filename, ios::out | ios::app);
    for(auto p : PROCESSES)
    {
        fout<<"Process id : "<<p.first<<endl;
        fout<<"|\tPage Number |\t Page Frame Number\t|\n";
        fout<<"|---------------|-----------------------|"<<endl;
        for (auto i : p.second.pagetable)
        {
            fout<<"|\t\t" << i.first << "\t\t|\t\t\t" << i.second <<"\t\t\t|"<< endl;
        }
        fout<<endl;
    }
    fout.close();
}

void swapout(int pid)
{
    // Swap out the process specified by pid into virtual memory. No other action
    // needed. Print to screen corresponding messages.
    // If an invalid pid is specified, output an error message and continue to accept
    // the next command.
    string filename = PROCESSES[pid].filename;
    if (filename == "")
    {
        out << "Invalid pid !" << endl;
        return;
    }
    int vm_free_space = VM_FREE_LIST.size();
    if (vm_free_space < PROCESSES[pid].size)
    {
        out << "Invalid pid !" << endl;
        return;
    }

    // map<int,int> pt = PROCESSES[pid].pagetable;
    if (PROCESSES[pid].present == 0)
    {
        out << "process not in main memory !" << endl;
        return;
    }
    // pt.present = 0;
    PROCESSES[pid].present = 0;
    for (auto i : PROCESSES[pid].pagetable)
    {
        int vpn = i.first;
        int pfn = i.second;

        // get next free page in VM
        int free_vm_index = VM_FREE_LIST[0];
        VM_FREE_LIST.erase(VM_FREE_LIST.begin());

        // copy page to VM from MM
        VIRTUAL_MEMORY[free_vm_index] = MAIN_MEMORY[pfn];

        // free the MM page
        MM_FREE_LIST.push_back(pfn);
        MM_TO_PID[pfn] = 0;

        // add pte
        VM_TO_PID[free_vm_index] = pid;
        i.second = free_vm_index;
    }
}

int swapin(int pid)
{
    // Swap in the process specified by pid into main memory. Note that other
    // process(es) may have to be swapped out to make room. Print
    // corresponding messages to screen. When a process is swapped in (or out),
    // ALL the pages of that process will be swapped out (or in) at the same time.
    if (PROCESSES[pid].present == 1)
    {
        // process already in MM
        return 1;
    }
    vector<pair<int, int>> plist = process_lru_list();
    // cout<<"size "<<plist.size()<<endl;
    // for(auto i : plist) cout<<i.second<<" ";cout<<endl;
    int size_swapin = PROCESSES[pid].size;
    // cout<<"swapin size "<<size_swapin<<endl;
    int temp = MM_FREE_LIST.size();
    vector<int> swapout_list;
    int flag = 0;
    for (auto i : plist)
    {
        // cout<<"temp is "<<temp<<endl;
        // cout<<"size swapin is "<<size_swapin<<endl;
        if (temp >= size_swapin)
        {
            flag = 1;
            break;
        }
        temp += PROCESSES[i.second].size;
        swapout_list.push_back(i.second);
    }
    if (flag == 0)
    {
        out << "swapin error" << endl;
        return 0;
    }
    if (temp > size_swapin + VM_FREE_LIST.size())
    {
        out << "swapin error" << endl;
        return 0;
    }
    // swap processes in swapoutlist and swapin process

    // vector of pairs of (pid,pair of (page number,pages to be swapped out))
    vector<pair<int, pair<int, vector<int>>>> list_swapout_pages;
    for (int proc : swapout_list)
    {
        for (auto pte : PROCESSES[proc].pagetable)
        {
            int mm_page_frame = pte.second;
            list_swapout_pages.push_back({proc, {pte.first, MAIN_MEMORY[mm_page_frame]}});
            // free page
            MM_FREE_LIST.push_back(mm_page_frame);
            MM_TO_PID[mm_page_frame] = 0;
        }
        PROCESSES[proc].present = 0;
    }

    // vector of pairs of pid,pages(to be swaped out)
    vector<pair<int, vector<int>>> list_swapin_pages;
    for (auto pte : PROCESSES[pid].pagetable)
    {
        int vm_page_frame = pte.second;
        list_swapin_pages.push_back({pte.first, VIRTUAL_MEMORY[vm_page_frame]});
        // free page
        VM_FREE_LIST.push_back(vm_page_frame);
        VM_TO_PID[vm_page_frame] = 0;
    }

    // swapout
    for (auto i : list_swapout_pages)
    {
        // get free page
        int free_vm_page = VM_FREE_LIST[0];
        VM_FREE_LIST.erase(VM_FREE_LIST.begin());
        // put the page into VM
        VIRTUAL_MEMORY[free_vm_page] = i.second.second;
        VM_TO_PID[free_vm_page] = i.first;
        // add entry to pagetable
        PROCESSES[i.first].pagetable[i.second.first] = free_vm_page;
    }

    // swapin
    for (auto i : list_swapin_pages)
    {
        // get free page
        int free_mm_page = MM_FREE_LIST[0];
        MM_FREE_LIST.erase(MM_FREE_LIST.begin());
        // put the page into MM
        MAIN_MEMORY[free_mm_page] = i.second;
        MM_TO_PID[free_mm_page] = pid;
        // add entry to page table
        PROCESSES[pid].pagetable[i.first] = free_mm_page;
    }

    return 1;
}

void run(int pid)
{
    // open the file corresponding to the process id
    fstream fd;
    string filename = PROCESSES[pid].filename;
    if (filename == "")
    {
        out << "Invalid pid !" << endl;
        return;
    }
    //  get the process into MM
    if (PROCESSES[pid].present == 0)
    {
        // if process not in MM then load the process into MM
        if (swapin(pid) == 0)
        {
            out << "run unsuccessfull !" << endl;
            return;
        }
    }

    fd.open(filename);
    string line;
    // Now start reading the file/exe line by line
    getline(fd, line);
    while (getline(fd, line))
    {
        stringstream s(line);
        string opcode;
        s >> opcode;
        // cout << "opcode=" << opcode << ",";
        switch (OPCODE_NUMBER[opcode])
        {
        case 1:
        {
            // add
            int x, y, z;
            sscanf(line.c_str(), "add %d, %d, %d", &x, &y, &z);
            // cout<<"x="<<x<<",y="<<y<<",z="<<z<<endl;
            if(check_valid_addr(pid,x)*check_valid_addr(pid,y)*check_valid_addr(pid,z) == 0)
            {
                // out<<"Invalid address !"<<endl;
                continue;
            }
            int xval = memory_load(pid, x);
            int yval = memory_load(pid, y);
            int zval = xval + yval;
            // cout<<"x"<<xval<<endl;
            // cout<<"y"<<yval<<endl;
            // cout<<"z"<<zval<<endl;
            memory_store(pid, z, zval);
            break;
        }
        case 2:
        {
            // sub
            int x, y, z;
            sscanf(line.c_str(), "sub %d, %d, %d", &x, &y, &z);
            // cout<<"x="<<x<<",y="<<y<<",z="<<z<<endl;
            if(check_valid_addr(pid,x)*check_valid_addr(pid,y)*check_valid_addr(pid,z) == 0)
            {
                // out<<"Invalid address !"<<endl;
                continue;
            }
            int xval = memory_load(pid, x);
            int yval = memory_load(pid, y);
            int zval = xval - yval;
            memory_store(pid, z, zval);
            break;
        }
        case 3:
        {
            // load
            int a, y;
            sscanf(line.c_str(), "load %d, %d", &a, &y);
            // cout<<"a="<<a<<",y="<<y<<endl;
            if(check_valid_addr(pid,y) == 0)
            {
                // out<<"Invalid address !"<<endl;
                continue;
            }
            memory_store(pid, y, a);
            break;
        }
        case 4:
        {
            // print
            int x;
            sscanf(line.c_str(), "print %d", &x);
            // cout<<"x="<<x<<endl;
            if(check_valid_addr(pid,x) == 0)
            {
                // out<<"Invalid address !"<<endl;
                continue;
            }
            int xval = memory_load(pid, x);
            out <<"Value at "<<x<<" is "<< xval << endl;
            break;
        }
        default:
        {
            out << "Invalid opcode in file " << PROCESSES[pid].filename << endl;
            break;
        }
        }
    }
}

void print_memloc(int memloc)
{
    int no_offset_bits = log2(PAGE_SIZE);
    int pageframe = memloc >> no_offset_bits;
    int bitmask = (1 << no_offset_bits) - 1;
    int offset = memloc & bitmask;
    int val = MAIN_MEMORY[pageframe][offset];
    out << "value at memloc " << memloc << " is " << val << endl;
}

void print(int memloc, int length)
{
    // Print the values stored in the physical memory locations from memloc
    // through memloc + length - 1
    for (int i = 0; i < length; i++)
    {
        print_memloc(memloc + i);
    }
}

void exit()
{
    // Exit the system and clean up all allocated memory.
    out<<"Exiting the system"<<endl;
    exit(0);
}

int main(int argc, char *argv[])
{
    string infile = "infile", outfile = "outfile";
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
    
    out.open(outfile,ios::app);
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
            run(pid);
            break;
        }
        case 3:
        {
            int pid = get_arg(s);
            kill(pid);
            break;
        }
        case 4:
        {
            listpr();
            break;
        }
        case 5:
        {
            int pid = get_arg(s);
            string filename;
            s >> filename;
            pte(pid, filename);
            break;
        }
        case 6:
        {
            string filename;
            s >> filename;
            pteall(filename);
            break;
        }
        case 7:
        {
            int pid = get_arg(s);
            swapout(pid);
            break;
        }
        case 8:
        {
            int pid = get_arg(s);
            int flag = swapin(pid);
            if (flag == 1)
                out << "swapin successful !" << endl;
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
            exit();
            break;
        }
        default:
        {
            out << "Invalid command " << opcode << endl;
            exit(0);
        }
        }
    }
    fd.close();
    out.close();
    return 0;
}