#include <iostream>
#include "Abstract_syntax_tree.hpp"
#include "evaluation.hpp"
#include "symbol.hpp"

using namespace std;

extern int yyparse();
extern Node* root;

int main() {
   int r= yyparse();
    evaluate(root);
   if(r==0){
   cout << "\n--- Symbol Table ---\n";
   
   for (auto &entry : symbol_table) {
        cout << entry.first << " = ";

        if (entry.second.check) {
            cout << "\"" << entry.second.s_val << "\"";
        } else {
            cout << entry.second.i_val;
        }

        cout << endl;
    }
     
    cout<<"\n------TREE---\n";
  printtree( root);
}
    return 0;
}
