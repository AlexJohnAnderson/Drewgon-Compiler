#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"

namespace drewgon{
	
static bool undeclID(const Position * pos){
	Report::fatal(pos, "Undeclared identifier");
	return false;
}
static bool badVarType(const Position * pos){
	Report::fatal(pos, "Invalid type in declaration");
	return false;
}
static bool multiDecl(const Position * pos){
	Report::fatal(pos, "Multiply declared identifier");
	return false;
}

//TODO here is a subset of the nodes needed to do nameAnalysis, 
// you should add the rest to allow for a complete treatment
// of any AST
bool ASTNode::nameAnalysis(SymbolTable * symTab){
	throw new ToDoError("This function should have"
		"been overriden in the subclass!");
}

bool ProgramNode::nameAnalysis(SymbolTable * symTab){
	bool res = true;
	ScopeTable * global = new ScopeTable();
	symTab->insert(global);
	for (auto global : *myGlobals){
		res = global->nameAnalysis(symTab) && res;
	}
	symTab->remove();
	return res;
}

bool DeclNode::nameAnalysis(SymbolTable * symTab){
	return true;
}

bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	if (myType->getTypeString().compare("void") == 0)
	{
		return(badVarType(myPos));
	}	
	
	SemSymbol * varDeclSymbol = new SemSymbol(myID->getName(), std::string("var"), myType->getTypeString());
	myID->attachSymbol(varDeclSymbol);
	nameAnalysisOk = symTab->insertSymbolIntoCurrentScope(varDeclSymbol);
	if (!nameAnalysisOk)
	{
		return(multiDecl(myPos));
	}
	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	std::string fnType("");
	std::string comma = "";
	for (auto formal : *myFormals) {
		fnType.append(comma + formal->getTypeNode()->getTypeString());
		comma = ",";
	}
	fnType.append("->" + myRetType->getTypeString());

	SemSymbol * fnDeclSymbol = new SemSymbol(myID->getName(), std::string("fn"), fnType);
	myID->attachSymbol(fnDeclSymbol);
	nameAnalysisOk = symTab->insertSymbolIntoCurrentScope(fnDeclSymbol);
	if (nameAnalysisOk)
	{
		ScopeTable * fnScope = new ScopeTable();
		symTab->insert(fnScope);
		for (auto formal : *myFormals) {
			nameAnalysisOk = formal->nameAnalysis(symTab);
			if (!nameAnalysisOk)
			{
				return false;
			}
		}
		for (auto stmt : *myBody) {
			nameAnalysisOk = stmt->nameAnalysis(symTab);
			if (!nameAnalysisOk)
			{
				symTab->remove();
				return false;
			}
		}
		symTab->remove();
		return true;
	}

	return nameAnalysisOk;
}

bool FormalDeclNode::nameAnalysis(SymbolTable* symTab){
	bool nameAnalysisOk = true;
	if (myType->getTypeString().compare("void") == 0)
	{
		return(badVarType(myPos));
	}	
	
	SemSymbol * varDeclSymbol = new SemSymbol(myID->getName(), std::string("var"), myType->getTypeString());
	myID->attachSymbol(varDeclSymbol);
	nameAnalysisOk = symTab->insertSymbolIntoCurrentScope(varDeclSymbol);
	if (!nameAnalysisOk)
	{
		return(multiDecl(myPos));
	}
	return nameAnalysisOk;
}

bool IntTypeNode::nameAnalysis(SymbolTable* symTab){
	// Name analysis may never even recurse down to IntTypeNode,
	// but if it does, just return true to indicate that 
	// name analysis has not failed, and add nothing to the symbol table
	return true;
}

bool BoolTypeNode::nameAnalysis(SymbolTable* symTab){
	// Name analysis may never even recurse down to BoolTypeNode,
	// but if it does, just return true to indicate that 
	// name analysis has not failed, and add nothing to the symbol table
	return true;
}

bool VoidTypeNode::nameAnalysis(SymbolTable* symTab){
	// Name analysis may never even recurse down to VoidTypeNode,
	// but if it does, just return true to indicate that 
	// name analysis has not failed, and add nothing to the symbol table
	return true;
}

bool BinaryExpNode::nameAnalysis(SymbolTable* symTab){
	bool resultLHS = myExp1->nameAnalysis(symTab);
	bool resultRHS = myExp2->nameAnalysis(symTab);
	return resultLHS && resultRHS;
}

bool MayhemNode::nameAnalysis(SymbolTable* symTab){
	return true;
}

bool IDNode::nameAnalysis(SymbolTable* symTab){
	mySymbol = symTab->searchscopes(name);
	if(mySymbol == nullptr)
	{
		return (undeclID(myPos));
	}
	return true;
}

bool CallExpNode::nameAnalysis(SymbolTable* symTab){
	bool res = true;
	res = myID->nameAnalysis(symTab);
	if (res)
	{
		for (auto arg : *myArgs )
		{
			res = arg->nameAnalysis(symTab);
			if (!res)
			{
				return false;
			}
		}
	}
	return res;
}

bool CallStmtNode::nameAnalysis(SymbolTable* symTab){	
	return myCallExp->nameAnalysis(symTab);
}

bool ReturnStmtNode::nameAnalysis(SymbolTable* symTab){
	return myExp->nameAnalysis(symTab);
}

bool ForStmtNode::nameAnalysis(SymbolTable* symTab){
	bool nameAnalysisInit = myInit->nameAnalysis(symTab);
	bool nameAnalysisCond = myCond->nameAnalysis(symTab);
	bool nameAnalysisItr = myCond->nameAnalysis(symTab);
	
	if(nameAnalysisInit && nameAnalysisCond && nameAnalysisItr)
	{
		ScopeTable * forScope = new ScopeTable();
		symTab->insert(forScope);
		for (auto stmt : *myBody) {
		bool nameAnalysisOk = stmt->nameAnalysis(symTab);
			if (!nameAnalysisOk)
			{
				symTab->remove();
				return false;
			}
		}
		symTab->remove();
		return true;
	}
	return false;
	
}

bool WhileStmtNode::nameAnalysis(SymbolTable* symTab){
	bool nameAnalysisOk = myCond->nameAnalysis(symTab);
	if (nameAnalysisOk)
	{
		ScopeTable * whileScope = new ScopeTable();
		symTab->insert(whileScope);
		for (auto stmt : *myBody) {
			nameAnalysisOk = stmt->nameAnalysis(symTab);
			if (!nameAnalysisOk)
			{
				symTab->remove();
				return false;
			}
		}
		symTab->remove();
		return true;
	}
	return false;
}

bool IfElseStmtNode::nameAnalysis(SymbolTable* symTab){
	bool nameAnalysisOk = myCond->nameAnalysis(symTab);
	if (nameAnalysisOk)
	{
		ScopeTable * ifScope = new ScopeTable();
		symTab->insert(ifScope);
		for (auto stmt : *myBodyTrue) {
			nameAnalysisOk = stmt->nameAnalysis(symTab);
			if (!nameAnalysisOk)
			{
				symTab->remove();
				return false;
			}
		}
		symTab->remove();
		ScopeTable * elseScope = new ScopeTable();
		symTab->insert(elseScope);
		for (auto stmt : *myBodyFalse) {
			nameAnalysisOk = stmt->nameAnalysis(symTab);
			if (!nameAnalysisOk)
			{
				symTab->remove();
				return false;
			}
		}
		symTab->remove();
		return true;
	}
	return false;
}

bool IfStmtNode::nameAnalysis(SymbolTable* symTab){
	bool nameAnalysisOk = myCond->nameAnalysis(symTab);
	if (nameAnalysisOk)
	{
		ScopeTable * ifScope = new ScopeTable();
		symTab->insert(ifScope);
		for (auto stmt : *myBody) {
			nameAnalysisOk = stmt->nameAnalysis(symTab);
			if (!nameAnalysisOk)
			{
				symTab->remove();
				return false;
			}
		}
		symTab->remove();
		return true;
	}
	return false;
}

bool PostDecStmtNode::nameAnalysis(SymbolTable* symTab){
	return myID->nameAnalysis(symTab);
}

bool PostIncStmtNode::nameAnalysis(SymbolTable* symTab){
	return myID->nameAnalysis(symTab);
}

bool OutputStmtNode::nameAnalysis(SymbolTable* symTab){
	return mySrc->nameAnalysis(symTab);
}


bool InputStmtNode::nameAnalysis(SymbolTable* symTab){
	return myDst->nameAnalysis(symTab);
}

bool AssignStmtNode::nameAnalysis(SymbolTable* symTab){
	return myExp->nameAnalysis(symTab);
}

bool AssignExpNode::nameAnalysis(SymbolTable* symTab){
	bool nameAnalysisOk = myDst->nameAnalysis(symTab);
	if (nameAnalysisOk)
	{
		return mySrc->nameAnalysis(symTab);
	}
	return false;
}

bool FnTypeNode::nameAnalysis(SymbolTable* symTab){
	return true;
}

bool UnaryExpNode::nameAnalysis(SymbolTable* symTab){
	return myExp->nameAnalysis(symTab);
}



}
