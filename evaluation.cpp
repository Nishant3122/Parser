
#include "evaluation.hpp"
#include "symbol.hpp"
int evaluate(Node* root){
    if(!root)
     return 0;
     if(root->type=="STM_List"){
        evaluate(root->left);
        evaluate(root->right);
        return 0;
    }
    else if(root->type=="variable_declaration"){
          
        if(root->left&&root->left->type=="="){
            if (symbol_table.find(root->left->left->s_val) != symbol_table.end()) {
        cout<<"Variable Redeclered"<<endl;
        exit(1);
        }    VALUE U;   
        U.check=false;
        U.i_val=0;
        U.s_val="";
           if(root->left->right->type=="STRING"){
                U.check=true;
                U.s_val=root->left->right->s_val;
                U.i_val=0;
           }
            else{
                int v=evaluate(root->left->right);
                U.i_val=v;
                U.s_val="";
            }
            symbol_table[root->left->left->s_val]=U;
              return 0;
        }
        
        if(root->left&&root->left->type=="IDENTIFIER"){
               if (symbol_table.find(root->left->s_val) != symbol_table.end()) {
        cout<<"Variable Redeclered"<<endl;
        exit(1);
        }  
        VALUE U; 
        U.check=false;
        U.i_val=0;
        U.s_val="";
        symbol_table[root->left->s_val]=U;
            return 0;
        }
        return 0;
    }
    else if(root->type=="="){
        int val=evaluate(root->right);
            if (symbol_table.find(root->left->s_val) == symbol_table.end()) {
        cout<<"Variable Undeclered"<<endl;
        exit(1);
    }    VALUE &U=symbol_table[root->left->s_val];
         if(root->right->type=="STRING"){
            U.check=true;
            U.i_val=0;
            U.s_val=root->right->s_val;
         }
         else{
            U.check=false;
            U.s_val="";
            U.i_val=val;
         }
         symbol_table[root->left->s_val]=U;
         return val;
    }
      
    else if(root->type=="PRE_INC"){
       
       
         if (symbol_table.find(root->left->s_val) == symbol_table.end()) {
        cout<<"Variable Undeclered"<<endl;
        exit(1);
    }
        VALUE &val=symbol_table[root->left->s_val];
         if(val.check){
            cout<<"INcrement Not happen on string";
            exit(1);
         }
         val.i_val=val.i_val+1;
        return val.i_val;
    }
   else if(root->type=="PRE_DEC"){
       
         if (symbol_table.find(root->left->s_val) == symbol_table.end()) {
        cout<<"Variable Undeclered"<<endl;
        exit(1);
    }
     VALUE &val=symbol_table[root->left->s_val];
         if(val.check){
             cout<<"Decrement Not happen on string";
            exit(1);
         }
       val.i_val-=1;
        
        return val.i_val;
    }
   else if(root->type=="POST_DEC"){
        
         if (symbol_table.find(root->left->s_val) == symbol_table.end()) {
        cout<<"Variable Undeclered"<<endl;
        exit(1);
    }   VALUE &val=symbol_table[root->left->s_val];
     if(val.check){
          cout<<"Decrement Not happen on string";
            exit(1);
     }
        int new_val=val.i_val;
          val.i_val-=1;
        //symbol_table[root->left->s_val]=new_val;
        return new_val;
    }
  else  if(root->type=="POST_INC"){
        
         if (symbol_table.find(root->left->s_val) == symbol_table.end()) {
        cout<<"Variable Undeclered"<<endl;
        exit(1);
    }
    VALUE &val=symbol_table[root->left->s_val];
        int new_val=val.i_val;
    val.i_val+=1;
      if(val.check){
          cout<<"Increment Not happen on string";
            exit(1);
     }
       // symbol_table[root->left->s_val]=new_val;
        return new_val;
    }

   else if(root->type=="IDENTIFIER"){
        if (symbol_table.find(root->s_val) == symbol_table.end()) {
        cout<<"Variable Undeclered"<<endl;
        exit(1);
    } VALUE U=symbol_table[root->s_val];
        if(U.check)
        {
            cout<<"Eroor"<<endl;
            exit(1);
        }
        return U.i_val;
    }
     else if(root->type=="INTEGER"){
          return root->i_val;
    }
    
    else if(root->type=="PLUS"){
        return evaluate(root->left)+evaluate(root->right);
    }
    else if(root->type=="MINUS"){
        return evaluate(root->left)-evaluate(root->right);
    }
     else if(root->type=="/"){
         return evaluate(root->left)/evaluate(root->right);
    }
     else if(root->type=="*"){
        return evaluate(root->left)*evaluate(root->right);
    }
    else if(root->type=="NEG"){
        return (-evaluate(root->left));
    }
    else if(root->type=="POS"){
        return (evaluate(root->left));
    }
     else if(root->type=="BLOCK"){
        evaluate(root->left);
       return 0;
    }
    else if(root->type=="IF"){
        int val=evaluate(root->left);
        if(val!=0){
            if(root->right->type=="BRANCH"){
                evaluate(root->right->left);
            }
            else{
                evaluate(root->right);
            }
        }
        else{
            if(root->right->type=="BRANCH"){
                evaluate(root->right->right);
            }
        }
        return 0;
    }
    else if(root->type=="WHILE"){
      
         while(evaluate(root->left)!=0){
            evaluate(root->right);
         }
         return 0;
    }
    
    else if(root->type=="OR"){
        int v=evaluate(root->left);
        if(v!=0){
            return 1;
        }
        int v2=evaluate(root->right);
        if(v2!=0){
            return 1;
        }
       return 0;
    }

    else if(root->type=="AND"){
        int v=evaluate(root->left);
        if(v==0){
            return 0;
        }
        int v2=evaluate(root->right);
        if(v2==0){
            return 0;
        }
       return 1;
    }
    else if(root->type=="EQ"){
        Node* left=root->left;
        Node* right=root->right;
        if(left->type=="INTEGER"&&right->type=="INTEGER"){
            return left->i_val == right->i_val;
        }
        if(left->type=="STRING"&& right->type=="STRING"){
            return left->s_val==right->s_val;
        }
        if(left->type=="IDENTIFIER"&&right->type=="INTEGER")
        {
            VALUE &v=symbol_table[left->s_val];
            if(v.check){
                cout<<"Error in equality"<<endl;
                exit(1);
            }
            return v.i_val==right->i_val;
        }
        if(left->type=="IDENTIFIER"&&right->type=="IDENTIFIER"){
               VALUE &u1=symbol_table[left->s_val];
               VALUE &u2=symbol_table[right->s_val];
                  if(u1.check !=u2.check){
                    cout<<"Type mismatch";
                    exit(1);
                  }

            return u1.i_val==u2.i_val;
        }
        if(left->type=="INTEGER"&&right->type=="IDENTIFIER"){
            VALUE &U=symbol_table[right->s_val];
            if(U.check){
                cout<<"Error in equality"<<endl;
                exit(1);
            }
            return left->i_val==U.i_val;
        }
        return 0;
    }
   else if(root->type=="NEQ"){
        int eq=0;
         Node* left=root->left;
        Node* right=root->right;
        if(left->type=="INTEGER"&&right->type=="INTEGER"){
            eq=( left->i_val == right->i_val);
        }
        else if(left->type=="STRING"&& right->type=="STRING"){
            eq=( left->s_val==right->s_val);
        }
       else  if(left->type=="IDENTIFIER"&&right->type=="INTEGER")
        {
            VALUE &v=symbol_table[left->s_val];
            if(v.check){
                cout<<"Error in equality"<<endl;
                exit(1);
            }
            eq= v.i_val==right->i_val;
        }
        else if(left->type=="IDENTIFIER"&&right->type=="IDENTIFIER")
            {
                 VALUE &u1=symbol_table[left->s_val];
               VALUE &u2=symbol_table[right->s_val];
                  if(u1.check !=u2.check){
                    cout<<"Type mismatch";
                    exit(1);
                  }

            eq= u1.i_val==u2.i_val;
            }
       else if(left->type=="INTEGER"&&right->type=="IDENTIFIER"){
            VALUE &U=symbol_table[right->s_val];
            if(U.check){
                cout<<"Error in equality"<<endl;
                exit(1);
            }
            eq= left->i_val==U.i_val;
        }
        else{
            return 1;
        }
        return !eq;
    }
   else if(root->type=="LT"){
       Node* left=root->left;
       Node* right=root->right;
        if(left->type=="INTEGER"&&right->type=="INTEGER"){
            return left->i_val < right->i_val;
        }
        else  if(left->type=="IDENTIFIER"&&right->type=="INTEGER")
        {
            VALUE &v=symbol_table[left->s_val];
            if(v.check){
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            }
               return v.i_val < right->i_val;
        }
        else if(left->type=="IDENTIFIER"&&right->type=="IDENTIFIER")
        {   VALUE u1=symbol_table[left->s_val];
            VALUE u2=symbol_table[right->s_val];
            if(u1.check || u2.check){
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            }
            return u1.i_val < u2.i_val;
        }
        else if(left->type=="STRING"||right->type=="STRING")
        {   
            
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            
            
        }
        else if(left->type=="INTEGER"&&right->type=="IDENTIFIER"){
            VALUE &val=symbol_table[right->s_val];
            if(val.check){
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            }
            return left->i_val < val.i_val;
        }
        return 0;

    }
   else  if(root->type=="GT"){
       Node* left=root->left;
       Node* right=root->right;
        if(left->type=="INTEGER"&&right->type=="INTEGER"){
            return left->i_val > right->i_val;
        }
        else  if(left->type=="IDENTIFIER"&&right->type=="INTEGER")
        {
            VALUE &v=symbol_table[left->s_val];
            if(v.check){
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            }
               return v.i_val > right->i_val;
        }
        else if(left->type=="IDENTIFIER"&&right->type=="IDENTIFIER")
        {   VALUE u1=symbol_table[left->s_val];
            VALUE u2=symbol_table[right->s_val];
            if(u1.check || u2.check){
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            }
            return u1.i_val > u2.i_val;
        }
        else if(left->type=="STRING"||right->type=="STRING")
        {   
            
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            
            
        }
        else if(left->type=="INTEGER"&&right->type=="IDENTIFIER"){
            VALUE &val=symbol_table[right->s_val];
            if(val.check){
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            }
            return left->i_val > val.i_val;
        }
        return 0;

    }
     else if(root->type=="LE"){
        Node* left=root->left;
       Node* right=root->right;
        if(left->type=="INTEGER"&&right->type=="INTEGER"){
            return left->i_val <= right->i_val;
        }
        else  if(left->type=="IDENTIFIER"&&right->type=="INTEGER")
        {
            VALUE &v=symbol_table[left->s_val];
            if(v.check){
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            }
               return v.i_val <= right->i_val;
        }
        else if(left->type=="IDENTIFIER"&&right->type=="IDENTIFIER")
        {   VALUE u1=symbol_table[left->s_val];
            VALUE u2=symbol_table[right->s_val];
            if(u1.check || u2.check){
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            }
            return u1.i_val <= u2.i_val;
        }
        else if(left->type=="STRING"||right->type=="STRING")
        {   
            
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            
            
        }
        else if(left->type=="INTEGER"&&right->type=="IDENTIFIER"){
            VALUE &val=symbol_table[right->s_val];
            if(val.check){
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            }
            return left->i_val <= val.i_val;
        }
        return 0;

    }
    
     else if(root->type=="GE"){
         Node* left=root->left;
       Node* right=root->right;
        if(left->type=="INTEGER"&&right->type=="INTEGER"){
            return left->i_val >= right->i_val;
        }
        else  if(left->type=="IDENTIFIER"&&right->type=="INTEGER")
        {
            VALUE &v=symbol_table[left->s_val];
            if(v.check){
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            }
               return v.i_val >= right->i_val;
        }
        else if(left->type=="IDENTIFIER"&&right->type=="IDENTIFIER")
        {   VALUE u1=symbol_table[left->s_val];
            VALUE u2=symbol_table[right->s_val];
            if(u1.check || u2.check){
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            }
            return u1.i_val >= u2.i_val;
        }
        else if(left->type=="STRING"||right->type=="STRING")
        {   
            
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            
            
        }
        else if(left->type=="INTEGER"&&right->type=="IDENTIFIER"){
            VALUE &val=symbol_table[right->s_val];
            if(val.check){
                cout<<"Comparing Not possible"<<endl;
                exit(1);
            }
            return left->i_val >= val.i_val;
        }

        return 0;
     }
   
     else if(root->type=="NOT"){
        return !(evaluate(root->left));
     }
    
    
    
    
    return 0;
}