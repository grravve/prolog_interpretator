#pragma once
#ifndef AST
#define AST

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

#include "NewParser.h"
#include "Tokens.h"

struct ASTNode
{
	std::string nodeName = "";
	std::string tokenValue = "";
	int spaceCount = 0;
};


void ASTCreate(std::vector<Token>);
void ASTOut();
void ASTAddNode(std::string, std::string, int);
void ASTNextToken();
void ASTCheckStructure();
void ASTCheckHeader();
void ASTCheckRule();
void ASTCheckBody();
void ASTCheckList();
void ASTCheckExpression();
void TransferTmpToAST();
#endif 