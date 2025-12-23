%{
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "ast.h"
#include "symbol_table.h"

extern int yylex();
extern int yylineno;
extern FILE* yyin;

void yyerror(const char* s);

ASTNode* root = nullptr;
SymbolTable symTable;
%}

%union {
    int num;
    std::string* str;
    ASTNode* node;
    StatementList* stmtList;
}

%token VAR IF ELSE WHILE
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON
%token ASSIGN
%token <num> INTEGER
%token <str> IDENTIFIER

%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left MULT DIV
%right UNARY

%type <node> program statement variable_decl assignment if_statement while_statement
%type <node> block expression equality comparison term factor unary primary
%type <stmtList> statement_list

%token EQ NE LT GT LE GE PLUS MINUS MULT DIV

%%

program:
    statement_list { 
        root = new ProgramNode($1); 
    }
    ;

statement_list:
    /* empty */ { 
        $$ = new StatementList(); 
    }
    | statement_list statement { 
        $1->push_back($2); 
        $$ = $1; 
    }
    ;

statement:
    variable_decl { $$ = $1; }
    | assignment { $$ = $1; }
    | if_statement { $$ = $1; }
    | while_statement { $$ = $1; }
    | block { $$ = $1; }
    ;

block:
    LBRACE statement_list RBRACE { 
        $$ = new BlockNode($2); 
    }
    ;

variable_decl:
    VAR IDENTIFIER SEMICOLON {
        if (symTable.isDeclared(*$2)) {
            fprintf(stderr, "Line %d: Variable '%s' already declared\n", 
                    yylineno, $2->c_str());
            delete $2;
            YYABORT;
        }
        symTable.declare(*$2);
        $$ = new VarDeclNode(*$2, nullptr);
        delete $2;
    }
    | VAR IDENTIFIER ASSIGN expression SEMICOLON {
        if (symTable.isDeclared(*$2)) {
            fprintf(stderr, "Line %d: Variable '%s' already declared\n", 
                    yylineno, $2->c_str());
            delete $2;
            YYABORT;
        }
        symTable.declare(*$2);
        $$ = new VarDeclNode(*$2, $4);
        delete $2;
    }
    ;

assignment:
    IDENTIFIER ASSIGN expression SEMICOLON {
        if (!symTable.isDeclared(*$1)) {
            fprintf(stderr, "Line %d: Variable '%s' not declared\n", 
                    yylineno, $1->c_str());
            delete $1;
            YYABORT;
        }
        $$ = new AssignmentNode(*$1, $3);
        delete $1;
    }
    ;

if_statement:
    IF LPAREN expression RPAREN statement {
        $$ = new IfNode($3, $5, nullptr);
    }
    | IF LPAREN expression RPAREN statement ELSE statement {
        $$ = new IfNode($3, $5, $7);
    }
    ;

while_statement:
    WHILE LPAREN expression RPAREN statement {
        $$ = new WhileNode($3, $5);
    }
    ;

expression:
    equality { $$ = $1; }
    ;

equality:
    comparison { $$ = $1; }
    | equality EQ comparison {
        $$ = new BinaryOpNode("==", $1, $3);
    }
    | equality NE comparison {
        $$ = new BinaryOpNode("!=", $1, $3);
    }
    ;

comparison:
    term { $$ = $1; }
    | comparison LT term {
        $$ = new BinaryOpNode("<", $1, $3);
    }
    | comparison GT term {
        $$ = new BinaryOpNode(">", $1, $3);
    }
    | comparison LE term {
        $$ = new BinaryOpNode("<=", $1, $3);
    }
    | comparison GE term {
        $$ = new BinaryOpNode(">=", $1, $3);
    }
    ;

term:
    factor { $$ = $1; }
    | term PLUS factor {
        $$ = new BinaryOpNode("+", $1, $3);
    }
    | term MINUS factor {
        $$ = new BinaryOpNode("-", $1, $3);
    }
    ;

factor:
    unary { $$ = $1; }
    | factor MULT unary {
        $$ = new BinaryOpNode("*", $1, $3);
    }
    | factor DIV unary {
        $$ = new BinaryOpNode("/", $1, $3);
    }
    ;

unary:
    PLUS unary %prec UNARY {
        $$ = new UnaryOpNode("+", $2);
    }
    | MINUS unary %prec UNARY {
        $$ = new UnaryOpNode("-", $2);
    }
    | primary { $$ = $1; }
    ;

primary:
    INTEGER {
        $$ = new IntegerNode($1);
    }
    | IDENTIFIER {
        if (!symTable.isDeclared(*$1)) {
            fprintf(stderr, "Line %d: Variable '%s' not declared\n", 
                    yylineno, $1->c_str());
            delete $1;
            YYABORT;
        }
        $$ = new VariableNode(*$1);
        delete $1;
    }
    | LPAREN expression RPAREN {
        $$ = $2;
    }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Line %d: Parse error: %s\n", yylineno, s);
}