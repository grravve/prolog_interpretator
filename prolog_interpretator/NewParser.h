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
void Structure();
void Header();
bool Dot();
bool Atom();
bool Variable();
bool OpenParenthesis();
bool CloseParenthesis();
void Term();
bool Comma();
bool RuleSign();
void RuleBody();
void Target();
void Expression();
bool Number();
void List();
void ListHeader();
bool VerticalBar();
bool OpenBracket();
bool CloseBracket();
bool IsOperator();
void ArithmeticExpression();
bool LogicOperator();
bool ArithmeticOperator();
bool CutOperator();
void ArithmeticExpression();
void CalculationArithmeticExpression();
void LogicalExpression();

#endif
