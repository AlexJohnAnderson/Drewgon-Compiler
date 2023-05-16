%{
#include <string>
#include <limits.h>

/* Get our custom yyFlexScanner subclass */
#include "lexer.hpp"
#include "tokens.hpp"
#undef  YY_DECL
#define YY_DECL int drewgon::Scanner::yylex( drewgon::Parser::semantic_type * const lval )

/* define yyterminate as this instead of NULL */
#define yyterminate() return( TokenKind::END )

/* Exclude unistd.h for Visual Studio compatability. */
#define YY_NO_UNISTD_H

/* typedef to make the reference to token kinds shorter */
using TokenKind = drewgon::Parser::token;

%}

%option debug
%option nodefault
%option yyclass="drewgon::Scanner"
%option noyywrap
%option c++

DIGIT [0-9]
IDENT [a-zA-Z_][a-zA-Z0-9_]*
STLIT \"([0-9a-zA-Z_&!\(\)@#$%^\*\-\+;:'\.,/\|\}\{<>]|[ ]|["]|"\\n"|"\\t"|"\\\""|"\\\\")*\"
COMME \/\/.*"\n"
UNTER \"([0-9a-zA-Z_&!\(\)@#$%^\*\-\+;:'\.,/\|\}\{<>]|[ ]|[\ ]|\\|"\\n"|"\\t"|"\\\""|"\\\\")*
BDESC \"([0-9a-zA-Z_&!\(\)@#$%^\*\-\+;:'\.,/\|\}\{<>]|[ ]|[\ ]|\\|"\\a"|"\\b"|"\\c"|"\\d"|"\\e"|"\\f"|"\\g"|"\\h"|"\\i"|"\\j"|"\\k"|"\\l"|"\\m"|"\\n"|"\\o"|"\\p"|"\\q"|"\\r"|"\\s"|"\\t"|"\\u"|"\\v"|"\\w"|"\\x"|"\\y"|"\\\""|"\\\\")*\"
BDNUN \"([0-9a-zA-Z_&!\(\)@#$%^\*\-\+;:'\.,/\|\}\{<>]|[ ]|[\ ]|\\|"\a"|"\\b"|"\\c"|"\\d"|"\\e"|"\\f"|"\\g"|"\\h"|"\\i"|"\\j"|"\\k"|"\\l"|"\\m"|"\\n"|"\\o"|"\\p"|"\\q"|"\\r"|"\\s"|"\\t"|"\\u"|"\\v"|"\\w"|"\\x"|"\\y"|"\\z"|"\\\""|"\\\\")*



%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

{COMME}	{
		lineNum++;
		continue;
		}
		
"+"		{
		// Keep track of the "kind" of token we matched. In this case,
		// it's a PLUS (aka the plus symbol). The full list of token
		// kinds is declared around line 197 of grammar.hh. The testing
		// functionScanner::outputTokens also indicates what each token
		// represents (declared in scanner.cpp line 9)
		int tokenKind = TokenKind::PLUS;

		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);

		//Instatiate a token. There are 5 classes of tokens defined based on 
		// - a literal string (StringLitToken)
		// - a literal integer (IntLitToken)
		// - a literal short (ShortLitToken)
		// - an identifier (IDToken)
		// - a base Token (any other type)
		// The token object is a handy place to keep information about the
		// token (i.e. token metadata) such as the line and column on which
		// the token character started (which can be used later in debug 
		// messages, etc). 
		Token * token = new Token(pos, tokenKind);

		//Assign the token to a instance variable of the scanner.
		// (When Flex processes a .l file in C++ mode, it creates 
		// a class called Scanner, which is a subclass of yyFlexScanner)
		// By assigning a value to this field, we put the token
		// metadata in a place where scanner's client program can find it.
		this->yylval->lexeme = token;

		//Since we consumed 1 character, increase the column number by 1
		colNum++;

		//Let the scanner's client program know that a PLUS token
		// has been matched
		return tokenKind;
		}

"-" 	{
		//Essentially the same thing as PLUS, but without the verbose
		// comments so it doesn't look so scary.
		int tokenKind = TokenKind::MINUS;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		}

{DIGIT}+	{
		//TODO: The following computation of the integer
		// value does not check for overflow. It should do so.
		int intVal = atoi(yytext);
		std::string bounds = std::to_string(intVal);
		
		//Note that the IntLitToken does not take a TokenKind argument
		// since it's implicitly known that the token represents an
		// INTLITERAL		
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		if(yytext != bounds)
		{
			intVal = 0;
			std::string msg = "Integer literal overflow";
			Report::fatal(pos, msg);
		}
		
		yylval->lexeme = new IntLitToken(pos, intVal);
		colNum = colNum + strlen(yytext);
		//As before, alert the client program that an INTLITERAL 
		// token was matched
		return TokenKind::INTLITERAL;
			
		}
[ ]+	{
		
		}
[\r\n]+	{
		//Update the line number, reset the columnNumber
		lineNum++;
		colNum = 1;
		}

"{"		{
		int tokenKind = TokenKind::LCURLY;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		}
		
"}"		{
		int tokenKind = TokenKind::RCURLY;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		}
		
"("		{
		int tokenKind = TokenKind::LPAREN;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		}
		
")"		{
		int tokenKind = TokenKind::RPAREN;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		}
		
";"		{
		int tokenKind = TokenKind::SEMICOL;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		}
		
"->"		{
		int tokenKind = TokenKind::ARROW;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum = colNum + 2;
		return tokenKind;
		}
		
","		{
		int tokenKind = TokenKind::COMMA;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		}
		
"--"		{
		int tokenKind = TokenKind::POSTDEC;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum = colNum + 2;
		return tokenKind;
		}
		
"*"		{
		int tokenKind = TokenKind::TIMES;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		}
		
"="		{
		int tokenKind = TokenKind::ASSIGN;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		}
		
"!"		{
		int tokenKind = TokenKind::NOT;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		}
		
"/"		{
		int tokenKind = TokenKind::DIVIDE;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		}
		
"++"	{
		int tokenKind = TokenKind::POSTINC;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum = colNum + 2;
		return tokenKind;
		}
		
"=="	{
		int tokenKind = TokenKind::EQUALS;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum = colNum + 2;
		return tokenKind;
		}
		
"!="	{
		int tokenKind = TokenKind::NOTEQUALS;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum = colNum + 2;
		return tokenKind;
		}
		
">"		{
		int tokenKind = TokenKind::GREATER;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		}
		
"<"		{
		int tokenKind = TokenKind::LESS;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		}
		
"<="	{
		int tokenKind = TokenKind::LESSEQ;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum = colNum + 2;
		return tokenKind;
		}
		
">="	{
		int tokenKind = TokenKind::GREATEREQ;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum = colNum + 2;
		return tokenKind;
		}
		
"int" 	{
		int tokenKind = TokenKind::INT;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"fn" 	{
		int tokenKind = TokenKind::FN;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"bool"	{
		int tokenKind = TokenKind::BOOL;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"void" 	{
		int tokenKind = TokenKind::VOID;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"mayhem" 	{
		int tokenKind = TokenKind::MAYHEM;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"if" 	{
		int tokenKind = TokenKind::IF;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"else" 	{
		int tokenKind = TokenKind::ELSE;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"while" {
		int tokenKind = TokenKind::WHILE;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"for" 	{
		int tokenKind = TokenKind::FOR;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"return" 	{
		int tokenKind = TokenKind::RETURN;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"output" 	{
		int tokenKind = TokenKind::OUTPUT;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"input" {
		int tokenKind = TokenKind::INPUT;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
"true" 	{
		int tokenKind = TokenKind::TRUE;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"false" {
		int tokenKind = TokenKind::FALSE;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"or" 	{
		int tokenKind = TokenKind::OR;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
"and" 	{
		int tokenKind = TokenKind::AND;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum=colNum+strlen(yytext);
		return tokenKind;
		}
		
{IDENT}	{
		std::string strVal = yytext;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		yylval->lexeme = new IDToken(pos, strVal);
		colNum = colNum + strlen(yytext);
		return TokenKind::ID;
		}
		
{STLIT}	{
		std::string strLit = yytext;		
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext)+1);
		yylval->lexeme = new StrToken(pos, strLit);
		colNum = colNum + strlen(yytext);
		return TokenKind::STRINGLITERAL;
		}
		
{UNTER}	{
		std::string msg = "Unterminated string literal detected";
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext)+1);
		Report::fatal(pos, msg);
		colNum=colNum+strlen(yytext);
		}
		
{BDESC}	{
		std::string msg = "String literal with bad escape sequence detected";
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext)+1);
		Report::fatal(pos, msg);
		colNum=colNum+strlen(yytext);
		}

{BDNUN}	{
		std::string msg = "Unterminated string literal with bad escape sequence detected";
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext)+1);
		Report::fatal(pos, msg);
		colNum=colNum+strlen(yytext);
		}	

.		{
		//TODO: Add the rest of the rules above this point so that legal characters
		// are properly accounted for. 
		std::string msg = "Illegal character ";
		msg += yytext;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		Report::fatal(pos,msg);
		colNum += strlen(yytext);
		}
%%
