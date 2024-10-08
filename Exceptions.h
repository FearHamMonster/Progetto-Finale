#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

struct LexicalError : std::runtime_error {
	LexicalError(const char* msg) : std::runtime_error(msg) { }
	LexicalError(std::string msg) : std::runtime_error(msg.c_str()) { }
};

struct ParseError : std::runtime_error {
	ParseError(const char* msg) : std::runtime_error(msg) { }
	ParseError(std::string msg) : std::runtime_error(msg.c_str()) { }
};

struct EvaluationError : std::runtime_error {
	EvaluationError(const char* msg) : std::runtime_error(msg) { }
	EvaluationError(std::string msg) : std::runtime_error(msg.c_str()) { }
};

#endif

