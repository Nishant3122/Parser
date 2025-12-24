#ifndef symbol_HPP
#define symbol_HPP
#include<string>
#include<unordered_map>
using namespace std;
struct VALUE{
    bool check;
    int i_val;
    string s_val;
};
extern unordered_map<string,VALUE> symbol_table;

#endif
