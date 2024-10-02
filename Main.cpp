#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>

#include "Exceptions.h"
#include "Token.h"
#include "Tokenizer.h"
#include "ExpressionManager.h"
#include "Parser.h"
#include "Visitor.h"


int main(int argc, char* argv[]) {

    // Command line parsing
    if (argc < 2) {
        std::cerr << "File not found!" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <file_name>" << std::endl;
        return EXIT_FAILURE;
    }

    // Opening input file
    std::ifstream inputFile;
    try {
        inputFile.open(argv[1]);
    }
    catch (std::exception const& exc) {
        std::cerr << "Cannot open " << argv[1] << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Lexical analysis
    Tokenizer tokenize;
    std::vector<Token> inputTokens;
    try {
        inputTokens = std::move(tokenize(inputFile));
        inputFile.close();
    }
    catch (LexicalError const& le) {
        std::cerr << "Lexical error" << std::endl;
        std::cerr << le.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception const& exc) {
        std::cerr << "Cannot read from " << argv[1] << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    for(int i = 0; i<inputTokens.size(); i++)
    {
        std::cout<<inputTokens[i].tag<<" "<<inputTokens[i].word<<std::endl;
    }

    // Analisi sinttattica
    ExpressionManager manager;
    Parser parser(manager, inputTokens);
    Program* program = nullptr;
    try {
        program = parser();
    }
    catch (ParseError const& pe) {
        std::cerr << "Parse error" << std::endl;
        std::cerr << pe.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception const& exc) {
        std::cerr << "Unknown error" << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Valutazione (Analisi semantica)
    try {
        PrintVisitor* p = new PrintVisitor();
        std::cout << "L'espressione letta è ";
        program->accept(p);
        std::cout << std::endl;
        /*EvaluationVisitor* v = new EvaluationVisitor();
        expr->accept(v);
        std::cout << "Il valore dell'espressione è " << v->getValue() << std::endl;*/
    }
    catch (EvaluationError const& ee) {
        std::cerr << "Errore nella valutazione" << std::endl;
        std::cerr << ee.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception const& exc) {
        std::cerr << "Errore generico " << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
