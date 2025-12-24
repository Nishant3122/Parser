
%{
#include<stdio.h>
#include<stdlib.h>
#include "Abstract_syntax_tree.hpp"
#include<string.h>
Node* root;
void yyerror(const char* s);
int yylex(void);
%}
%code requires {
    #include "Abstract_syntax_tree.hpp"
}
/*Decleration*/
%token VAR
%token IF
%token WHILE
%token ELSE
%token<sval> IDENTIFIER
%token<ival> INTEGER
%token EQ NEQ LT GT LE GE ASSIGN SEMI
%token LPAREN RPAREN LBRACES RBRACES
%token PLUS MINUS MUL DIV
%token INC DEC
%token OR AND NOT
%token TRUE FALSE
%union{
    int ival;
    char* sval;
    Node* node;
}
/*%type <ival> expression*/
%token <sval> STRING
%type<node> Logical_and Logical_OR equality comparision if_statement while_statement
%type<node>  factor term expression Uniary smaller
%type<node> variable_declaration assign_statement block increment
%type<node> program statement_list statement
%%
    program:
        statement_list
        {
            root=$1;
        }
    ;
    statement_list:
        statement_list statement
        {
            $$=make_IntermediateNode("STM_List",$1,$2);
        }
        |statement
        {
            $$=$1;
        }
    ;   
    statement:
    variable_declaration
    |if_statement
    |while_statement
    |block
    |assign_statement
    |increment
    | expression SEMI
    ;
    variable_declaration:
    VAR IDENTIFIER ASSIGN expression SEMI
     {
        Node* id=Cha_Node($2);
        Node* assi=make_IntermediateNode("=",id,$4);
        $$=make_IntermediateNode("variable_declaration",assi,nullptr);
     }
    | VAR IDENTIFIER SEMI
     {
        Node* id=Cha_Node($2);
        $$=make_IntermediateNode("variable_declaration",id,nullptr);
     }
    ;
    if_statement:
    IF LPAREN expression RPAREN statement
    {
        $$=make_IntermediateNode("IF",$3,$5);
    }
    | IF LPAREN expression RPAREN statement ELSE statement
    {
        Node* B=make_IntermediateNode("BRANCH",$5,$7);
        $$=make_IntermediateNode("IF",$3,B);
    }
    ;
    while_statement:
    WHILE LPAREN expression RPAREN statement
    {
        $$=make_IntermediateNode("WHILE",$3,$5);
    }
    ;
    assign_statement:
    IDENTIFIER ASSIGN expression SEMI
    {
        $$=make_IntermediateNode("=",Cha_Node($1),$3);
    }
    |IDENTIFIER ASSIGN IDENTIFIER SEMI
    {
        $$=make_IntermediateNode("=",Cha_Node($1),Cha_Node($3));
    }
    ;
    block:
    LBRACES statement_list RBRACES
    {
        $$=make_IntermediateNode("BLOCK",$2,nullptr);
    }
    ;
    increment:
    INC IDENTIFIER SEMI
    {
        $$=make_IntermediateNode("PRE_INC",Cha_Node($2),nullptr);
    }
   
    |DEC IDENTIFIER SEMI
    {
         $$=make_IntermediateNode("PRE_DEC",Cha_Node($2),nullptr);
    }
    |IDENTIFIER INC SEMI
    {
         $$=make_IntermediateNode("POST_INC",Cha_Node($1),nullptr);
    }
    |IDENTIFIER DEC SEMI
    {
         $$=make_IntermediateNode("POST_DEC",Cha_Node($1),nullptr);
    }
    ;
    expression:
    Logical_OR
    {
        $$=$1;
    }
    ;
    Logical_OR:
    Logical_and
    {
        $$=$1;
    }
    |Logical_OR OR Logical_and
    {
        $$=make_IntermediateNode("OR",$1,$3);
    }
    ;
    Logical_and:
    equality
    {
        $$=$1;
    }
    |Logical_and AND equality
    {
        $$=make_IntermediateNode("AND",$1,$3);
    }
    ;
    equality:
    comparision
    {
        $$=$1;
    }
    |equality EQ comparision
    {
        $$=make_IntermediateNode("EQ",$1,$3);
    }
    |equality NEQ comparision
    {
        $$=make_IntermediateNode("NEQ",$1,$3);
    }
    ;
    comparision:
    term
    {
        $$=$1;
    }
    |comparision LT term
    {
        $$=make_IntermediateNode("LT",$1,$3);
    }
    |comparision GT term
    {
        $$=make_IntermediateNode("GT",$1,$3);
    }
    |comparision GE term
    {
        $$=make_IntermediateNode("GE",$1,$3);
    }
    |comparision LE term
    {
        $$=make_IntermediateNode("LE",$1,$3);
    }
    ;
    term:
    factor
    {
        $$=$1;
    }
    |term PLUS factor
    {
        $$=make_IntermediateNode("PLUS",$1,$3);
    }
    |term MINUS factor
    {
        $$=make_IntermediateNode("MINUS",$1,$3);
    }
    ;
    factor:
    Uniary
    {
        $$=$1;
    }
    |factor MUL Uniary
    {
         $$=make_IntermediateNode("*",$1,$3); 
    }
    |factor DIV Uniary
    {
        $$=make_IntermediateNode("/",$1,$3);
    }
    ;
    Uniary:
    INC IDENTIFIER
    {
        $$=make_IntermediateNode("PRE_INC",Cha_Node($2),nullptr);
    }
    |DEC IDENTIFIER
    {
        $$=make_IntermediateNode("PRE_DEC",Cha_Node($2),nullptr);
    }
    |IDENTIFIER INC 
    {
         $$=make_IntermediateNode("POST_INC",Cha_Node($1),nullptr);
    }
    |IDENTIFIER DEC 
    {
         $$=make_IntermediateNode("POST_DEC",Cha_Node($1),nullptr);
    }

    |MINUS smaller
     {

        $$=make_IntermediateNode("NEG",$2,nullptr);
     }
    |PLUS smaller
    {
        $$=make_IntermediateNode("POS",$2,nullptr);
    }
    |smaller
    { 
        $$=$1;
    }
    |NOT Uniary
    {
        $$=make_IntermediateNode("NOT",$2,nullptr);
    }
    ;
    smaller:
    
    INTEGER
    {
       $$= INT_Node($1);
    }
    |STRING
     {
        $$=String_Node($1);
     }
    |IDENTIFIER
    {
        $$=Cha_Node($1);
    }
    |LPAREN expression RPAREN
    {
        $$=$2;
    }
    ;

    %%
    void yyerror(const char *s){
     
        extern int yylineno;
           printf("Error in parsing %d\n",yylineno);
    }





    

