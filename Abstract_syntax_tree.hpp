#ifndef Abstract_syntax_tree_HPP
#define Abstract_syntax_tree_HPP
#include<iostream>
#include<string>
using namespace std;
class Node{
    public:
    string type;
    string s_val;
    int i_val;
    Node *left;
    Node *right;
    Node(string s,Node *l=nullptr,Node *r=nullptr)
    : type(s),
    left(l),
    right(r) ,s_val(""),i_val(0) {}
};
Node* make_IntermediateNode(const string &s,Node* l,Node*r);
Node* INT_Node(int val);
Node* Cha_Node(const string &s);
Node* String_Node(const string &s);
void printtree(Node* root);
#endif