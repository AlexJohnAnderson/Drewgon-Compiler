#ifndef DREWGON_NAME_ANALYSIS
#define DREWGON_NAME_ANALYSIS

#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"
#include "errName.hpp"

namespace drewgon{

class NameAnalysis{
public:
	static NameAnalysis * build(ProgramNode * astIn){
		NameAnalysis * nameAnalysis = new NameAnalysis;
		SymbolTable * symTab = new SymbolTable();
		bool res = astIn->nameAnalysis(symTab);
		delete symTab;
		if (!res){ return nullptr; }
		nameAnalysis->ast = astIn;
		return nameAnalysis;
	}
	ProgramNode * ast;

private:
	NameAnalysis(){
		//This private constructor means the only way
		// to get a nameAnalysis instance is through
		// NameAnalysis::build, forcing the 
		// program to only have 1 instance total.
		// This is known as the "singleton" design pattern
	}
};

}

#endif
