#include "Node.h"


void Expression::accept(Visitor* v){
    throw EvaluationError("accept method of Expression didn't get redefined in subclass");
}


void Unary::accept(Visitor* v)
{
    v->visitUnaryOp(this);
}

void Arithm::accept(Visitor* v)
{
    v->visitBinOp(this);
}

void Access::accept(Visitor* v)
{
    v->visitAccess(this);
}

void If::accept(Visitor* v)
{
    v->visitIf(this);
}

void Else::accept(Visitor* v)
{
    v->visitElse(this);
}

void While::accept(Visitor* v)
{
    v->visitWhile(this);
}

void Do::accept(Visitor* v)
{
    v->visitDo(this);
}

void Set::accept(Visitor* v)
{
    v->visitSet(this);
}

void SetElem::accept(Visitor* v)
{
    v->visitSetElem(this);
}

void Break::accept(Visitor* v)
{
    v->visitBreak(this);
}

void Print::accept(Visitor* v)
{
    v->visitPrint(this);
}

void Id::accept(Visitor* v)
{
    v->visitId(this);
}

void intConstant::accept(Visitor* v)
{
    v->visitIntConstant(this);
}
void boolConstant::accept(Visitor* v)
{
    v->visitBoolConstant(this);
}


//Logical
void Not::accept(Visitor* v)
{
    v->visitNot(this);
}
void And::accept(Visitor* v)
{
    v->visitAnd(this);
}
void Rel::accept(Visitor* v)
{
    v->visitRel(this);
}

void Or::accept(Visitor* v)
{
    v->visitOr(this);
}

//Decls
void Decls::accept(Visitor* v) {
    v->visitDecls(this);
}

void Decl::accept(Visitor* v){
    v->visitDecl(this);
}

//Block
void Block::accept(Visitor* v)
{
    v->visitBlock(this);
}

//Type
void Type::accept(Visitor* v) {
    v->visitType(this);
}

void vectorType::accept(Visitor* v) {
    v->visitVectorType(this);
}

//Stmt
void Stmts::accept(Visitor* v) {
    v->visitStmts(this);
}

//Stmt should not 
void Stmt::accept(Visitor* v) {
    throw EvaluationError("accept method of Stmt didn't get redefined in subclass");
}



void Program::accept(Visitor* v)
{
    v->visitProgram(this);
}


std::string Type::typeid2String[Type::numOfTypes] = {"int","bool"};

std::string Rel::opCode2String[Rel::numOfOps] = {">",">=","<","<="};

std::string Op::binOp2String[Op::numOfBinOps] = {"+","-","*","/","==","!="};
std::string Op::unaryOp2String[Op::numOfUnaryOps] = {"-"};