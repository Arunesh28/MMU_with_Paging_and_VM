#include <string>
#include <sstream>
using namespace std;
// int nwords(string str)
// {
//     stringstream sin(str);
//     string word;

//     int count = 0;
//     while (sin >> word)
//         count++;
//     return count;
// }

int get_file_size(string filename)
{
    fstream fd;
    fd.open(filename,ios::in);
    string sz;
    getline(fd,sz);
    fd.close();
    return stoi(sz);
}

int get_arg(stringstream &s)
{
    string arg_str;
    s >> arg_str;
    int arg = stoi(arg_str);
    return arg;
}