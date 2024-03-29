#include "ast.hpp"
#include "errors.hpp"
#include "symbol_table.hpp"

namespace drewgon{

static void doIndent(std::ostream& out, int indent){
	for (int k = 0 ; k < indent; k++){ out << "    "; }
}

void ProgramNode::unparse(std::ostream& out, int indent){
	for (DeclNode * decl : *myGlobals){
		decl->unparse(out, indent);
	}
}

void VarDeclNode::unparse(std::ostream& out, int indent){
	if (indent == -1){
		myType->unparse(out, 0);
		out << " ";
		myID->unparse(out, 0);
	} else {
		doIndent(out, indent); 
		myType->unparse(out, 0);
		out << " ";
		myID->unparse(out, 0);
		out << ";\n";
	}
}

void FormalDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent); 
	getTypeNode()->unparse(out, 0);
	out << " ";
	ID()->unparse(out, 0);
}

void FnDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent); 
	myRetType->unparse(out, 0); 
	out << " ";
	myID->unparse(out, 0);
	out << "(";
	bool firstFormal2 = true;
	for(auto formal : *myFormals){
		if (firstFormal2) { firstFormal2 = false; }
		else { out << ", "; }
		formal->unparse(out, 0);
	}
	out << "){\n";
	for(auto stmt : *myBody){
		stmt->unparse(out, indent+1);
	}
	doIndent(out, indent);
	out << "}\n";
}

void AssignStmtNode::unparse(std::ostream& out, int indent){
	if (indent == -1){
		myExp->unparse(out,0);
	} else {
		doIndent(out, indent);
		myExp->unparse(out,0);
		out << ";\n";
	}
}

void InputStmtNode::unparse(std::ostream& out, int indent){
	if (indent == -1){
		out << "input ";
		myDst->unparse(out,0);
	} else {
		doIndent(out, indent);
		out << "input ";
		myDst->unparse(out,0);
		out << ";\n";
	}
}

void OutputStmtNode::unparse(std::ostream& out, int indent){
	if (indent == -1){
		out << "output ";
		mySrc->unparse(out,0);
	} else {
		doIndent(out, indent);
		out << "output ";
		mySrc->unparse(out,0);
		out << ";\n";
	}
}

void PostIncStmtNode::unparse(std::ostream& out, int indent){
	if (indent != -1){ doIndent(out, indent); }
	this->myID->unparse(out,0);
	out << "(int)";
	out << "++";
	if (indent != -1){ out << ";\n"; }
}

void PostDecStmtNode::unparse(std::ostream& out, int indent){
	if (indent != -1){ doIndent(out, indent); }
	this->myID->unparse(out,0);
	out << "(int)";
	out << "--";
	if (indent != -1){ out << ";\n"; }
}

void IfStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "if (";
	myCond->unparse(out, 0);
	out << "){\n";
	for (auto stmt : *myBody){
		stmt->unparse(out, indent + 1);
	}
	doIndent(out, indent);
	out << "}\n";
}

void IfElseStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "if (";
	myCond->unparse(out, 0);
	out << "){\n";
	for (auto stmt : *myBodyTrue){
		stmt->unparse(out, indent + 1);
	}
	doIndent(out, indent);
	out << "} else {\n";
	for (auto stmt : *myBodyFalse){
		stmt->unparse(out, indent + 1);
	}
	doIndent(out, indent);
	out << "}\n";
}

void WhileStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "while (";
	myCond->unparse(out, 0);
	out << "){\n";
	for (auto stmt : *myBody){
		stmt->unparse(out, indent + 1);
	}
	doIndent(out, indent);
	out << "}\n";
}

void ForStmtNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "for (";
	myInit->unparse(out, -1);
	out << "; ";
	myCond->unparse(out, 0);
	out << "; ";
	myItr->unparse(out, -1);
	out << "){\n";
	for (auto stmt : *myBody){
		stmt->unparse(out, indent + 1);
	}
	doIndent(out, indent);
	out << "}\n";
}

void ReturnStmtNode::unparse(std::ostream& out, int indent){
	if (indent == -1){
		out << "return";
		if (myExp != nullptr){
			out << " ";
			myExp->unparse(out, 0);
		}
	} else {
		doIndent(out, indent);
		out << "return";
		if (myExp != nullptr){
			out << " ";
			myExp->unparse(out, 0);
		}
		out << ";\n";
	}
}

void CallStmtNode::unparse(std::ostream& out, int indent){
	if (indent != -1){ doIndent(out, indent); }
	myCallExp->unparse(out, 0);
	if (indent != -1){ out << ";\n"; }
}

void ExpNode::unparseNested(std::ostream& out){
	out << "(";
	unparse(out, 0);
	out << ")";
}

void CallExpNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myID->unparse(out, 0);
	out << "(";
	bool firstArg2 = true;
	for(auto arg : *myArgs){
		if (firstArg2) { firstArg2 = false; }
		else { out << ", "; }
		arg->unparse(out, 0);
	}
	out << ")";
}

void CallExpNode::unparseNested(std::ostream& out){
	unparse(out, 0);
}

void MinusNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " - ";
	myExp2->unparseNested(out);
}

void PlusNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " + ";
	myExp2->unparseNested(out);
}

void TimesNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " * ";
	myExp2->unparseNested(out);
}

void DivideNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " / ";
	myExp2->unparseNested(out);
}

void AndNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " and ";
	myExp2->unparseNested(out);
}

void OrNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " or ";
	myExp2->unparseNested(out);
}

void EqualsNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " == ";
	myExp2->unparseNested(out);
}

void NotEqualsNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " != ";
	myExp2->unparseNested(out);
}

void GreaterNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " > ";
	myExp2->unparseNested(out);
}

void GreaterEqNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " >= ";
	myExp2->unparseNested(out);
}

void LessNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " < ";
	myExp2->unparseNested(out);
}

void LessEqNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myExp1->unparseNested(out); 
	out << " <= ";
	myExp2->unparseNested(out);
}

void NotNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "!";
	myExp->unparseNested(out); 
}

void NegNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "-";
	myExp->unparseNested(out); 
}

void VoidTypeNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "void";
}

void IntTypeNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "int";
}

void BoolTypeNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "bool";
}

void FnTypeNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	bool first = true;
	out << "fn (";
	for (auto inType : *myInTypes){
		if (first){ first = false; }
		else { out << ", "; }
		inType->unparse(out, indent);
	}
	out << ")";
	out << "->";
	myOutType->unparse(out, indent);
}

void AssignExpNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	myDst->unparse(out, 0);
	out << " = ";
	mySrc->unparseNested(out);
}

void IDNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << name;
	if (mySymbol != nullptr){
		out << "("
		  << mySymbol->getType()
		  << ")";
	}
}

void IDNode::unparseNested(std::ostream& out){
	this->unparse(out, 0);
}

void FalseNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "false";
}

void IntLitNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << myNum;
}

void StrLitNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << myStr;
}

void TrueNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "true";
}

void MayhemNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "mayhem";
}

} //End namespace drewgon
