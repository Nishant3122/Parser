#include<iostream>
#include "Abstract_syntax_tree.hpp"
using namespace std;
Node* make_IntermediateNode(const string& s,Node *left,Node *right){
    return new Node(s,left,right);
}
Node* INT_Node(int val){
    Node *temp=new Node("INTEGER");
    temp->i_val=val;
    return temp;

}
Node* Cha_Node(const string& s){
    Node *temp=new Node("IDENTIFIER");
    temp->s_val=s;
    return temp;
}
Node* String_Node(const string& s){
    Node *temp=new Node("STRING");
    temp->s_val=s;
    return temp;

}
void printtree(Node* root){
    if(!root)
    return;
     string s="";
    s="<--"+root->type;
    if(root->type=="INTEGER"){
        s=s+"("+to_string(root->i_val)+")";
    }
    else if(root->type=="STRING"){
        s=s+"("+(root->s_val)+")";
    }
    else if(root->type=="IDENTIFIER"){
        s=s+"("+(root->s_val)+")";
    }
   
        s=s+"-->";
    
    if(root->left!=nullptr){
       string ans="";
        ans=ans+root->left->type;
        if(root->left->type=="INTEGER"){
        ans=ans+"("+to_string(root->left->i_val)+")";
       }
       if(root->left->type=="STRING"){
        ans=ans+"("+(root->left->s_val)+")";
    }
    if(root->left->type=="IDENTIFIER"){
      ans=ans+"("+(root->left->s_val)+")";
    }
    s=ans+s;
    }
    else{
      s="(.)"+s;
    }
    if(root->right!=nullptr){
        string ans="";
        ans=ans+root->right->type;
        if(root->right->type=="INTEGER"){
          ans=ans+"("+to_string(root->right->i_val)+")";
        }
        else if(root->right->type=="STRING"){
            ans=ans+"("+(root->right->s_val)+")";
        }
        else if(root->right->type=="IDENTIFIER"){
            ans=ans+"("+(root->right->s_val)+")";
        }
       s=s+ans;
    }
    else{
        s=s+"(.)";
    }
    cout<<s<<endl;
    printtree(root->left);
    printtree(root->right);
}
