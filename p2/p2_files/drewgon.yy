%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace{drewgon}
%define api.parser.class {Parser}
%define parse.error verbose
%output "parser.cc"
%token-table

%code requires{
	#include <list>
	#include "tokens.hpp"
	namespace drewgon {
		class Scanner;
	}

//The following definition is required when 
// we don't use the %locations directive (which we won't)
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

//End "requires" code
}

%parse-param { drewgon::Scanner &scanner }
%code{
   // C std code for utility functions
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   // Our code for interoperation between scanner/parser
   #include "scanner.hpp"

  //Request tokens from our scanner member, not 
  // from a global function
  #undef yylex
  #define yylex scanner.yylex
}

/*
The %union directive is a way to specify the 
set of possible types that might be used as
translation attributes on a symbol.
For this project, only terminals have types (we'll
have translation attributes for non-terminals in the next
project)
*/
%union {
   drewgon::Token* lexeme;
   drewgon::Token* transToken;
   drewgon::StrToken* transStrToken;
   drewgon::IntLitToken* transIntToken;
   drewgon::Position* transPosition;
}

%define parse.assert

/* Terminals 
 *  No need to touch these, but do note the translation type
 *  of each node. Most are just "transToken", which is defined in
 *  the %union above to mean that the token translation is an instance
 *  of drewgon::Token *, and thus has no fields (other than line and column).
 *  Some terminals, like ID, are "transIDToken", meaning the translation
 *  also has a name field. 
*/
%token                   END	   0 "end file"
%token	<transToken>     AND
%token	<transToken>     ARROW
%token	<transToken>     ASSIGN
%token	<transToken>     BOOL
%token	<transToken>     COMMA
%token	<transToken>     DIVIDE
%token	<transToken>     ELSE
%token	<transToken>     EQUALS
%token	<transToken>     FALSE
%token	<transToken>     FN
%token	<transToken>     FOR
%token	<transToken>     GREATER
%token	<transToken>     GREATEREQ
%token	<transIDToken>   ID
%token	<transToken>     IF
%token	<transToken>     INPUT
%token	<transToken>     INT
%token	<transIntToken>  INTLITERAL
%token	<transToken>     LCURLY
%token	<transToken>     LESS
%token	<transToken>     LESSEQ
%token	<transToken>     LPAREN
%token	<transToken>     MINUS
%token	<transToken>     MAYHEM
%token	<transToken>     NOT
%token	<transToken>     NOTEQUALS
%token	<transToken>     OR
%token	<transToken>     OUTPUT
%token	<transToken>     PLUS
%token	<transToken>     POSTDEC
%token	<transToken>     POSTINC
%token	<transToken>     RETURN
%token	<transToken>     RCURLY
%token	<transToken>     RPAREN
%token	<transToken>     SEMICOL
%token	<transStrToken>  STRINGLITERAL
%token	<transToken>     TIMES
%token	<transToken>     TRUE
%token	<transToken>     VOID
%token	<transToken>     WHILE


%right ASSIGN
%left AND
%left OR
%nonassoc LESS
%nonassoc LESSEQ
%nonassoc GREATER
%nonassoc GREATEREQ
%nonassoc NOTEQUALS
%nonassoc EQUALS
%left MINUS
%left PLUS
%left TIMES
%left DIVIDE
%left NOT

%type program
%type globals

/* NOTE: Make sure to add precedence and associativity 
 * declarations
*/

%%

/* TODO: add productions for the other nonterminals in the 
   grammar and make sure that all of the productions of the 
   given nonterminals are complete
*/
program 	: globals{
				//For the project, we will only be checking std::cerr for 
				// correctness. You might choose to uncomment the following
				// Line to help you debug, which will print when this
				// production is applied
			}
			
globals 	: globals varDecl SEMICOL {}
			| globals fnDecl {}
			| {} /* epsilon*/ 
			
varDecl     : type id {}

type        : primType {}
            | FN fnType {}

primType    : INT {}
            | BOOL {}
            | VOID {}

fnType		: LPAREN typeList RPAREN ARROW type {}
			| LPAREN RPAREN ARROW type {}

typeList	: type {}
			| typeList COMMA type {}

fnDecl      : type id LPAREN RPAREN LCURLY stmtList RCURLY {}
			| type id LPAREN formals RPAREN LCURLY stmtList RCURLY {}

formals     : formalDecl {}
            | formals COMMA formalDecl {}

formalDecl	: type id {}

stmtList    : stmtList stmt SEMICOL {}
            | stmtList blockStmt {}
            | {} /* epsilon */ 

blockStmt	: WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY {}
			| FOR LPAREN stmt SEMICOL exp SEMICOL stmt RPAREN LCURLY stmtList RCURLY {}
			| IF LPAREN exp RPAREN LCURLY stmtList RCURLY {}
			| IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY {}

stmt        : varDecl {}
			| assignExp {}
			| id POSTDEC {}
			| id POSTINC {}
			| INPUT id {}
			| OUTPUT exp {}
			| RETURN exp SEMICOL {}
			| RETURN SEMICOL {}
			| callExp SEMICOL {}

exp         : assignExp {}
			| exp MINUS exp {}
			| exp PLUS exp {}
			| exp TIMES exp {}
			| exp DIVIDE exp {}
			| exp AND exp {}
			| exp OR exp {}
			| exp EQUALS exp {}
			| exp NOTEQUALS exp {}
			| exp GREATER exp {}
			| exp GREATEREQ exp {}
			| exp LESS exp {}
			| exp LESSEQ exp {}
			| NOT exp {}
			| MINUS term {}
			| term {}

assignExp   : id ASSIGN exp {}

callExp     : id LPAREN RPAREN {}  // fn call with no args
			| id LPAREN actualsList RPAREN {} // with args
			
actualsList : exp {}
			| actualsList COMMA exp {}
			

term        : id {}
			| INTLITERAL {}
			| STRINGLITERAL {}
			| TRUE {}
			| FALSE {}
			| LPAREN exp RPAREN {}
			| callExp {}

id          : ID {}
 /* TODO: add productions for the entire grammar of the language */
	
%%

void drewgon::Parser::error(const std::string& msg){
	//For the project, we will only be checking std::cerr for 
	// correctness. You might choose to uncomment the following
	// Line to help you debug, since it gives slightly more 
	// descriptive error messages 
	//std::cout << msg << std::endl;
	std::cerr << "\nSyntax error.\n" << std::endl;
}
