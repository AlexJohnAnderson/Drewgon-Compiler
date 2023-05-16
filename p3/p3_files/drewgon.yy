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
	#include "ast.hpp"
	#include <cstring>
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
%parse-param { drewgon::ProgramNode** root }
%code{
   // C std code for utility functions
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   // Our code for interoperation between scanner/parser
   #include "scanner.hpp"
   #include "ast.hpp"
   #include "tokens.hpp"

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
	drewgon::IDToken*                       transIDToken;
	drewgon::ProgramNode*                   transProgram;

	drewgon::ExpNode *                      transExp;
	std::list<drewgon::ExpNode *> *        transExpList;
	   drewgon::AssignExpNode *                transAssignExp;
	   drewgon::BinaryExpNode *                transBinaryExp;
		   drewgon::PlusNode *                     transPlus;
		   drewgon::AndNode *                      transAnd;
		   drewgon::DivideNode *                   transDivide;
		   drewgon::EqualsNode *                   transEquals;
		   drewgon::GreaterEqNode *                transGreaterEq;
		   drewgon::GreaterNode *                  transGreater;
		   drewgon::LessEqNode *                   transLessEq;
		   drewgon::LessNode *                     transLess;
		   drewgon::MinusNode *                    transMinus;
		   drewgon::NotEqualsNode *                transNotEquals;
		   drewgon::OrNode *                       transOr;
		   drewgon::TimesNode *                    transTimes;
	   drewgon::CallExpNode *                  transCallExp; 
	   drewgon::FalseNode *                    transFalse;
	   drewgon::IDNode *                       transID;
	   drewgon::IntLitNode *                   transIntLit;
	   drewgon::MayhemNode *                   transMayhem;
	   drewgon::StrLitNode *				   transStrLit;
	   drewgon::TrueNode *                     transTrue;   
	   drewgon::UnaryExpNode *                 transUnaryExp;
	drewgon::StmtNode *                     transStmt;
		std::list<drewgon::StmtNode *> *        transStmtList;
		drewgon::AssignStmtNode *               transAssignStmtNode;
		drewgon::CallStmtNode *                 transCallStmt;
		std::list<drewgon::DeclNode *> *        transDeclList;
			drewgon::FnDeclNode *               	   transFnDeclNode;
			drewgon::VarDeclNode *                     transVarDecl;
				drewgon::FormalDeclNode * 			       transFormalDecl;
				std::list<drewgon::FormalDeclNode *> *           transFormalDeclList;
			drewgon::ForStmtNode *                  transForStmt;
			drewgon::IfElseStmtNode *               transIfElseStmt;
			drewgon::IfStmtNode *               	transIfStmt;
			drewgon::InputStmtNode *                transInputStmt;
			drewgon::OutputStmtNode *               transOutputStmt;
			drewgon::PostDecStmtNode *              transPostDecStmt;
			drewgon::PostIncStmtNode *              transPostIncStmt;
			drewgon::ReturnStmtNode *               transReturnStmt;
			drewgon::WhileStmtNode *                transWhileStmt;
	drewgon::TypeNode *                     transType;
		std::list<drewgon::TypeNode *> *        transTypeList;
		drewgon::IntTypeNode *                  transIntType;
		drewgon::BoolTypeNode *                 transBoolType;
		drewgon::VoidTypeNode *                 transVoidType;
		drewgon::FnTypeNode *                   transFnType;
		std::list<drewgon::FnTypeNode *> *        transFnTypeList;
   drewgon::IntLitToken*                   transIntToken;
   drewgon::StrToken*                      transStrToken;
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

/* Nonterminals
*  The specifier in angle brackets
*  indicates the type of the translation attribute using
*  the names defined in the %union directive above
*  TODO: You will need to add more attributes for other nonterminals
*  to this list as you add productions to the grammar
*  below (along with indicating the appropriate translation
*  attribute type).
*/
/*    (attribute type)    (nonterminal)    */
%type <transProgram>    program
%type <transDeclList>   globals
%type <transType>       type
%type <transVarDecl>    varDecl
%type <transType>       primType
%type <transFnType> 	fnType
%type <transTypeList> 	typeList
%type <transFnDeclNode> fnDecl
%type <transFormalDeclList> formals
%type <transFormalDecl> formalDecl
%type <transStmtList>	stmtList
%type <transStmt>		blockStmt
%type <transStmt> 		stmt
%type <transExp>        exp
%type <transAssignExp>  assignExp
%type <transCallExp> 	callExp
%type <transExpList>	actualsList
%type <transExp>		term
%type <transID>         id


%right ASSIGN
%left OR
%left AND
%nonassoc LESS GREATER LESSEQ GREATEREQ EQUALS NOTEQUALS
%left MINUS PLUS
%left TIMES DIVIDE
%left NOT 

%%

program 	: globals
			{
			$$ = new ProgramNode($1);
			*root = $$;
			}

globals 	: globals varDecl SEMICOL
			{ 
				$$ = $1; 
				$$->push_back($2);
			}
			| globals fnDecl
			{
			//TODO: like most of the SDD clauses,
			// you'll need to add some code here
			// to make the LHS symbol have a reasonable
			// translation to create an AST
				$$ = $1;
				DeclNode * declNode = $2;
				$$->push_back(declNode);
			}
			| /* epsilon */
			{
			$$ = new std::list<DeclNode * >();
			}

varDecl 	: type id
			{
			const Position * p = new Position($1->pos(), $2->pos());
			$$ = new VarDeclNode(p, $1, $2);
			}

type		: primType
			{ $$ = $1; }
			| FN fnType
			{ 
				$$ = $2;
			}
primType	: INT
			{ $$ = new IntTypeNode($1->pos()); }
			| BOOL
			{ $$ = new BoolTypeNode($1->pos()); }
			| VOID
			{ $$ = new VoidTypeNode($1->pos()); }

fnType		: LPAREN typeList RPAREN ARROW type
			{ 
				const Position * p = new Position($1->pos(), $5->pos());
				$$ = new FnTypeNode(p, $2, $5);
			}
			| LPAREN RPAREN ARROW type
			{ 
				std::list<TypeNode *> * list = new std::list<TypeNode * >();
				const Position * p = new Position($1->pos(), $4->pos());
				$$ = new FnTypeNode(p,list,$4); 
			}

typeList	: type
			{
			  std::list<DeclNode *> * list = new std::list<DeclNode *>();
              $$->push_back($1);
			}
			| typeList COMMA type
			{
			  $1->push_back($3);
			  $$ = $1;
			}

fnDecl	 	: type id LPAREN RPAREN LCURLY stmtList RCURLY
			{ 
				const Position * p = new Position($1->pos(), $7->pos());
				std::list<FormalDeclNode *> * list = new std::list<FormalDeclNode * >();
				$$ = new FnDeclNode(p, $1, $2, list, $6); 
			}
			| type id LPAREN formals RPAREN LCURLY stmtList RCURLY
			{
				const Position * p = new Position($1->pos(), $8->pos());
				$$ = new FnDeclNode(p, $1, $2, $4, $7); 
			}

formals 	: formalDecl
			{ 
			  std::list<FormalDeclNode *> * list = new std::list<FormalDeclNode *>();
              list->push_back($1);
              $$ = list;
			  }
			| formals COMMA formalDecl
			{ 
			  $1->push_back($3);
			  $$ = $1;
			}

formalDecl 	: type id
			{ 
			  const Position * p = new Position($1->pos(), $2->pos());
			  $$ = new FormalDeclNode(p, $1, $2);
			}

stmtList 	: /* epsilon */
			{ 
				$$ = new std::list<StmtNode * >();
			}
			| stmtList stmt SEMICOL
			{
				$$ = $1;
				$$->push_back($2);
			}
			| stmtList blockStmt
			{
				$$ = $1;
				$$->push_back($2);
				
			}


blockStmt	: WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY
			{ 
				const Position * p = new Position($1->pos(), $7->pos());
				$$ = new WhileStmtNode(p, $3, $6); 
			}
			| FOR LPAREN stmt SEMICOL exp SEMICOL stmt RPAREN LCURLY stmtList RCURLY
			{ 
				const Position * p = new Position($1->pos(), $11->pos());
				$$ = new ForStmtNode(p, $3, $5, $7, $10); 
			}
			| IF LPAREN exp RPAREN LCURLY stmtList RCURLY
			{ 
				const Position * p = new Position($1->pos(), $7->pos());
				$$ = new IfStmtNode(p, $3, $6); 
			}
			| IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY
			{ 
				const Position * p = new Position($1->pos(), $11->pos());
				$$ = new IfElseStmtNode(p, $3, $6, $10); 
			}

stmt		: varDecl
			{ 
				$$ =$1;
			}
			| assignExp
			{ 
				Position * p = new Position($1->pos(), $1->pos());
				$$ = new AssignStmtNode(p, $1);
			}
			| id POSTDEC
			{
				Position * p = new Position($1->pos(), $2->pos());
				$$ = new PostDecStmtNode(p, $1);
			}
			| id POSTINC
			{ 	
				Position * p = new Position($1->pos(), $2->pos());
				$$ = new PostIncStmtNode(p, $1);
			}
			| INPUT id
			{ 
				Position * p = new Position($1->pos(), $2->pos());
				$$ = new InputStmtNode(p, $2);
			}
			| OUTPUT exp
			{ 
				Position * p = new Position($1->pos(), $2->pos());
				$$ = new OutputStmtNode(p, $2);
			}
			| RETURN exp
			{
				Position * p = new Position($1->pos(), $2->pos());
				$$ = new ReturnStmtNode(p, $2);
			}
			| RETURN
			{ 
 				Position * p = new Position($1->pos(), $1->pos());
				$$ = new ReturnStmtNode(p, nullptr);
			}
			| callExp
			{ 
				Position * p = new Position($1->pos(), $1->pos());
				$$ = new CallStmtNode(p, $1);
				//$$ = $1;
			}

exp			: assignExp 
			{  				
				$$ = $1;
			}
			| exp MINUS exp
			{ 
			Position * p = new Position($1->pos(), $3->pos());
			$$ = new MinusNode(p, $1, $3);
			}
			| exp PLUS exp
			{
			Position * p = new Position($1->pos(), $3->pos());
			$$ = new PlusNode(p, $1, $3);
			}
			| exp TIMES exp
			{
			Position * p = new Position($1->pos(), $3->pos());
			$$ = new TimesNode(p, $1, $3);
			}
			| exp DIVIDE exp
			{
			Position * p = new Position($1->pos(), $3->pos());
			$$ = new DivideNode(p, $1, $3);
			}
			| exp AND exp
			{ 
			Position * p = new Position($1->pos(), $3->pos());
			$$ = new AndNode(p, $1, $3);
			}
			| exp OR exp
			{
			Position * p = new Position($1->pos(), $3->pos());
			$$ = new OrNode(p, $1, $3);
			}
			| exp EQUALS exp
			{
			Position * p = new Position($1->pos(), $3->pos());
			$$ = new EqualsNode(p, $1, $3);
			}
			| exp NOTEQUALS exp
			{
			Position * p = new Position($1->pos(), $3->pos());
			$$ = new NotEqualsNode(p, $1, $3);
			}
			| exp GREATER exp
			{
			Position * p = new Position($1->pos(), $3->pos());
			$$ = new GreaterNode(p, $1, $3);
			}
			| exp GREATEREQ exp
			{
			Position * p = new Position($1->pos(), $3->pos());
			$$ = new GreaterEqNode(p, $1, $3);
			}
			| exp LESS exp
			{
			Position * p = new Position($1->pos(), $3->pos());
			$$ = new LessNode(p, $1, $3);
			}
			| exp LESSEQ exp
			{ 
			Position * p = new Position($1->pos(), $3->pos());
			$$ = new LessEqNode(p, $1, $3);
			}
			| NOT exp
			{  
				Position * p = new Position($1->pos(), $2->pos());
				$$ = new NotNode(p, $2);
			}
			| MINUS term
			{ 
				Position * p = new Position($1->pos(), $1->pos());
				$$ = new NegNode(p, $2);
			}
			| term
			{	
				$$ = $1;
			}

assignExp	: id ASSIGN exp
			{ 	
				Position * p = new Position($1->pos(), $3->pos());
				$$ = new AssignExpNode(p, $1, $3); 
			}

callExp		: id LPAREN RPAREN
			{ 
				std::list<ExpNode *> * list = new std::list<ExpNode * >();
				Position * p = new Position($1->pos(), $3->pos());
				$$ = new CallExpNode(p, $1, list); 
			}
			| id LPAREN actualsList RPAREN
			{ 
				Position * p = new Position($1->pos(), $4->pos());
				$$ = new CallExpNode(p, $1, $3);
			}

actualsList	: exp
			{ 
				ExpNode* expnode = $1;
				$$->push_back(expnode);
			}
			| actualsList COMMA exp
			{
				$1->push_back($3);
				$$ = $1;
			}

term 		: id
			{ 
				$$ = $1;
			}
			| INTLITERAL 
			{ 
				 
              Position * p = new Position($1->pos(), $1->pos());
              $$ = new IntLitNode(p, $1->num());
        
			}
			| STRINGLITERAL 
			{ 
				Position * p = new Position($1->pos(), $1->pos());
				$$ = new StrLitNode(p, $1->str());
			}
			| TRUE
			{
				$$ = new TrueNode($1->pos());
			}
			| FALSE
			{ 
				$$ = new FalseNode($1->pos());
			}
			| LPAREN exp RPAREN
			{ 
				$$ = $2;
			}
			| MAYHEM
			{
				$$ = new MayhemNode($1->pos());
			}
			| callExp
			{ 
				$$ = $1;
			}

id			: ID
			{ 
				const Position * pos = $1->pos();
				$$ = new IDNode(pos, $1->value());
			}
%%

void drewgon::Parser::error(const std::string& msg){
	std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
