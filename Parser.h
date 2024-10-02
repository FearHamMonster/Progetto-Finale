#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "Node.h"
#include "ExpressionManager.h"
#include "Token.h"
/*#include "Exceptions.h"
#include "Expression.h"
#include "Stmt.h"
#include "Block.h"
#include "Program.h"
*/
// Function object per il parsing di espressioni
// Funzione di parsing: restituisce "true" se l'espressione � corretta
// "false" altrimenti. Suppone che nella stringa vi siano solo simboli
// terminali per la grammatica definita sopra.
class Parser {

public:
    Parser(ExpressionManager& manager, std::vector<Token>& tokenStream)
     : em{ manager }, tokenItr {tokenStream.begin()}, streamEnd{tokenStream.end()}{}
    ~Parser() = default;
    Parser(Parser const&) = delete;
    Parser& operator=(Parser const&) = delete;

    //un oggetto p di tipo Parser inizia il parsing chiamando p()
    Program* operator()() {
        Program* p = parseProgram();
        if (tokenItr != streamEnd) {
            throw ParseError("Unexpected end of input");
        }
        return p;
    }



private:
    std::vector<Token>::const_iterator streamEnd;

    //Vector di token prodotto dal tokenizer
    std::vector<Token>::const_iterator tokenItr;
  
    // Riferimento all'expression manager "di sistema"
    ExpressionManager& em;

    // Parser a discesa ricorsiva nella struttura dell'espressione
    //Expression* recursiveParse(std::vector<Token>::const_iterator& tokenItr);

    
    Program* parseProgram();

    Block* parseBlock();

    //parsing relativo a dichiarazioni
    Decls* parseDecls();
    Decl* parseDecl();
    Type* parseType();
    Id* parseId();
    //parsing relativo a stmts
    Stmts* parseStmts();
    Stmt* parseStmt();

    

    Expression* parseExpression();

    Expression* parseOr();
    Expression* parseAnd();
    Expression* parseEquality();
    Expression* parseRel();
    Expression* parseLowerPrecedenceBinOp();
    Expression* parseHigerPrecedenceBinOp();
    Expression* parseUnaryOp();
    Expression* parseFactor();


    // Avanzamento "sicuro" di un iteratore
    void safe_next() {
        if (tokenItr == streamEnd) {
            throw ParseError("Unexpected end of input");
        }
        ++tokenItr;
    }

    void consumeToken(const int tokenId)
    {
        if (tokenItr->tag == tokenId)
            safe_next();
        else 
            throw ParseError{
            std::string("Expecting ").
            append(Token::id2word[tokenId]).
            append(", instead found ").
            append(Token::id2word[tokenItr->tag])};
    }

};

#endif
