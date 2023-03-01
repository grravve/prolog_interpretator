#pragma once
#ifndef LEXER
#define LEXER
#include "Tokens.h"

Token GetToken(std::string string);
Token CreateNumberOrAtom(std::string string, bool isMinus);
std::string ReadLetter(std::string string);
std::string ReadNumber(std::string string, bool &isAtom, bool isMinus);
std::string ReadAtom(std::string string, int mode);
std::string ReadVariable(std::string);
void DeleteSpacebars(std::string);
void IncreaseCurrentChar();
void SkipSpaces(std::string string);
bool CheckNextChar(std::string string, char nextChar);


#endif LEXER

