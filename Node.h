#ifndef NODE_H
#define NODE_H
#include <string>
#include <map>

//forward declaration essenziali per evitare errori di compilazipone
class Visitor;  
class Id;
class Stmts;
class Stmt;
class Block;

class Node
{
    public:
    virtual ~Node() = default;
    Node() = default;
    virtual void accept(Visitor* v) = 0;
};

//Type
class Type : public Node{

public:
    enum TypeCode{INT, BOOL};
    
    static const int numOfTypes = 2;
    static std::string  typeid2String [numOfTypes]; 

    Type(TypeCode t) : type{t}{}

    TypeCode getType() {return type;}
    

    void accept(Visitor* v) override;   

private:

    TypeCode type;

};

class vectorType : public Type{
public: 

    vectorType(Type::TypeCode t, int s) : Type(t), size{s}{}
    
    int getSize(){return size;}

    void accept(Visitor* v) override;   

private:
    int size;

};


//Decls
class Decl : public Node{
public:

    Decl(Type* t, Id* i): type{t}, id{i}{}
    Type* getType(){return type;}
    Id* getId(){return id;}

    void accept(Visitor* v) override;
private:
    Type* type;
    Id* id;
};

class Decls : public Node{
public:

    //la definizione di decls vuoto non è compito dell'user della classe 
    static constexpr Decls* EMPTY_DECLS = nullptr; 
    Decls(Decl* dec, Decls* decs ): declaration{dec}, declarations{decs}{}

    Decl* getDecl(){return declaration;}
    Decls* getDecls(){return declarations;}

    void accept(Visitor* v) override;

private:
    Decl* declaration;
    Decls* declarations; 
};



//Program
class Program : public Node{
public:

    Program(Block* b) : block{b}{}

    Block* getBlock() {return block;}
    
    void accept(Visitor* v) override;

private:
    Block* block;
};



//Expression
class Expression : public Node{
public:
    void accept(Visitor* v) override;
};
    
class Constant : public Expression {
    
public: 
    Constant(Type::TypeCode t) : typeCode{t}{}

private:
    Type::TypeCode typeCode;

};

class Id: public Expression
{
public:
  Id(std::string name_) : name{name_} {};
  Id& operator= (const Id& other) = default;
  
  std::string getName() {
    return name;
  }

  void accept(Visitor* v) override;

private:
  std::string name; 
};

class intConstant : public Constant{
public:

    intConstant(int v) : value{v}, Constant(Type::INT){}

    int getValue () const  {
        return value;
    }

    void accept(Visitor* v) override;

private:
    int value;
};

class boolConstant : public Constant{
public:
    
    boolConstant(bool v) : value{v}, Constant(Type::BOOL) {}
    
    bool getValue() const  {
        return value;
    }

    void accept(Visitor* v) override;

private:
    bool value;
};


//Logical
class Logical : public Expression{

};

class Not : public Logical{

public:

    Not(Expression* e) : exp{e}{}
    Expression* getExp() {return exp;}

    void accept(Visitor* v) override;

private:
    Expression* exp;

};

class And : public Logical{

public:

    And(Expression* l, Expression* r) : left{l}, right{r}{}
    Expression* getLeftExp() {return left;}
    Expression* getRightExp() {return right;}

    void accept(Visitor* v) override;

private:
    Expression* left;
    Expression* right;

};

class Or : public Logical{

public:

    Or(Expression* l, Expression* r) : left{l}, right{r}{}
    Expression* getLeftExp() {return left;}
    Expression* getRightExp() {return right;}

    void accept(Visitor* v) override;


private:
    Expression* left;
    Expression* right;

};

class Rel : public Logical{
public:
    enum OpCode {MORE, MORE_EQ, LESS, LESS_EQ};
    static const int numOfOps = 4;
    static std::string opCode2String[numOfOps];

    Rel(Expression* l, Expression* r, OpCode op) :
    left{l}, right{r}, operation{op}{}
    Expression* getLeftExp() {return left;}
    Expression* getRightExp() {return right;}
    OpCode getOp(){return operation;} 

    void accept(Visitor* v) override;

private:
    Expression* left;
    Expression* right;
    OpCode operation;
};




//Operations
class Op : public Expression {

public:
  enum BinOpCode {ADD, SUB, MUL, DIV, EQ, NOT_EQ};
  static const int numOfBinOps = 6;
  static std::string binOp2String[numOfBinOps];

  enum UnaryOpCode {UNARY_MIN};
  static const int numOfUnaryOps = 1; 
  static std::string unaryOp2String[numOfUnaryOps];
};

//operatori unari
class Unary : public Op{

public:
    Unary( Expression* ex, UnaryOpCode op) : exp{ex}, operation{op} {}

    //dato che al momento c'è solo un'operazione unaria la si assegna automaticamente
    Unary (Expression* ex) : exp{ex} {operation = UNARY_MIN;}
    Expression* getExp() {return exp;}
    UnaryOpCode getOp(){return operation;} 

    void accept(Visitor* v) override;

private:
    Expression* exp;
    UnaryOpCode operation;

};

//operatori binari
class Arithm : public Op{

public:

    Arithm(Expression* l, Expression* r, BinOpCode op) :
    left{l}, right{r}, operation{op}{} 
    Expression* getLeftExp() {return left;}
    Expression* getRightExp() {return right;}
    BinOpCode getOp() {return operation;} 

    void accept(Visitor* v) override;

private:
    Expression* left;
    Expression* right;
    BinOpCode operation;    
};

//operazione di accesso indicizzato a vettore
class Access : public Op{
public:

    Access(Id* vec, Expression* ind) : vector{vec}, index{ind}{}
    Id* getId() {return vector;}
    Expression* getIndex() {return index;}

    void accept(Visitor* v) override;

private:
    Id* vector;
    Expression* index;

};



//Stmts
class Stmts: public Node{
public:

static constexpr Stmts* EMPTY_STMTS = nullptr; 
Stmts(Stmts* stmts_, Stmt* stmt_) : stmts{stmts_}, stmt{stmt_}{}

Stmts* getStmts() {return stmts;}
Stmt* getStmt() {return stmt;}

void accept(Visitor* v) override;   

private:
Stmts* stmts;
Stmt* stmt;
};

class Stmt : public Node{
  
public:
    void accept(Visitor* v) override;   

};

class If : public Stmt {
public:

    If(Stmt* s, Expression* e) : stmt{s}, condition{e}{}

    Stmt* getStmt() {return stmt;}
    Expression* getCondition () {return condition;}

    void accept(Visitor* v) override;   

private:
    Stmt* stmt;
    Expression* condition;
};

class Else : public Stmt{
public:

    Else(Stmt* stmtTrue, Stmt* stmtFalse, Expression* e)
     : stmtIfTrue{stmtTrue}, stmtIfFalse{stmtFalse}, condition{e}{}

    Stmt* getifTrueStmt() {return stmtIfTrue;}
    Stmt* getifFalseStmt() {return stmtIfFalse;}
    Expression* getCondition () {return condition;}

    void accept(Visitor* v) override;   

private:
    Expression* condition;
    Stmt* stmtIfTrue;
    Stmt* stmtIfFalse;
};

class While : public Stmt{
public:

    While(Stmt* s, Expression* e) : stmt{s}, condition{e}{}
    
    Stmt* getStmt() {return stmt;}
    Expression* getCondition () {return condition;}

 
    void accept(Visitor* v) override;   

private:
    Stmt* stmt;
    Expression* condition;
};

class Do : public Stmt{
public:

    Do(Stmt* s, Expression* e) : stmt{s}, condition{e}{}
    Stmt* getStmt() {return stmt;}
    Expression* getCondition () {return condition;}


    void accept(Visitor* v) override;   

private:
    Stmt* stmt;
    Expression* condition;
  
};

class Set : public Stmt{
public:

    Set(Id* var, Expression* e) : variable{var}, exp{e}{}
    Id* getId(){return variable;}
    Expression* getExp(){return exp;}

    void accept(Visitor* v) override;   

private:
    Id* variable;
    Expression* exp;    
};

class SetElem : public Stmt{
public:

    SetElem(Id* v, Expression* e, Expression* i) :
         arrayName{v}, exp{e}, index{e} {}

    Id* getId(){return arrayName;}
    Expression* getExp(){return exp;}
    Expression* getIndex(){return index;}


    void accept(Visitor* v) override;   


private:
    Id* arrayName;
    Expression* exp;    
    Expression* index;
};

class Break : public Stmt{
public:
    Break() = default;
    void accept(Visitor* v) override;   

};

class Print : public Stmt{
public:

    Print(Expression* e) : expToPrint{e}{}
    Expression* getExp(){return expToPrint;}
   
    void accept(Visitor* v) override;   

private:
    Expression* expToPrint;    
};

class Block : public Stmt{
public:

    Block(Decls* decs, Stmts* stmts) : declarations{decs}, statements{stmts}{}
    Decls* getDecls(){return declarations;}
    Stmts* getStmts(){return statements;}

    void accept(Visitor* v) override;   

private:
    Decls* declarations;
    Stmts* statements;

};


#include "Visitor.h"


#endif