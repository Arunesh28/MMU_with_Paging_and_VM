#include <bits/stdc++.h>
#include "helper.h"
using namespace std;
int main()
{
    // string str = "ca 123 cccccccccccccccccccc c     c";
    // stringstream s(str);
    // string x;
    // s>>x;
    // cout<<x<<endl;
    // cout<<get_pid(s)<<endl;
    // string s = "file3";
    // cout<<get_file_size(s)<<endl;
    // map<int,string> mp;
    // if(mp[23] == "")
    // cout<<"yes"<<endl;
    // else
    // cout<<"no"<<endl;
    // vector<>
    // int x = 3000;
    // cout<<(x>>10)<<endl;
    // int y = 1024;
    // cout<<log2(y)<<endl;
    int x = 3000;
    int bit_mask = (1<<10) - 1;
    // cout<<bit_mask<<endl;
    cout<<(bit_mask&x)<<endl;
    return 0;
}