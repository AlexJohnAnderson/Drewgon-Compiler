#include "ast.hpp"

namespace drewgon{

/*
doIndent is declared static, which means that it can 
only be called in this file (its symbol is not exported).
*/
static void doIndent(std::ostream& out, int indent){
	for (int k = 0 ; k < indent; k++){ out << "\t"; }
}

/*
In this code, the intention is that functions are grouped 
into files by purpose, rather than by class.
If you're used to having all of the functions of a class 
defined in the same file, this style may be a bit disorienting,
though it is legal. Thus, we can have
ProgramNode::unparse, which is the unparse method of ProgramNodes
defined in the same file as DeclNode::unparse, the unparse method
of DeclNodes.
*/


void ProgramNode::unparse(std::ostream& out, int indent){
	/* Oh, hey it's a for-each loop in C++!
	   The loop iterates over each element in a collection
	   without that gross i++ nonsense. 
	 */
	for (auto global : *myGlobals){
		/* The auto keyword tells the compiler
		   to (try to) figure out what the
		   type of a variable should be from 
		   context. here, since we're iterating
		   over a list of DeclNode *s, it's 
		   pretty clear that global is of 
		   type DeclNode *.
		*/
		global->unparse(out, indent);
	}
}

void VarDeclNode::unparse(std::ostream& out, int indent){
	if(indent == -1)
	{
		doIndent(out, indent);
		this->myType->unparse(out, 0);
		out << " ";
		this->myId->unparse(out, 0);
	}
	else
	{
		doIndent(out, indent);
		this->myType->unparse(out, 0);
		out << " ";
		this->myId->unparse(out, 0);
		out << ";\n";
	}
	
}

void IDNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << this->name;
}

void TypeNode::unparse(std::ostream& out, int indent){
	out << "";
}

void IntTypeNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "int";
}

void BoolTypeNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "bool";
}

void VoidTypeNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "void";
}

void FnTypeNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "fn";
}

void AssignExpNode::unparse(std::ostream& out, int indent){
	if(indent == -1)
	{
		doIndent(out, indent);
		this->myId->unparse(out, 0);
		out << " = ";
		this->myExp->unparse(out, 0);
	}
	else
	{
		doIndent(out, indent);
		this->myId->unparse(out, 0);
		out << " = ";
		this->myExp->unparse(out, 0);
		out << ";\n";
	}
}

void AndNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	this->myLhs->unparse(out, 0);
	out << " && ";
	this->myRhs->unparse(out, 0);
	out << ")";
}

void DivideNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	this->myLhs->unparse(out, 0);
	out << " / ";
	this->myRhs->unparse(out, 0);
	out << ")";
}

void EqualsNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	this->myLhs->unparse(out, 0);
	out << " == ";
	this->myRhs->unparse(out, 0);
	out << ")";
}

void GreaterEqNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	this->myLhs->unparse(out, 0);
	out << " >= ";
	this->myRhs->unparse(out, 0);
	out << ")";
}

void GreaterNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	this->myLhs->unparse(out, 0);
	out << " > ";
	this->myRhs->unparse(out, 0);
	out << ")";
}

void LessEqNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	this->myLhs->unparse(out, 0);
	out << " <= ";
	this->myRhs->unparse(out, 0);
	out << ")";
}

void LessNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	this->myLhs->unparse(out, 0);
	out << " < ";
	this->myRhs->unparse(out, 0);
	out << ")";
}

void MinusNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	this->myLhs->unparse(out, 0);
	out << " - ";
	this->myRhs->unparse(out, 0);
	out << ")";
}

void NotEqualsNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	this->myLhs->unparse(out, 0);
	out << " != ";
	this->myRhs->unparse(out, 0);
	out << ")";
}

void OrNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	this->myLhs->unparse(out, 0);
	out << " || ";
	this->myRhs->unparse(out, 0);
	out << ")";
}

void PlusNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	this->myLhs->unparse(out, 0);
	out << " + ";
	this->myRhs->unparse(out, 0);
	out << ")";
}

void TimesNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	this->myLhs->unparse(out, 0);
	out << " * ";
	this->myRhs->unparse(out, 0);
	out << ")";
}


void FalseNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "false";
}


void TrueNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "true";
}


void IntLitNode::unparse(std::ostream& out, int indent){
	out << this->numval;
}

void StrLitNode::unparse(std::ostream& out, int indent){
	out << this->stringVal;
}

void MayhemNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "mayhem";
}

void NotNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	out << " ! ";
	this->myRhs->unparse(out, 0);
	out << ")";
}

void NegNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "(";
	out << "-";
	this->myRhs->unparse(out, 0);
	out << ")";
}

void AssignStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	this->myAssignStmt->unparse(out, 0);
}

void CallStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	this->myCallStmt->unparse(out, 0);
}

void FormalDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	this->myType->unparse(out, 0);
	out << " ";
	this->myId->unparse(out, 0);
}

void FnDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	this->myType->unparse(out, 0);
	out << " ";
	this->myId->unparse(out, 0);
	out << "(";
	
	if (parameters != nullptr){
        
		std::string comma = "";
	    for (auto param: *parameters){
		    out << comma;
			param->unparse(out, 0);
			comma = ", ";
		}
	}
	out << ") {\n";
	for (auto stmt: *functionBody)
	{
		stmt->unparse(out, indent + 1);
	}
	doIndent(out, indent);
	out << "\n}\n";

}

void ForStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "for ("; 
	this->myStmt1->unparse(out, -1);
	out << ";";
	this->myExp->unparse(out, 0);
	out << ";";
	this->myStmt2->unparse(out, -1);
	out << "){\n";
	for (auto myStmt: *myStmtList)
	{
		myStmt->unparse(out, indent + 1);
	}
	doIndent(out, indent);
	out << "\n}\n";
}

void IfStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "if("; 
	this->myExp->unparse(out, -1);
	out << ") {\n";
	for (auto myStmt: *myStmtList)
	{
		myStmt->unparse(out, indent + 1);
	}
	out << "\n}\n";

}

void IfElseStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "if (";
	this->myCondition->unparse(out, -1); 
	out << ") {\n";
	for (auto stmt: *IfTrueBody)
	{
		stmt->unparse(out, indent + 1);
	}
	out << "\n}\n else {\n";
	for (auto stmt: *IfFalseBody)
	{
		stmt->unparse(out, indent + 1);
	}
	out << "\n}\n";
}

void WhileStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "while ("; 
	this->myCondition->unparse(out, -1); 
	out << ") {\n";
	for (auto stmt: *WhileBody)
	{
		stmt->unparse(out, indent + 1);
	}
	out << "\n}\n";
}

void InputStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "input";
	out << "(";
	this->myId->unparse(out, 0);
	out << ");";
}


void OutputStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "output";
	out << "(";
	this->myExp->unparse(out, 0);
	out << ");";
}

void PostDecStmtNode::unparse(std::ostream& out, int indent){
	if(indent == -1)
	{
		doIndent(out, indent);
		this->myId->unparse(out, 0);
		out << "--";
	}
	else
	{
		doIndent(out, indent);
		this->myId->unparse(out, 0);
		out << "--; \n";
	}
}

void PostIncStmtNode::unparse(std::ostream& out, int indent){
	if(indent == -1)
	{
		doIndent(out, indent);
		this->myId->unparse(out, 0);
		out << "++";
	}
	else
	{
		doIndent(out, indent);
		this->myId->unparse(out, 0);
		out << "++; \n";
	}
}

void ReturnStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "return ";
	this->myExp->unparse(out, 0);
	out << ";";
}

void CallExpNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	this->myId->unparse(out, 0); 
	out << "(";
	 for (auto exp: *myExp) {
	 	exp->unparse(out, 0);
	 }
	 
	out << ");\n";
}


} // End namespace drewgon
