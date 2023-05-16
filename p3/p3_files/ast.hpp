#ifndef CMINUSMINUS_AST_HPP
#define CMINUSMINUS_AST_HPP

#include <ostream>
#include <list>
#include <string.h>
#include "tokens.hpp"
#include <cassert>


/* You'll probably want to add a bunch of ASTNode subclasses */

namespace drewgon{

/* You may find it useful to forward declare AST subclasses
   here so that you can use a class before it's full definition
*/
class DeclNode;
class TypeNode;
class StmtNode;
class IDNode;
class ExpNode;
class TypeNode;
class IDNode;
class CallExpNode;
class ExpNode;
class VarDeclNode;
class IntTypeNode;
class BoolTypeNode;
class VoidTypeNode;
class FnTypeNode;
class AssignExpNode;
class BinaryExpNode;
class AndNode;
class DivideNode;
class EqualsNode;
class GreaterEqNode;
class LessEqNode;
class GreaterNode;
class LessNode;
class OrNode;
class MinusNode;
class NotEqualsNode;
class PlusNode;
class TimesNode;
class CallExpNode;
class FalseNode;
class IDNode;
class IntLitNode;
class MayhemNode;
class StrLitNode;
class TrueNode;
class UnaryExpNode;
class NegNode;
class NotNode;



/** 
* \class ASTNode
* Base class for all other AST Node types
**/
class ASTNode{
public:
	ASTNode(const Position * p) : myPos(p){ }
	virtual void unparse(std::ostream& out, int indent) = 0;
	const Position * pos() { return myPos; }
	std::string posStr() { return pos()->span(); }
protected:
	const Position * myPos = nullptr;
};

/** 
* \class ProgramNode
* Class that contains the entire abstract syntax tree for a program.
* Note the list of declarations encompasses all global declarations
* which includes (obviously) all global variables and struct declarations
* and (perhaps less obviously), all function declarations
**/
class ProgramNode : public ASTNode{
public:
	ProgramNode(std::list<DeclNode *> * globalsIn) ;
	void unparse(std::ostream& out, int indent) override;
private:
	std::list<DeclNode * > * myGlobals;
};

class StmtNode : public ASTNode{
public:
	StmtNode(const Position * p) : ASTNode(p){ }
	void unparse(std::ostream& out, int indent) override = 0;
};


/** \class DeclNode
* Superclass for declarations (i.e. nodes that can be used to 
* declare a struct, function, variable, etc).  This base class will 
**/
class DeclNode : public StmtNode{
public:
	DeclNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

/**  class ExpNode
* Superclass for expression nodes (i.e. nodes that can be used as
* part of an expression).  Nodes that are part of an expression
* should inherit from this abstract superclass.
**/
class ExpNode : public ASTNode{
public:
	ExpNode(const Position * p) : ASTNode(p){ }
	void unparse(std::ostream& out, int indent) override =0 ;
};

/** class TypeNode
* Superclass of nodes that indicate a data type. For example, in 
* the declaration "int a", the int part is the type node (a is an IDNode
* and the whole thing is a DeclNode).
**/
class TypeNode : public ASTNode{
protected:
	TypeNode(const Position * p) : ASTNode(p){
	}
public:
	virtual void unparse(std::ostream& out, int indent) override = 0;
};

/** An identifier. Note that IDNodes subclass
 * ExpNode because they can be used as part of an expression. 
**/
class IDNode : public ExpNode{
public:
	IDNode(const Position * p, std::string nameIn) 
	: ExpNode(p), name(nameIn){ }
	void unparse(std::ostream& out, int indent);
private:
	/** The name of the identifier **/
	std::string name;
};

/** A variable declaration.
**/
class VarDeclNode : public DeclNode{
public:
	VarDeclNode(const Position * p, TypeNode * type, IDNode * id) 
	: DeclNode(p), myType(type), myId(id){
		
	}
	void unparse(std::ostream& out, int indent) override;
private:
	TypeNode * myType;
	IDNode * myId;
};

class IntTypeNode : public TypeNode{
public:
	IntTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class BoolTypeNode : public TypeNode{
public:
	BoolTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class VoidTypeNode : public TypeNode{
public:
	VoidTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class FnTypeNode : public TypeNode{
public:
	FnTypeNode(const Position * p, std::list<TypeNode *> * type, TypeNode * ret)
	: TypeNode(p), myType(type), myRet(ret){
		
	}
	void unparse(std::ostream& out, int indent) override;
	
	std::list<TypeNode * > * myType;
	TypeNode * myRet;
	
};

class AssignExpNode : public ExpNode{
public:
	AssignExpNode( Position * p, IDNode * id, ExpNode * exp) 
	: ExpNode(p), myId(id), myExp(exp) {
		assert (myId != nullptr);
		assert (myExp != nullptr);
	}
	void unparse(std::ostream& out, int indent);

	IDNode * myId;
	ExpNode * myExp;
};

class BinaryExpNode : public ExpNode{
public:
	BinaryExpNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: ExpNode(p), myLhs(lhs), myRhs(rhs){
		assert (myLhs != nullptr);
		assert (myRhs != nullptr);
	}
	void unparse(std::ostream& out, int indent) override = 0;
	ExpNode * myLhs;
	ExpNode * myRhs;
};

class AndNode : public BinaryExpNode{
public:
	AndNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: BinaryExpNode(p, lhs, rhs), myLhs(lhs), myRhs(rhs) {}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myLhs;
	ExpNode * myRhs;
};

class DivideNode : public BinaryExpNode{
public:
	DivideNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: BinaryExpNode(p, lhs, rhs), myLhs(lhs), myRhs(rhs) {}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myLhs;
	ExpNode * myRhs;
};

class EqualsNode : public BinaryExpNode{
public:
	EqualsNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: BinaryExpNode(p, lhs, rhs), myLhs(lhs), myRhs(rhs) {}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myLhs;
	ExpNode * myRhs;
};

class GreaterEqNode : public BinaryExpNode{
public:
	GreaterEqNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: BinaryExpNode(p, lhs, rhs), myLhs(lhs), myRhs(rhs) {}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myLhs;
	ExpNode * myRhs;
};

class GreaterNode : public BinaryExpNode{
public:
	GreaterNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: BinaryExpNode(p, lhs, rhs), myLhs(lhs), myRhs(rhs) {}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myLhs;
	ExpNode * myRhs;
};

class LessEqNode : public BinaryExpNode{
public:
	LessEqNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: BinaryExpNode(p, lhs, rhs), myLhs(lhs), myRhs(rhs) {}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myLhs;
	ExpNode * myRhs;
};

class LessNode : public BinaryExpNode{
public:
	LessNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: BinaryExpNode(p, lhs, rhs), myLhs(lhs), myRhs(rhs) {}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myLhs;
	ExpNode * myRhs;
};

class MinusNode : public BinaryExpNode{
public:
	MinusNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: BinaryExpNode(p, lhs, rhs), myLhs(lhs), myRhs(rhs) {}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myLhs;
	ExpNode * myRhs;
};

class NotEqualsNode : public BinaryExpNode{
public:
	NotEqualsNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: BinaryExpNode(p, lhs, rhs), myLhs(lhs), myRhs(rhs) {}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myLhs;
	ExpNode * myRhs;
};

class OrNode : public BinaryExpNode{
public:
	OrNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: BinaryExpNode(p, lhs, rhs), myLhs(lhs), myRhs(rhs) {}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myLhs;
	ExpNode * myRhs;
};

class PlusNode : public BinaryExpNode{
public:
	PlusNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: BinaryExpNode(p, lhs, rhs), myLhs(lhs), myRhs(rhs) {}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myLhs;
	ExpNode * myRhs;
};

class TimesNode : public BinaryExpNode{
public:
	TimesNode(const Position * p, ExpNode * lhs, ExpNode * rhs)
	: BinaryExpNode(p, lhs, rhs), myLhs(lhs), myRhs(rhs) {}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myLhs;
	ExpNode * myRhs;
};



class CallExpNode : public ExpNode{
public:
	CallExpNode(const Position * p, IDNode * id, std::list<ExpNode *> * exp)
	: ExpNode(p), myId(id), myExp(exp){ }
	void unparse(std::ostream& out, int indent) override;
private:
	IDNode * myId;
	std::list<ExpNode *> * myExp;
};

class FalseNode : public ExpNode{
public:
	FalseNode(const Position * p): ExpNode(p){}
	void unparse(std::ostream& out, int indent) override;
};

class TrueNode : public ExpNode{
public:
	TrueNode(const Position * p): ExpNode(p){}
	void unparse(std::ostream& out, int indent) override;
};

class IntLitNode : public ExpNode{
public:
    IntLitNode(const Position * p, int Val)
    : ExpNode(p), numval(Val){ }
    void unparse(std::ostream& out, int indent) override;
private:
    int numval;
};

class StrLitNode : public ExpNode{
public:
	StrLitNode(Position * p, std::string Val)
	: ExpNode(p), stringVal(Val){ }
	void unparse(std::ostream& out, int indent) override;
private:
	std::string stringVal;
};

class MayhemNode : public ExpNode{
public:
	MayhemNode(const Position * p): ExpNode(p){}
	void unparse(std::ostream& out, int indent) override;
};

class UnaryExpNode : public ExpNode{
public:
	UnaryExpNode(const Position * p, ExpNode * exp)
	: ExpNode(p), myExp(exp){
		assert (myExp != nullptr);
	}
	void unparse(std::ostream& out, int indent) override = 0;
private:
	ExpNode * myExp;
};

	class NotNode : public UnaryExpNode{
		public:
			NotNode(const Position * p, ExpNode * rhs)
			: UnaryExpNode(p, rhs), myRhs(rhs){}
			void unparse(std::ostream& out, int indent) override;
		private:
			ExpNode * myRhs;
	};
		
	class NegNode : public UnaryExpNode{
		public:
			NegNode(const Position * p, ExpNode * rhs)
			: UnaryExpNode(p, rhs), myRhs(rhs){}
			void unparse(std::ostream& out, int indent) override;
		private:
			ExpNode * myRhs;
	};


class AssignStmtNode : public StmtNode{
public:
	AssignStmtNode(const Position * p, AssignExpNode * assignStmt)
	: StmtNode(p), myAssignStmt(assignStmt){
		assert (myAssignStmt != nullptr);
	}
	void unparse(std::ostream& out, int indent) override;
private:
	AssignExpNode * myAssignStmt;
};

class CallStmtNode : public StmtNode{
public:
	CallStmtNode(const Position * p, CallExpNode * callStmt)
	: StmtNode(p), myCallStmt(callStmt){
		assert (myCallStmt != nullptr);
	}
	void unparse(std::ostream& out, int indent) override;
private:
	CallExpNode * myCallStmt;
};

class FormalDeclNode : public VarDeclNode{
public:
	FormalDeclNode(const Position * p, TypeNode * type, IDNode * id)
	: VarDeclNode(p, type, id),  myType(type), myId(id) { }
	void unparse(std::ostream& out, int indent) override;
private:
	TypeNode * myType;
	IDNode * myId;
};

class FnDeclNode : public DeclNode{
public:
	FnDeclNode(const Position * p, TypeNode * type, IDNode * id, std::list<FormalDeclNode * > * paramIn, std::list<StmtNode * > * funcBody)
	: DeclNode(p), myType(type), myId(id), parameters(paramIn), functionBody(funcBody) { }
	void unparse(std::ostream& out, int indent) override;
private:
	TypeNode * myType;
	IDNode * myId;
	std::list<FormalDeclNode * > *parameters;
	std::list<StmtNode * > *functionBody;
};



class ForStmtNode : public StmtNode{
public:
	ForStmtNode(const Position * p, StmtNode * stmt1, ExpNode * exp, StmtNode * stmt2, std::list<StmtNode * > * stmtList)
	 : StmtNode(p), myStmt1(stmt1), myExp(exp), myStmt2(stmt2), myStmtList(stmtList){ 
			assert(myStmt1 != nullptr);
			assert(myExp != nullptr);
			assert(myStmt2 != nullptr);
			assert(myStmtList != nullptr);			
		}
		void unparse(std::ostream& out, int indent) override;
    private:
		StmtNode * myStmt1;
		ExpNode * myExp;
		StmtNode * myStmt2;
		std::list<StmtNode * > * myStmtList;

};

class IfStmtNode : public StmtNode{
public:
	IfStmtNode(const Position * p , ExpNode * exp , std::list<StmtNode * > * stmtList) 
	: StmtNode(p), myExp(exp), myStmtList(stmtList) {
		assert(myExp != nullptr);
		assert(myStmtList != nullptr);
		}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myExp;
	std::list<StmtNode * > * myStmtList;
};

class IfElseStmtNode : public StmtNode{
public:
	IfElseStmtNode(const Position * p , ExpNode * Condition, std::list<StmtNode *> * tbody, std::list<StmtNode *> * fbody ) 
	: StmtNode(p), myCondition(Condition), IfTrueBody(tbody) ,IfFalseBody(fbody) {
		assert(myCondition != nullptr);
		assert(IfTrueBody != nullptr);
		assert(IfFalseBody != nullptr);
		}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myCondition;
	std::list<StmtNode * > * IfTrueBody;
	std::list<StmtNode * > * IfFalseBody;
};

class WhileStmtNode : public StmtNode{
public:
	WhileStmtNode(const Position * p , ExpNode * Condition, std::list<StmtNode *> * body ) 
	: StmtNode(p), myCondition(Condition), WhileBody(body) {
		assert(myCondition != nullptr);
		assert(WhileBody != nullptr);
		}
	void unparse(std::ostream& out, int indent) override;
private:
	ExpNode * myCondition;
	std::list<StmtNode * > * WhileBody;
};

class InputStmtNode : public StmtNode{
public:
    InputStmtNode(const Position *p, IDNode * id)
	: StmtNode(p), myId(id) {
		assert(myId != nullptr);
		}
	void unparse(std::ostream& out, int indent) override;
private:
	IDNode * myId;
};

class OutputStmtNode : public StmtNode{
public:
    OutputStmtNode(const Position *p, ExpNode * exp)
	: StmtNode(p), myExp(exp) { 
		assert(myExp != nullptr);
	}
	void unparse(std::ostream& out, int indent) override;
private:
	ExpNode * myExp;
};

class PostDecStmtNode : public StmtNode{
public:
	PostDecStmtNode(const Position * p , IDNode * id) 
	: StmtNode(p), myId(id) {
		assert(myId != nullptr);
		}
	void unparse(std::ostream& out, int indent) override;
private:
	IDNode * myId;
};

class PostIncStmtNode : public StmtNode{
public:
	PostIncStmtNode(const Position * p , IDNode * id) 
	: StmtNode(p), myId(id) { 
		assert(myId != nullptr);
	}
	void unparse(std::ostream& out, int indent) override;
	private:
	IDNode * myId;
};

class ReturnStmtNode : public StmtNode{
public:
	ReturnStmtNode(const Position * p , ExpNode * exp) 
	: StmtNode(p), myExp(exp) { }
	ReturnStmtNode(Position * p) : StmtNode(p) {}
	void unparse(std::ostream& out, int indent) override;
	private:
	ExpNode * myExp;
};




} //End namespace drewgon

#endif
