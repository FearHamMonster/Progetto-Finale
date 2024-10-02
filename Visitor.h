#ifndef VISITOR_H
#define VISITOR_H

#include <vector>
#include <iostream>

#include "Node.h"
/*#include "ast/Op.h"
#include "ast/Program.h"
#include "ast/Logical.h"*/
#include "Exceptions.h"


class Program;
class Block;
class Type;
class vectorType;
class Decls;
class Decl;
class Id;

// Visitor astratto per la visita degli oggetti della gerarchia "Expression"
class Visitor {
public:
    virtual ~Visitor() {}
    virtual void visitProgram(Program* program) = 0;
    virtual void visitBlock(Block* program) = 0;
    virtual void visitType(Type* type) = 0;
    virtual void visitVectorType(vectorType* type) = 0;
    virtual void visitDecls(Decls* decls) = 0;
    virtual void visitDecl(Decl* program) = 0;
    virtual void visitId(Id* idNode) = 0;
    
    virtual void visitStmts(Stmts* program) = 0;
    
    virtual void visitIntConstant(intConstant* numNode) = 0;
    virtual void visitBoolConstant(boolConstant* numNode) = 0;
    virtual void visitBinOp(Arithm* arithNode) = 0;
    virtual void visitUnaryOp(Unary* unaryNode) = 0;
    virtual void visitAccess(Access* accessNode) = 0;
   
    virtual void visitIf(If* ifNode) = 0;
    virtual void visitElse(Else* elseNode) = 0;
    virtual void visitWhile(While* whileNode) = 0;
    virtual void visitDo(Do* doNode) = 0;
    virtual void visitSet(Set* setNode) = 0;
    virtual void visitSetElem(SetElem* setElemNode) = 0;
    virtual void visitBreak(Break* breakNode) = 0;
    virtual void visitPrint(Print* printNode) = 0;


    virtual void visitNot(Not* notNode) = 0;
    virtual void visitAnd(And* andNode) = 0;
    virtual void visitOr(Or* andNode) = 0;
    virtual void visitRel(Rel* relNode) = 0;
};

// Visitor concreto per la valutazione delle espressioni
class EvaluationVisitor : public Visitor {
public:
    EvaluationVisitor() : intAccumulator{ }, boolAccumulator{} { }
    ~EvaluationVisitor() = default;
    EvaluationVisitor(EvaluationVisitor const&) = delete;
    EvaluationVisitor& operator=(EvaluationVisitor const&) = delete;

    /*void visitOperator(Operator* opNode) override {
        // Prelevo i puntatori agli operandi e gli faccio accettare 
        // questo oggetto come visitatore (propago la visita)
        Expression* left = opNode->getLeft();
        left->accept(this);
        Expression* right = opNode->getRight();
        right->accept(this);
        // Prelevo i valori dall'accumulatore
        int rval = accumulator.back(); accumulator.pop_back();
        int lval = accumulator.back(); accumulator.pop_back();
        switch (opNode->getOp()) {
        case Operator::PLUS:
            accumulator.push_back(lval + rval); return;
        case Operator::MINUS:
            accumulator.push_back(lval - rval); return;
        case Operator::TIMES:
            accumulator.push_back(lval * rval); return;
        case Operator::DIV:
            if (rval == 0) {
                throw EvaluationError{ "Division by zero" };
            }
            accumulator.push_back(lval / rval); return;
        default:
            return;
        }
    }
    void visitIntConstant (intConstant* numNode) override {
         intAccumulator.push_back(numNode->getValue());
    }

    void visitBoolConstant (boolConstant* numNode) override {
         boolAccumulator.push_back(numNode->getValue());
    }
    */
    int getIntValue() const {
        return intAccumulator.back();
    }
    bool getBoolValue() const {
        return boolAccumulator.back();
    }
private:
    std::vector<int> intAccumulator;
    std::vector<bool> boolAccumulator;
};


// Visitor concreto per la stampa delle espressioni
class PrintVisitor : public Visitor {
public:
    PrintVisitor() = default;
    ~PrintVisitor() = default;
    PrintVisitor(PrintVisitor const&) = delete;
    PrintVisitor& operator=(PrintVisitor const&) = delete;

    void visitProgram(Program* program) override {
        std::cout<<"Program(";
        program->getBlock()->accept(this);
        std::cout<<")";
    }

    void visitBlock(Block* block) override {
        std::cout<<"Block(";
        if(block->getDecls()) 
            block->getDecls()->accept(this);
        else 
            std::cout<<"NULL";
        std::cout<<", ";
        if(block->getStmts()) 
            block->getStmts()->accept(this);
        else 
            std::cout<<"NULL";
        std::cout<<")";
    }

    void visitDecls(Decls* decls) override {

        std::cout<<"Decls(";
        decls->getDecl()->accept(this);
        std::cout<<", ";
        if(decls->getDecls())
            decls->getDecls()->accept(this);
        else 
            std::cout<<"NULL";
        std::cout<<")";
    }

    void visitDecl(Decl* decl) override {
        std::cout<<"Decl(";
        decl->getType()->accept(this);
        std::cout<<", ";
        decl->getId()->accept(this);
        std::cout<<")";
    }

    void visitType(Type* type) override {
        std::cout<<"Type(";
        std::cout<<Type::typeid2String[type->getType()];
        std::cout<<")";
    }

    void visitVectorType(vectorType* type) override {
        std::cout<<"VectorType(";
        std::cout<<Type::typeid2String[type->getType()];
        std::cout<<", ["<<type->getSize()<<"]";
        std::cout<<")";
    }


    void visitId(Id* id) override {
        std::cout<<"Id(";
        std::cout<<id->getName();
        std::cout<<")";
    }

    void visitStmts(Stmts* stmts) override {

        std::cout<<"Stmts(";
        stmts->getStmt()->accept(this);
        std::cout<<", ";
        if(stmts->getStmts())
            stmts->getStmts()->accept(this);
        else 
            std::cout<<"NULL";
        std::cout<<")";
    }    
    
    void visitIf(If* ifNode) override {
        std::cout<<"If(";
        ifNode->getCondition()->accept(this);
        std::cout<<", ";
        ifNode->getStmt()->accept(this);
        std::cout<<")";
    }


    void visitElse(Else* elseNode) override {
        std::cout<<"Else(";
        elseNode->getCondition()->accept(this);
        std::cout<<", ";
        elseNode->getifTrueStmt()->accept(this);
        std::cout<<", ";
        elseNode->getifFalseStmt()->accept(this);
        std::cout<<")";
    }

    void visitWhile(While* whileNode) override {
        std::cout<<"While(";
        whileNode->getCondition()->accept(this);
        std::cout<<", ";
        whileNode->getStmt()->accept(this);
        std::cout<<")";
    }

    void visitDo(Do* doNode) override {
        std::cout<<"Do(";
        doNode->getCondition()->accept(this);
        std::cout<<", ";
        doNode->getStmt()->accept(this);
        std::cout<<")";
    }

    void visitSet(Set* setNode) override {
        std::cout<<"Set(";
        setNode->getId()->accept(this);
        std::cout<<", ";        
        setNode->getExp()->accept(this);
        std::cout<<")";
    }

    void visitSetElem(SetElem* setElemNode) override {
        std::cout<<"SetElem(";
        setElemNode->getId()->accept(this);
        std::cout<<",[";        
        setElemNode->getIndex()->accept(this);
        std::cout<<"], ";
        setElemNode->getExp()->accept(this);
        std::cout<<")";        
    }

    void visitBreak(Break* breakNode) override {
        std::cout<<"Break()";
    }

    void visitPrint(Print* printNode) override {
        std::cout<<"Print(";
        printNode->getExp()->accept(this);
        std::cout<<")";
    }

    void visitNot(Not* notNode) override {
        std::cout<<"Not(";
        notNode->getExp()->accept(this);
        std::cout<<")";        
    }

    void visitAnd(And* andNode) override {
        std::cout<<"And(";
        andNode->getLeftExp()->accept(this);
        std::cout<<", ";        
        andNode->getRightExp()->accept(this);
        std::cout<<")";        
    }

    void visitOr(Or* orNode) override {
        std::cout<<"Or(";
        orNode->getLeftExp()->accept(this);
        std::cout<<", ";        
        orNode->getRightExp()->accept(this);
        std::cout<<")";        
    }

    void visitRel(Rel* relNode) override {
        std::cout<<"Rel(";
        std::cout<<Rel::opCode2String[relNode->getOp()];
        std::cout<<", ";        
        relNode->getLeftExp()->accept(this);
        std::cout<<", ";        
        relNode->getRightExp()->accept(this);
        std::cout<<")";                
    }

    void visitIntConstant(intConstant* numNode) 
    {
        std::cout<<"IntConstant("<<numNode->getValue()<<")";
    }
    
    void visitBoolConstant(boolConstant* numNode)
    {
        std::cout<<"BoolConstant("<<numNode->getValue()<<")";
    } 
    
    void visitBinOp(Arithm* arithmNode)
    {
        std::cout<<"Arithm(";
        std::cout<<Op::binOp2String[arithmNode->getOp()];
        std::cout<<", ";        
        arithmNode->getLeftExp()->accept(this);
        std::cout<<", ";        
        arithmNode->getRightExp()->accept(this);
        std::cout<<")";                
    }
    
    void visitUnaryOp(Unary* unaryNode)
    {
        std::cout<<"Unary(";
        std::cout<<Op::unaryOp2String[unaryNode->getOp()];
        std::cout<<", ";        
        unaryNode->getExp()->accept(this);        
        std::cout<<")";                
    }

    void visitAccess(Access* accessNode)
    {
        std::cout<<"Access(";
        accessNode->getId()->accept(this);
        std::cout<<",[";        
        accessNode->getIndex()->accept(this);
        std::cout<<",] )";        
    }

};


#endif
