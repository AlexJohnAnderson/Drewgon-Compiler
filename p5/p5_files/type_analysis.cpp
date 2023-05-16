#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"
#include "types.hpp"
#include "name_analysis.hpp"
#include "type_analysis.hpp"
#include <iterator>

namespace drewgon{

TypeAnalysis * TypeAnalysis::build(NameAnalysis * nameAnalysis){
	//To emphasize that type analysis depends on name analysis
	// being complete, a name analysis must be supplied for 
	// type analysis to be performed.
	TypeAnalysis * typeAnalysis = new TypeAnalysis();
	auto ast = nameAnalysis->ast;	
	typeAnalysis->ast = ast;

	ast->typeAnalysis(typeAnalysis);
	if (typeAnalysis->hasError){
		return nullptr;
	}

	return typeAnalysis;

}

void ProgramNode::typeAnalysis(TypeAnalysis * ta){

	//pass the TypeAnalysis down throughout
	// the entire tree, getting the types for
	// each element in turn and adding them
	// to the ta object's hashMap
	const DataType * noType = BasicType::produce(VOID);
	for (auto global : *myGlobals){
		global->typeAnalysis(ta, noType);
	}

	//The type of the program node will never
	// be needed. We can just set it to VOID
	//(Alternatively, we could make our type 
	// be error if the DeclListNode is an error)
	ta->nodeType(this, BasicType::produce(VOID));
}

void FnDeclNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){

	//HINT: you might want to change the signature for
	// typeAnalysis on FnBodyNode to take a second
	// argument which is the type of the current 
	// function. This will help you to know at a 
	// return statement whether the return type matches
	// the current function

	//Note: this function may need extra code
	const DataType * retType = myRetType->getType();
	for (auto stmt : *myBody){
		stmt->typeAnalysis(ta, retType);
	}
}

void StmtNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	TODO("Implement me in the subclass");
}

void AssignStmtNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	myExp->typeAnalysis(ta);

	//It can be a bit of a pain to write 
	// "const DataType *" everywhere, so here
	// the use of auto is used instead to tell the
	// compiler to figure out what the subType variable
	// should be
	auto subType = ta->nodeType(myExp);

	// As error returns null if subType is NOT an error type
	// otherwise, it returns the subType itself
	if (subType->asError()){
		ta->nodeType(this, subType);
	} else {
		ta->nodeType(this, BasicType::produce(VOID));
	}
}

void ExpNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Override me in the subclass");
}

void AssignExpNode::typeAnalysis(TypeAnalysis * ta){
	//TODO: Note that this function is incomplete. 
	// and needs additional code
	bool alreadyReported;
	//Do typeAnalysis on the subexpressions
	myDst->typeAnalysis(ta);
	mySrc->typeAnalysis(ta);

	const DataType * tgtType = ta->nodeType(myDst);
	const DataType * srcType = ta->nodeType(mySrc);
	
	if (tgtType->asFn() || (srcType->asFn() && !mySrc->isFnCall()))
	{
		Position * p = new Position(myDst->pos(), myDst->pos());
		ta->errAssignOpd(p);
		Position * q = new Position(mySrc->pos(), mySrc->pos());
		ta->errAssignOpd(q);
		alreadyReported = true;
	}

	if (mySrc->isFnCall() && tgtType == srcType->asFn()->getReturnType()) {
		ta->nodeType(this, tgtType);
		return;
	}

	if (tgtType == srcType){
		ta->nodeType(this, tgtType);
		return;
	}
	
	//Some functions are already defined for you to
	// report type errors. Note that these functions
	// also tell the typeAnalysis object that the
	// analysis has failed, meaning that main.cpp
	// will print "Type check failed" at the end
	if(!alreadyReported)
	{
		ta->errAssignOpr(this->pos());
		
	}

	//Note that reporting an error does not set the
	// type of the current node, so setting the node
	// type must be done
	ta->nodeType(this, ErrorType::produce());
}

void DeclNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	TODO("Override me in the subclass");
}

void VarDeclNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	// VarDecls always pass type analysis, since they 
	// are never used in an expression. You may choose
	// to type them void (like this), as discussed in class
	ta->nodeType(this, BasicType::produce(VOID));
}

void IDNode::typeAnalysis(TypeAnalysis * ta){
	// IDs never fail type analysis and always
	// yield the type of their symbol (which
	// depends on their definition)
	ta->nodeType(this, this->getSymbol()->getDataType());
}

void IntLitNode::typeAnalysis(TypeAnalysis * ta){
	// IntLits never fail their type analysis and always
	// yield the type INT
	ta->nodeType(this, BasicType::produce(INT));
}

void TypeNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, BasicType::produce(VOID));
}

void MayhemNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, BasicType::produce(VOID));
}

void TrueNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, BasicType::produce(BOOL));
}

void StrLitNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, BasicType::produce(VOID));
}

void FalseNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, BasicType::produce(STRING));
}

void NegNode::typeAnalysis(TypeAnalysis * ta){
	myExp->typeAnalysis(ta);

	const DataType * ExpType = ta->nodeType(myExp);

	if (ExpType->isInt() || ExpType->asFn()->getReturnType()->isInt() )
	{
		ta->nodeType(this, ExpType);
		return;
	}
	
	ta->errMathOpd(this->pos());
	ta->nodeType(this, ErrorType::produce());
}

void NotNode::typeAnalysis(TypeAnalysis * ta){
	myExp->typeAnalysis(ta);

	const DataType * ExpType = ta->nodeType(myExp);

	if (ExpType->isBool() || ExpType->asFn()->getReturnType()->isBool())
	{
		ta->nodeType(this, ExpType);
		return;
	}
	ta->errLogicOpd(this->pos());
	ta->nodeType(this, ErrorType::produce());
}

void LessEqNode::typeAnalysis(TypeAnalysis * ta){
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errRelOpd(myExp1->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errRelOpd(myExp2->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

void LessNode::typeAnalysis(TypeAnalysis * ta){
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errRelOpd(myExp1->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errRelOpd(myExp2->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

void GreaterEqNode::typeAnalysis(TypeAnalysis * ta){
myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errRelOpd(myExp1->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errRelOpd(myExp2->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

void GreaterNode::typeAnalysis(TypeAnalysis * ta){
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errRelOpd(myExp1->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errRelOpd(myExp2->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

void NotEqualsNode::typeAnalysis(TypeAnalysis * ta){
myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);

	if (left->asFn() == nullptr)
	{
		if (right->asFn() == nullptr)
		{
			if (left == right)
			{
				ta->nodeType(this, BasicType::produce(BOOL));
				return;
			}
			else {
				ta->errEqOpr(this->pos());
			}
		}
		else {
			if (!myExp2->isFnCall())
			{
				ta->errEqOpd(myExp2->pos());
			}
			
			if (left == right->asFn()->getReturnType())
			{
				ta->nodeType(this, BasicType::produce(BOOL));
				return;
			}
			else {
				ta->errEqOpr(this->pos());
			}
		}
	}
	else {
		if (right->asFn() == nullptr)
		{
			if (!myExp1->isFnCall())
			{
				ta->errEqOpd(myExp1->pos());
			}
			
			if (left->asFn()->getReturnType() == right)
			{
				ta->nodeType(this, BasicType::produce(BOOL));
				return;
			}
			else {
				ta->errEqOpr(this->pos());
			}
		}
		else {
			if (!myExp1->isFnCall() || !myExp2->isFnCall())
			{
				ta->errEqOpd(this->pos());
			}
			
			if (left->asFn()->getReturnType() == right->asFn()->getReturnType())
			{
				ta->nodeType(this, BasicType::produce(BOOL));
				return;
			}
			else {
				ta->errEqOpr(this->pos());
			}
			
		}
	}
	ta->nodeType(this, ErrorType::produce());
}

void EqualsNode::typeAnalysis(TypeAnalysis * ta){
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);

	if (left->asFn() == nullptr)
	{
		if (right->asFn() == nullptr)
		{
			if (left == right)
			{
				ta->nodeType(this, BasicType::produce(BOOL));
				return;
			}
			else {
				ta->errEqOpr(this->pos());
			}
		}
		else {
			if (!myExp2->isFnCall())
			{
				ta->errEqOpd(myExp2->pos());
			}
			
			if (left == right->asFn()->getReturnType())
			{
				ta->nodeType(this, BasicType::produce(BOOL));
				return;
			}
			else {
				ta->errEqOpr(this->pos());
			}
		}
	}
	else {
		if (right->asFn() == nullptr)
		{
			if (!myExp1->isFnCall())
			{
				ta->errEqOpd(myExp1->pos());
			}
			
			if (left->asFn()->getReturnType() == right)
			{
				ta->nodeType(this, BasicType::produce(BOOL));
				return;
			}
			else {
				ta->errEqOpr(this->pos());
			}
		}
		else {
			if (!myExp1->isFnCall() || !myExp2->isFnCall())
			{
				ta->errEqOpd(this->pos());
			}
			
			if (left->asFn()->getReturnType() == right->asFn()->getReturnType())
			{
				ta->nodeType(this, BasicType::produce(BOOL));
				return;
			}
			else {
				ta->errEqOpr(this->pos());
			}
			
		}
	}
	ta->nodeType(this, ErrorType::produce());
}

void OrNode::typeAnalysis(TypeAnalysis * ta){
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isBool())
		{
			ta->errLogicOpd(myExp1->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isBool() || !myExp1->isFnCall())
		{
			ta->errLogicOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isBool())
		{
			ta->errLogicOpd(myExp2->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isBool() || !myExp2->isFnCall())
		{
			ta->errLogicOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

void AndNode::typeAnalysis(TypeAnalysis * ta){
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isBool())
		{
			ta->errLogicOpd(myExp1->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isBool() || !myExp1->isFnCall())
		{
			ta->errLogicOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isBool())
		{
			ta->errLogicOpd(myExp2->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isBool() || !myExp2->isFnCall())
		{
			ta->errLogicOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

void DivideNode::typeAnalysis(TypeAnalysis * ta){
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errMathOpd(myExp1->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errMathOpd(myExp2->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(INT));
}

void TimesNode::typeAnalysis(TypeAnalysis * ta){
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errMathOpd(myExp1->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errMathOpd(myExp2->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(INT));
}

void PlusNode::typeAnalysis(TypeAnalysis * ta){
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);
	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errMathOpd(myExp1->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errMathOpd(myExp2->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(INT));
}

void MinusNode::typeAnalysis(TypeAnalysis * ta){
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * left = ta->nodeType(myExp1);
	const DataType * right = ta->nodeType(myExp2);

	if (left->asFn() == nullptr)
	{
		if (!left->isInt())
		{
			ta->errMathOpd(myExp1->pos());
		}
	}
	else {
		if (!left->asFn()->getReturnType()->isInt() || !myExp1->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (right->asFn() == nullptr)
	{
		if (!right->isInt())
		{
			ta->errMathOpd(myExp2->pos());
		}
	}
	else {
		if (!right->asFn()->getReturnType()->isInt() || !myExp2->isFnCall())
		{
			ta->errMathOpd(this->pos());
		}
	}
	if (!ta->passed())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	ta->nodeType(this, BasicType::produce(INT));
}

void CallExpNode::typeAnalysis(TypeAnalysis * ta){
	bool error = false;
	SemSymbol * sym = myID->getSymbol();
	std::string myKind = sym->kindToString(sym->getKind());
	const DataType * fnType = sym->getDataType();
	if (myKind != "fn") {
		ta->errCallee(myID->pos());
		error = true;
	}
	else {
		const TypeList * formalTypes = fnType->asFn()->getFormalTypes();
		if (formalTypes->count() != myArgs->size())
		{
			ta->errArgCount(this->pos());
			error = true;
		}
		
		ExpNode ** argArr = new ExpNode*[myArgs->size()];
		int arrPos = 0;
		for (auto arg : *myArgs) {
			argArr[arrPos] = arg;
			++arrPos;
		}

		arrPos = 0;
		const std::list<const DataType *> * types = formalTypes->getTypes(); 
		for (auto type : *types)
		{
			argArr[arrPos]->typeAnalysis(ta);
			auto argType = ta->nodeType(argArr[arrPos]);
			if (type != argType)
			{
				ta->errArgMatch(myID->pos());
				error = true;
			}
			++arrPos;
		}
	}
	if (error)
	{
		ta->nodeType(this, ErrorType::produce());
	}
	else {
		ta->nodeType(this, fnType->asFn()->getReturnType());
	}
	
}

void CallStmtNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	myCallExp->typeAnalysis(ta);
	ta->nodeType(this, ta->nodeType(myCallExp));
}

void ForStmtNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	myCond->typeAnalysis(ta);

	const DataType * CondType = ta->nodeType(myCond);

	if (CondType->isBool()|| currentFnType->isBool())
	{	
		for (auto stmt : *myBody)
		{
			stmt->typeAnalysis(ta, currentFnType);
		}
		ta->nodeType(this, BasicType::produce(VOID));
		return;

	}
	ta->errLoopCond(myCond->pos());
	ta->nodeType(this, ErrorType::produce());


}

void WhileStmtNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	myCond->typeAnalysis(ta);

	const DataType * CondType = ta->nodeType(myCond);

	if (CondType->isBool() || currentFnType->isBool())
	{
		for (auto stmt : *myBody)
		{
			stmt->typeAnalysis(ta, currentFnType);
		}
		ta->nodeType(this, BasicType::produce(VOID));
		return;
	}

	ta->errLoopCond(myCond->pos());
	ta->nodeType(this, ErrorType::produce());
}

void IfElseStmtNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	myCond->typeAnalysis(ta);

	const DataType * CondType = ta->nodeType(myCond);

	if (CondType->isBool()||currentFnType->isBool())
	{
		for (auto truebody : *myBodyTrue)
		{
			truebody->typeAnalysis(ta, currentFnType);
		}

		for (auto falsebody : *myBodyFalse)
		{
			falsebody->typeAnalysis(ta, currentFnType);
		}

		ta->nodeType(this, BasicType::produce(VOID));
		return;
	}
	
	ta->errIfCond(myCond->pos());
	ta->nodeType(this, ErrorType::produce());
}

void IfStmtNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	myCond->typeAnalysis(ta);

	const DataType * CondType = ta->nodeType(myCond);

	if (CondType->isBool() || currentFnType->isBool())
	{
		for (auto stmt : *myBody)
		{
			stmt->typeAnalysis(ta, currentFnType);
		}
		ta->nodeType(this, BasicType::produce(VOID));
		return;
	}
	else{
		ta->errIfCond(myCond->pos());
		ta->nodeType(this, ErrorType::produce());
	}
	

}

void PostDecStmtNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	myID->typeAnalysis(ta);

	const DataType * LValType = ta->nodeType(myID);

	if (LValType->isInt() && LValType->asFn() == nullptr )
	{
		ta->nodeType(this, LValType);
		return;
	}

	ta->errMathOpd(this->pos());
	ta->nodeType(this, ErrorType::produce());
}

void PostIncStmtNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	myID->typeAnalysis(ta);

	const DataType * LValType = ta->nodeType(myID);

	if (LValType->isInt() && LValType->asFn() == nullptr)
	{
		ta->nodeType(this, LValType);
		return;
	}
	
	ta->errMathOpd(this->pos());
	ta->nodeType(this, ErrorType::produce());
}

void ReturnStmtNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	
	const DataType * returnType;
	if (myExp)
	{
		myExp->typeAnalysis(ta);
		returnType = ta->nodeType(myExp);
	}
	else {
		returnType = BasicType::produce(VOID);
	}
	
	if (currentFnType->isVoid() && (returnType->isBool() || returnType->isInt()))
	{
		Position * p = new Position(myExp->pos(), myExp->pos());
		ta->extraRetValue(p);
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (!currentFnType->isVoid() && returnType->isVoid())
	{
		Position * p = new Position(myExp->pos(), myExp->pos());
		ta->errRetEmpty(p);
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (returnType->asFn())
	{
		if (currentFnType != returnType->asFn()->getReturnType())
		{
			ta->errRetWrong(myExp->pos());
			ta->nodeType(this, ErrorType::produce());
			return;
		}
		ta->nodeType(this, ta->nodeType(myExp));
		return;
	}
	
	if (currentFnType != returnType)
	{
		ta->errRetWrong(myExp->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	
	
	ta->nodeType(this, ta->nodeType(myExp));
}

void OutputStmtNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	mySrc->typeAnalysis(ta);

	const DataType * SrcType = ta->nodeType(mySrc);

	if (SrcType->asFn() != nullptr)
	{
		ta->errOutputFn(this->pos());
		ta->nodeType(this, ErrorType::produce());
	}

	if (SrcType->isVoid())
	{
		ta->errOutputVoid(this->pos());
		ta->nodeType(this, ErrorType::produce());
	}

	if (SrcType->isBool() || SrcType->isInt() || SrcType->isString())
	{
		ta->nodeType(this, SrcType);
		return;
	}
}

void InputStmtNode::typeAnalysis(TypeAnalysis * ta, const DataType * currentFnType){
	myDst->typeAnalysis(ta);

	const DataType * DstType = ta->nodeType(myDst);

	if (DstType->asFn() != nullptr)
	{
		ta->errAssignFn(this->pos());
		ta->nodeType(this, ErrorType::produce());
	}
	
	if (DstType->validVarType()){
		ta->nodeType(this, DstType);
		return;
	}
}
}

