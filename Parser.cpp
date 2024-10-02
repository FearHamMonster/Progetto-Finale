#include <sstream>

#include "Parser.h"


Program* Parser::parseProgram()
{
    return em.makeProgram(parseBlock());
}


Block* Parser::parseBlock()
{
    consumeToken(Token::LEFT_CURLY);
    auto decls = parseDecls();
    auto stmts = parseStmts();
    Block* block = em.makeBlock(decls, stmts);
    consumeToken(Token::RIGHT_CURLY);
    return block;
}

Stmts* Parser::parseStmts()
{
    if(tokenItr->tag == Token::RIGHT_CURLY) //end of block reached
        return Stmts::EMPTY_STMTS;

    Stmt* stmt = parseStmt();
    Stmts* stmts = parseStmts();
    return em.makeStmts(stmts,stmt);
}

Stmt* Parser::parseStmt()
{
    switch(tokenItr->tag)
    {
        case Token::ID:
        {   
            Id* id = parseId();
            if(tokenItr->tag == Token::LEFT_SQUARE) 
            {
                safe_next();
                Expression* index = parseExpression();
                consumeToken(Token::RIGHT_SQUARE);
                consumeToken(Token::ASSIGN);
                Expression* expr = parseExpression();
                consumeToken(Token::END_STMT);
                return em.makeSetElem(id, index, expr);   
            }
            else
            {
                consumeToken(Token::ASSIGN);
                Expression* expr = parseExpression();
                consumeToken(Token::END_STMT);
                return em.makeSet(id, expr);
            }
        }
       
        case Token::IF:
        {  
            safe_next();
            consumeToken(Token::LP);
            Expression* condition = parseExpression();
            consumeToken(Token::RP);
            Stmt* ifTrueStmt = parseStmt();
            if(tokenItr->tag == Token::ELSE)
            {
                safe_next();
                Stmt* ifFalseStmt = parseStmt();
                return em.makeElse(ifTrueStmt,ifFalseStmt,condition);
            }
            return em.makeIf(ifTrueStmt, condition);
        }

        case Token::WHILE:
        {
            safe_next();
            consumeToken(Token::LP);
            Expression* condition = parseExpression();
            consumeToken(Token::RP);
            Stmt* stmt = parseStmt();
            return em.makeWhile(stmt,condition);
        }

        case Token::DO:
        {
            safe_next();
            Stmt* stmt = parseStmt();
            consumeToken(Token::WHILE);
            consumeToken(Token::LP);
            Expression* condition = parseExpression();
            consumeToken(Token::RP);
            consumeToken(Token::END_STMT);
            return em.makeDo(stmt,condition);
        }
        
        case Token::BREAK:
        {
            safe_next();
            consumeToken(Token::END_STMT);
            return em.makeBreak();
        }

        case Token::PRINT:
        {
            safe_next();
            consumeToken(Token::LP);
            Expression* exprToPrint = parseExpression();
            consumeToken(Token::RP);
            consumeToken(Token::END_STMT);
            return em.makePrint(exprToPrint);
        }

        case Token::LEFT_CURLY:
        {
            return parseBlock();
        }

        default:    
        {
            throw ParseError("No valid symbol at start of Stmt Parsing");
        }
    }
}


Decls* Parser::parseDecls()
{
    //if condition is true, decls is not null, so it contains at least one declaration
    if(tokenItr->tag == Token::BOOL || tokenItr->tag == Token::INT)
    {
        Decl* decl = parseDecl();
        return em.makeDecls(decl, parseDecls());
    }
    else return Decls::EMPTY_DECLS;
}

Decl* Parser::parseDecl()
{
  Type* type = parseType();
  Id* id = parseId();
  consumeToken(Token::END_STMT);
  return em.makeDecl(type,id);
}

Id* Parser::parseId()
{
    if(tokenItr->tag != Token::ID)
        throw ParseError{"Expected identifier, not found"};
        
    auto id = em.makeId(tokenItr->word);
    safe_next();
    return id;
    
}

Type* Parser::parseType()
{
    Type::TypeCode typeCode;
    switch (tokenItr->tag)
    {
    case Token::INT:
        typeCode = Type::INT;
        break;

    case Token::BOOL:
        typeCode = Type::BOOL;
        break;
    
    default:
        throw ParseError{"basic type expected and not found"};
    }

    safe_next(); //skip typeCode

    //if condition true, type is a vector type (for example, "int [10] myArray")
    if(tokenItr->tag == Token::LEFT_SQUARE)
    {
        safe_next();    //skip bracket
        if(tokenItr->tag != Token::NUM)
            throw ParseError{"Expected numeric constant, not found"};
        
        Type* type = em.makeVectorType(typeCode, std::stoi(tokenItr->word));
        safe_next();    //skip number
        consumeToken(Token::RIGHT_SQUARE);  //skip bracket  
        return type;
    }
    //return non-vector basic type
    return em.makeType(typeCode);
}


//<bool> -> <bool> || <join> | <join> can be expressed in the following way:
// <bool> -> <join> (|| <join>)*

Expression* Parser::parseExpression()
{
    Expression* exp = parseAnd();
    while(tokenItr->tag == Token::OR)
    {
        safe_next();
        exp = em.makeOr(exp, parseAnd());
    }
    return exp;
}

Expression* Parser::parseAnd()
{
    Expression* exp = parseEquality();
    while(tokenItr->tag == Token::OR)
    {
        safe_next();
        exp = em.makeAnd(exp, parseEquality());
    }
    return exp;
    
}

Expression* Parser::parseEquality()
{
    Expression* exp = parseRel();
    while(tokenItr->tag == Token::EQ || tokenItr->tag == Token::NOT_EQ)
    {
        if(tokenItr->tag == Token::EQ)
        {
            safe_next();
            exp = em.makeBinOp(Op::EQ, exp, parseRel());
        }
        else if(tokenItr->tag == Token::NOT_EQ)
        {
            safe_next();
            exp = em.makeBinOp(Op::NOT_EQ, exp, parseRel());
        }
    }
    return exp;
}

Expression* Parser::parseRel()
{
    switch((tokenItr+1)->tag)
    {
        case Token::LESS:
        {
            Expression* exp1 = parseLowerPrecedenceBinOp();
            safe_next();
            Expression* exp2 = parseLowerPrecedenceBinOp();
            return em.makeRel(exp1,exp2,Rel::LESS);
        }

        case Token::LESS_EQ:
        {
            Expression* exp1 = parseLowerPrecedenceBinOp();
            safe_next();
            Expression* exp2 = parseLowerPrecedenceBinOp();
            return em.makeRel(exp1,exp2,Rel::LESS_EQ);
        }
        
        case Token::MORE:
        {
            Expression* exp1 = parseLowerPrecedenceBinOp();
            safe_next();
            Expression* exp2 = parseLowerPrecedenceBinOp();
            return em.makeRel(exp1,exp2,Rel::MORE);
        }

        case Token::MORE_EQ:
        {
            Expression* exp1 = parseLowerPrecedenceBinOp();
            safe_next();
            Expression* exp2 = parseLowerPrecedenceBinOp();
            return em.makeRel(exp1,exp2,Rel::MORE_EQ);
        } 

        default:
        {
            return parseLowerPrecedenceBinOp();
        }
    }

}


Expression* Parser::parseLowerPrecedenceBinOp()
{
    Expression* exp = parseHigerPrecedenceBinOp();
    while(tokenItr->tag == Token::ADD || tokenItr->tag == Token::MIN)
    {
        if(tokenItr->tag == Token::ADD)
        {
            safe_next();
            exp = em.makeBinOp(Op::ADD, exp, parseHigerPrecedenceBinOp());
        }
        else if(tokenItr->tag == Token::MIN)
        {
            safe_next();
            exp = em.makeBinOp(Op::SUB, exp, parseHigerPrecedenceBinOp());
        }
    }
    return exp;   
}

Expression* Parser::parseHigerPrecedenceBinOp()
{
    Expression* exp = parseUnaryOp();
    while(tokenItr->tag == Token::MUL || tokenItr->tag == Token::DIV)
    {
        if(tokenItr->tag == Token::MUL)
        {
            safe_next();
            exp = em.makeBinOp(Op::MUL, exp, parseUnaryOp());
        }
        else if(tokenItr->tag == Token::DIV)
        {
            safe_next();
            exp = em.makeBinOp(Op::DIV, exp, parseUnaryOp());
        }
    }
    return exp;   
}

Expression* Parser::parseUnaryOp()
{
    switch(tokenItr->tag)
    {
        case Token::NOT:
            safe_next();
            return em.makeNot(parseUnaryOp());
            break;

        case Token::MIN:
            safe_next();
            return em.makeUnaryOp(Op::UNARY_MIN, parseUnaryOp());
            break;
        default:
            return parseFactor();
            break;
    }
}


Expression* Parser::parseFactor()
{
    switch(tokenItr->tag)
    {
        case Token::LP:
        {
            safe_next();
            Expression* exp = parseExpression();
            consumeToken(Token::RP);
            return exp;
        }
        
        case Token::ID:
        {
            Id* id = parseId();
            if(tokenItr->tag == Token::LEFT_SQUARE) 
            {
                safe_next();
                Expression* index = parseExpression();
                consumeToken(Token::RIGHT_SQUARE);
                return em.makeAccess(id, index);
            }
            return id;
        }

        case Token::NUM:
        {
            std::string word = tokenItr->word;
            safe_next();
            return em.makeIntConstant(std::stoi(word));
        }

        //weather token is true or false boolConstant is created
        case Token::TRUE:

        case Token::FALSE:
        {
            std::string word = tokenItr->word;
            safe_next();
            if(word == Token::id2word[Token::TRUE])
                return em.makeBoolConstant(true);
            
            else if(word == Token::id2word[Token::FALSE])
                return em.makeBoolConstant(false);
        }

        default:
        {
            std::stringstream ss("Error while parsing factor");
            ss<<tokenItr->tag;
            throw ParseError(ss.str());
        }
    }       
}