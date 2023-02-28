#pragma once
#ifndef NEWPARSER
#define NEWPARSER

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

void GetNextToken();
void Error(std::string);

void StartAnalysis(std::string str);

void Program();
void Sentence();
void Header();
bool Dot();
bool Structure();
bool Atom();
bool OpenParenthesis();
bool CloseParenthesis();
bool Term();
bool Comma();
bool RuleSign();
bool RuleBody();
bool Target();
bool Expression();
bool Operator();
bool Number();
bool List();
bool ListHeader();
bool VerticalBar();
bool OpenBracket();
bool CloseBracket();

#endif
